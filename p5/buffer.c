/**
 * @file buffer.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component is responsible for reading and storing the input file.
 * It uses a struct named Buffer to store the entire file contents in memory.
 * This makes it easy to compute with and to add padding at the start of the MD5 computation.
 */

#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"

/** The initial capacity of the buffer array */
#define CAP 3

/**
 * This function dynamically allocates a Buffer struct, initializes its fields
 * (a typical representation for a resizable array).
 *
 * @return the new Buffer struct
 */
Buffer *makeBuffer() {

    Buffer *this = (Buffer *) malloc(CAP * sizeof(Buffer));
    this->cap = CAP;
    this->data = (unsigned char *) malloc(this->cap * sizeof(unsigned char));
    this->len = 0;

    return this;
}

/**
 * This function adds a single byte to the end of the given buffer,
 * enlarging the data array if necessary.
 *
 * @param b the buffer to append to
 * @param byte the value to append to the buffer
 */
void appendBuffer( Buffer *b, unsigned char byte ){

    if (b->len >= b->cap){

        b->cap *= 2;
        b->data = (unsigned char *) realloc(b->data, b->cap * sizeof(unsigned char));
    }

    b->data[b->len] = byte;
    b->len++;
}

/**
 * This function frees all the memory for the given buffer.
 *
 * @param this the buffer to free
 */
void freeBuffer(Buffer *this){

    free(this->data);
    free(this);
}

/**
 * This function creates a new buffer, reads the contents of the file with the given name,
 * stores it in the buffer and returns a pointer to the buffer.
 * If the file can't be opened, it just returns NULL.
 *
 * @param filename the name of the file to read information from
 *
 * @return the buffer created from the input file
 */
Buffer *readFile( const char *filename ){

    FILE *fp = fopen(filename, "r");
    if (!fp){
        return NULL;
    }

    Buffer *b = makeBuffer();
    while (1){

        int byte = fgetc(fp);
        if (byte == EOF){
            break;
        }

        appendBuffer(b, byte);
    }

    return b;
}
