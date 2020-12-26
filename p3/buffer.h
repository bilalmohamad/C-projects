/**
 * @file buffer.h
 * @author Bilal Mohamad (bmohama)
 *
 * This is the header file used by the buffer.c file.
 * This file contains functions that are capable of performing cut, copy, and paste operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/**
 * If modifies the given string (line) by removing n characters starting at the
 * given starting index.
 * The text that's removed is automatically copied to the cut-and-paste buffer.
 * It returns false and leaves the line unmodified if the cut operation can't be performed
 * (e.g., if the given range of characters extends past the end of the given string).
 *
 * @param line    the line being observed and modified
 * @param start   the starting index of the line
 * @param n       the number of characters to cut
 *
 * @return    true    if the input was valid to perform the function
 *            false   otherwise
 *
 */
bool cut( char *line, int start, int n );


/**
 * It copies text from the given string to the cut-and-paste buffer.
 * The copied text is n characters starting at the given start index.
 * Unlike the cut() operation, this function doesn't actually remove the text that's being copied.
 * It should return false and leave line unmodified if the copy operation can't be performed
 * (e.g., if the given range of characters extends past the end of the given string).
 *
 * @param line    the line being observed and modified
 * @param start   the starting index of the line
 * @param n       the number of characters to copy
 *
 * @return    true    if the input was valid to perform the function
 *            false   otherwise
 */
bool copy( char *line, int start, int n );


/**
 * It inserts the text in the cut-and-paste buffer into the given string (line)
 * starting at the given start position.
 * It returns false if the paste operation can't be performed
 * (e.g., if the resulting string would exceed the maximum line length).
 *
 * @param line    the line being observed and modified
 * @param start    the starting index of the line
 *
 * @return    true    if the input was valid to perform the function
 *            false   otherwise
 */
bool paste( char *line, int start );
