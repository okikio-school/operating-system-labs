#include "myshell.h"
#include "kernel_commands.h"

volatile sig_atomic_t sigint_received = 0;
static void sigint_handler() {
	sigint_received = 1;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

	// Setup the handler
	sa.sa_handler = &sigint_handler;
	// Interupt system calls, this entire section of code is necessary because
	// we're reading input using `fgets` from the stdin
	// the only way to tell `fgets` to stop when working with signals 
	// is to set the action the signal should take, in this case interrupt/cancelling 
	// all system calls
	sa.sa_flags = SA_INTERRUPT;
	// Block every signal during the handler
	sigfillset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
	}

	char cwd[PATH_MAX];
	char hostName[HOST_NAME_MAX];
	struct passwd *pw;

	uid_t uid;
	pid_t pid;

	while (true) {
        if (sigint_received) {
            // Reset the flag and continue the loop
            sigint_received = 0;
			printf("\n");
            continue;
        }

		if (getpid() != -1) {
			printf("(PID: %d) ", getpid());
		}

		// Retrieve the effective user ID of the calling process
		uid = geteuid();
		// Get the password database entry for the user ID
		pw = getpwuid(uid);

		// Print the username in green
		if (pw) {
			printf(COLOR_GREEN_BOLD "%s@" COLOR_RESET, pw->pw_name);
		}

		// Print the hostname in green
		int hostNameResult = gethostname(hostName, sizeof(hostName));
		if (hostNameResult == 0) {
			printf(COLOR_GREEN_BOLD "%s" COLOR_RESET, hostName);
		}

		// Print a colon only if both username and hostname are available
		if (pw && hostNameResult == 0) {
			printf(":");
		}

		// Print the current working directory followed by a dollar sign
		printf(COLOR_BLUE_BOLD "%s" COLOR_RESET, getcwd(cwd, sizeof(cwd)));
		printf("$ ");

		// Flush stdout to make sure all colors are reset before any other program prints to the terminal
		fflush(stdout);

		// Read a line of input, including whitespaces
		char input_char[ARG_MAX];
		// fgets(input_char, sizeof(input_char), stdin);
		if (fgets(input_char, sizeof(input_char), stdin) == NULL) {
            // Check if fgets returned NULL due to a signal interruption
            if (errno == EINTR && sigint_received) {
                printf("\n");
                sigint_received = 0; // Reset the flag
                clearerr(stdin); // Clear the error state of the stream
                continue; // Continue to the next iteration of the loop
            }
            // Handle other errors or end-of-file (EOF)
            // ...
        }

		// Remove the newline character if it's read by fgets
		input_char[strcspn(input_char, "\n")] = 0;
		
		// Returns first token 
		char *token = strtok(trim(input_char), SPACE_DELIMETER);
		char *full_command[ARG_MAX];
		char command[ARG_MAX];
		char *args[ARG_MAX];
		int i = 0;

		if (token != NULL) {
			strcpy(command, trim(token));
			printf("Command: %s\n", command);
			full_command[i] = command;

			// Token string
			token = strtok(NULL, SPACE_DELIMETER);
		}

		// Keep printing tokens while one of the
		// delimiters present in str[].
		while (token != NULL)
		{
			printf("Arg: %s\n", token);
			args[i] = token;
			full_command[i + 1] = token;
			i++;

			// Token string
			token = strtok(NULL, SPACE_DELIMETER);
		}

		if (strcmp(command, "cd") == 0) {
			if (args[0] != NULL && args[1] == NULL) {
				changeDirectory(args[0]);
			} else if (args[0] == NULL) {
				changeDirectory(getenv("HOME"));
			} else {
				printf("myshell: cd: too many arguments\n");
			}
		} else if (
			strcmp(command, "quit") == 0 || 
			strcmp(command, "exit") == 0
		) {
			quitShell();
		} else if (strcmp(command, "clr") == 0) {
			if (args[0] != NULL) {
				printf("Usage: clear\n");
			} else {
				clearScreen();
			}
		} else if (strcmp(command, "environ") == 0) {
			listEnviron();
		} else if (strcmp(command, "dir") == 0) {
			if (args[0] != NULL && args[1] != NULL) {
				printf("Usage: dir [path]\n");
			} else {
				if (args[0] != NULL) {
					listFiles(args[0]);
				} else {
					listFiles(getcwd(cwd, sizeof(cwd)));
				}
			}
		} else if (strcmp(command, "echo") == 0) {
			char *argsv = concatenateArgs(args);

			if (argsv != NULL) {
				echo(argsv);
				free(argsv); // Free the dynamically allocated memory.
			} else {
				printf("Error concatenating args for %s", command);
			}
		} else if (strcmp(command, "pwd") == 0) {
			printWorkingDirectory();
		} else if (strcmp(command, "help") == 0) {
			help();
		} else if (strcmp(command, "pause") == 0) {
			pauseShell();
		} else {
			// Child Process
			if ((pid = fork()) == 0) {
				// Move the child process to a new process group, 
				// to ensure it can handle signals seperately from the parent process
				// as in when the child process recieves a signal we don't want said signal 
				// to also be fired on the parent process, it creates bugs and sometimes 
				// infinite loops
				if (setpgid(0, 0) == -1) {
					// setpgid(0, 0) sets the process group ID of the child process
					// to its own process ID, effectively creating a new process group.
					// This is necessary to separate the child from the parent's process group,
					// allowing the child to handle signals like SIGINT independently.
					perror("setpgid failed");
					exit(EXIT_FAILURE);
				}

				// Now the child process is in its own process group, separate from the parent.
				// The child can handle signals (e.g., SIGINT from pressing Ctrl+C) independently.
				// Reset signal handling to default for the child process
				if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
					perror("Signal error in child");
					exit(EXIT_FAILURE);
				}

				char *argsv = concatenateArgs(full_command);
				printf("full_command: --%s--\n", argsv);
				free(argsv);

				// Child Process					
				execvp(command, full_command);
				printf("Command not found\n");

				// If execvp[] returns, it means it failed to execute the command.
				perror("execvp"); // Print the execvp error.
				exit(EXIT_FAILURE); // Exit the child process if execvp fails.
			} else {
				// Parent Process

				// The parent process can wait for the child process to complete.
				// The parent and child are in separate groups, so signals sent to one
				// do not affect the other.
				int status;
				waitpid(pid, &status, 0);

				if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
					printf("Exit status: %d\n", WEXITSTATUS(status));
				}
			}

		}

	}

	printf("\n");
	return 0;
}