/**
 * @file document.h
 * @author Bilal Mohamad (bmohama)
 *
 * This is the header file for the document.c file.
 * This file contains a function that can read in information from a file and
 * store it in a 2D array.
 * This file contains a function that can also write the information
 * stored in the 2D array to an output file.
 *
 */

#include <stdio.h>

/** Maximum length of a line in the document. */
#define MAX_LENGTH 100

/** Maximum number of lines in the document. */
#define MAX_LINES 10000

/**
 * It reads a text file and stores it in the given 2D array of characters, one line per row.
 * If it's successful, it returns the number of lines read into the document,
 * but if the input file is bad (e.g., too many lines),
 * this function can print an error message and terminate the program immediately.
 *
 * @param fp    a file pointer to the file being read in
 * @param doc   the 2D array that will store the information found in the entered file
 *
 * @return   the number of lines in the file
 */
int readDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH + 1 ] );

/**
 * It writes out the given document to the given output stream.
 *
 * @param fp    a file pointer to the file that is being written to
 * @param doc   the 2D array containing the information read in from the input file
 * @param lines the number of lines in the input file
 */
void printDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH + 1 ], int lines );
