/**
 * @file document.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component file contains the implementation of the buffer.h header file.
 * This file can read in information from a file and store it in a 2D array.
 * This file can also write the information stored in the 2D array to an output file.
 *
 */

#include "document.h"

/**
 * It reads a text file and stores it in the given 2D array of characters, one line per row.
 * If it's successful, it returns the number of lines read into the document,
 * but if the input file is bad (e.g., too many lines),
 * this function can print an error message and terminate the program immediately.
 *
 * @param fp    a file pointer to the file being read in
 * @param doc   the 2D array that will store the information found in the entered file
 *
 * @return    the number of lines in the file
 */
int readDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH + 1 ] ){

    int lines = 1;
    int row = 0;
    int col = 0;
    char val;

    while (fscanf(fp, "%c", &val) == 1){

        doc[row][col] = val;

        if (val == '\n'){
            lines++;
            row++;
            col = -1;
        }

        col++;
    }

    return lines;
}


/**
 * It writes out the given document to the given output stream.
 *
 * @param fp    a file pointer to the file that is being written to
 * @param doc   the 2D array containing the information read in from the input file
 * @param lines the number of lines in the input file
 */
void printDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH +    1 ], int lines ){

    for (int i = 0; i < lines; i++){
        fprintf(fp, "%s", doc[i]);
    }
}
