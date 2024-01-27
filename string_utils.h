/* string_utils.h */

/**
 * This header file contains declarations for functions that perform
 * trimming operations on strings. Trimming is the process of removing
 * leading and trailing whitespace characters from a string.
 * 
 * The functions provided are:
 * - ltrim: Trims leading whitespace.
 * - rtrim: Trims trailing whitespace.
 * - trim: Trims both leading and trailing whitespace.
 *
 * Usage of these functions is recommended when processing strings that
 * might contain unwanted whitespace, particularly user input, or text
 * retrieved from files or networks.
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define ANSI color codes with bright/bold modifiers
#define COLOR_GREEN_BOLD "\x1b[1;32m" // Bright green color
#define COLOR_BLUE_BOLD "\x1b[1;34m"  // Bright blue color
#define COLOR_RESET "\x1b[0m"	

/**
 * Trims leading whitespace characters from a string.
 *
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the first non-whitespace character in the string.
 */
char* ltrim(char *s);

/**
 * Trims trailing whitespace characters from a string.
 *
 * @param s Pointer to the string to be trimmed.
 * @return The same pointer passed in as the parameter, now pointing to a string
 *         with trailing whitespace removed.
 */
char* rtrim(char *s);

/**
 * Trims both leading and trailing whitespace characters from a string.
 *
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the trimmed string. If the string is all whitespace,
 *         the returned pointer will point to the terminating null character.
 */
char* trim(char *s);

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
char* concatenateArgs(char *args[]);

#endif /* STRING_UTILS_H */
