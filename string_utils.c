#include "string_utils.h"

/**
 * Trims leading whitespace characters from a string.
 * 
 * This function moves the pointer to the first non-whitespace character
 * in the string, effectively "trimming" the whitespace from the start.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the first non-whitespace character in the string.
 */
char* ltrim(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}

/**
 * Trims trailing whitespace characters from a string.
 * 
 * This function iterates from the end of the string moving backwards
 * and places a null terminator '\0' after the last non-whitespace
 * character, effectively "trimming" the whitespace from the end.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return The same pointer passed in as the parameter, now pointing to a string
 *         with trailing whitespace removed.
 */
char* rtrim(char *s) {
    char* back = s + strlen(s);
    while (isspace((unsigned char)*--back));
    *(back + 1) = '\0';
    return s;
}

/**
 * Trims both leading and trailing whitespace characters from a string.
 * 
 * This function first trims the leading whitespace by calling `ltrim`
 * and then trims the trailing whitespace by calling `rtrim`.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the trimmed string. If the string is all whitespace,
 *         the returned pointer will point to the terminating null character.
 */
char* trim(char *s) {
    return rtrim(ltrim(s));  
}

/**
 * concatenateArgs - Concatenates an array of strings into a single string.
 *
 * This function takes an array of string pointers (`args`) and concatenates them
 * into a single string, separating each argument with a space. It iterates over
 * the `args` array and concatenates each non-NULL string. The function stops concatenation
 * when a NULL pointer is encountered, which indicates the end of valid strings in the array.
 * Memory is dynamically allocated for the concatenated string, so it must be freed by the caller
 * after use to prevent memory leaks. If memory allocation fails, the function returns NULL.
 *
 * @param args An array of strings (char pointers).
 * @return A pointer to the dynamically allocated concatenated string. The caller is responsible
 *         for freeing this memory. Returns NULL if memory allocation fails.
 */
char* concatenateArgs(char *args[]) {
    // Calculate the total length required for the concatenated string.
    size_t totalLength = 0;
    for (size_t i = 0; i < ARG_MAX && args[i] != NULL; ++i) {
        totalLength += strlen(args[i]) + 1; // +1 for space or end character.
    }

    // Allocate memory for the concatenated string.
    char *argsv = malloc(totalLength * sizeof(char));
    if (argsv == NULL) {
        perror("Unable to allocate memory for concatenated arguments");
        return NULL;
    }

    // Start with an empty string.
    argsv[0] = '\0';

    // Concatenate the arguments.
    for (size_t i = 0; i < ARG_MAX && args[i] != NULL; ++i) {
        strcat(argsv, args[i]);
        strcat(argsv, " "); // Add a space between arguments.
    }

    return argsv;
}
