#define _PROGRAM_NAME "myshell"
#define _AUTHOR_NAME "Okiki Ojo"

#include "constants.h"
#include "kernel_commands.h"
#include "string_utils.h"

#include <unistd.h>
#include <pwd.h>

#include <errno.h>
#include <string.h>
#include <limits.h>		  // Used for HOST_NAME_MAX
#include <linux/limits.h> // Include the Linux-specific limits.h
#include <bits/local_lim.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>

int main(int argc, char *argv[]);