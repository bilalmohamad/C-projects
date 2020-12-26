/**
 * @file buffer.c
 * @author Bilal Mohamad (bmohama)
 *
 * This is the component file that will implement the functions from the buffer.h file.
 * This file contains functions that are capable of performing cut, copy, and paste operations.
 */

#include "buffer.h"
#include "document.h"

static char buffer[MAX_LENGTH + 1];

/**
 * It modifies the given string (line) by removing n characters starting at
 * the given starting index.
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
 */
bool cut( char *line, int start, int n ){


    int size = strlen(line);

    if (size == 0){

    }
    else{

        /*printf("Size: %d\n", size);
        int num = start + n - 1;
        printf("Start: %d\n", num);*/

        if (start + n - 1 > size || start <= 0 || n <= 0){
            return false;
        }

        strncpy(buffer, &line[start - 1], n);
        buffer[n] = '\0';

        for (int i = start - 1; i < size - n; i++){
            line[i] = line[i + n];
        }

        for (int i = size - n; i < size; i++){
            line[i] = '\0';
        }

    }

    return true;
}


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
bool copy( char *line, int start, int n ){

    int size = strlen(line);
    /*printf("Size: %d\n", size);
    int num = start + n - 1;
    printf("Start: %d\n", num);*/

    if (size == 0){

    }
    else{

        if (start + n - 1 > size || start <= 0 || n <= 0){
            return false;
        }

        strncpy(buffer, &line[start - 1], n);
        buffer[n] = '\0';

    }

    return true;
}


/**
 * It inserts the text in the cut-and-paste buffer into the given string (line)
 * starting at the given start position.
 * It returns false if the paste operation can't be performed
 * (e.g., if the resulting string would exceed the maximum line length).
 *
 * @param line    the line being observed and modified
 * @param start   the starting index of the line
 *
 * @return    true    if the input was valid to perform the function
 *            false   otherwise
 *
 */
bool paste( char *line, int start ){

    //TODO DOUBLE-CHECK
    //PLUS 1 MAY OR MAY NOT BE NECESSARY
    int size = strlen(line);
    int bufferSize = strlen(buffer);
//    printf("Size: %d\n", size);

    if (size == 0){

    }
    else{

        if (size + bufferSize > MAX_LENGTH || bufferSize == 0 || start <= 0){
            return false;
        }


        for (int i = size - 1; i >= start - 1; i--){
            line[i + bufferSize] = line[i];
        }

        for (int i = 0; buffer[i]; i++){
            line[i + start - 1] = buffer[i];
        }

    }

    return true;
}
