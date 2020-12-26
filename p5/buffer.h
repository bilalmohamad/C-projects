/**
 * @file buffer.h
 * @author Bilal Mohamad (bmohama)
 *
 * This file acts as the header file for the buffer.c file
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

/** Representation for the contents of an input file, copied to memory. */
typedef struct {
  /** Array of bytes from the file (not a string). */
  unsigned char *data;

  /** Number of currently used bytes in the data array. */
  unsigned int len;

  /** Capacity of the data array (it's typically over-allocated. */
  unsigned int cap;
} Buffer;

/**
 * This function dynamically allocates a Buffer struct, initializes its fields
 * (a typical representation for a resizable array).
 *
 * @return the new Buffer struct
 */
Buffer *makeBuffer();

/**
 * This function adds a single byte to the end of the given buffer,
 * enlarging the data array if necessary.
 *
 * @param b the buffer to append to
 * @param byte the value to append to the buffer
 */
void appendBuffer( Buffer *b, unsigned char byte );

/**
 * This function frees all the memory for the given buffer.
 *
 * @param this the buffer to free
 */
void freeBuffer( Buffer *b );

/**
 * This function creates a new buffer, reads the contents of the file with the given name,
 * stores it in the buffer and returns a pointer to the buffer.
 * If the file can't be opened, it just returns NULL.
 *
 * @param filename the name of the file to read information from
 *
 * @return the buffer created from the input file
 */
Buffer *readFile( const char *filename );


#endif
