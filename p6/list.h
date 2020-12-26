/**
 * @file list.h
 * @author Bilal Mohamad (bmohama)
 *
 * This file acts as the header file for the list.c file
 */

#ifndef LIST_H
#define LIST_H

// It's OK to make an alias for a type before you've defined the type.
typedef struct NodeStruct Node;

/** Representation for a node on the linked list, containing a string. */
struct NodeStruct {
    /** Dynamically allocated string stored in this node. */
    char *str;

    /** Pointer to the next node. */
    Node *next;

    /** Pointer to the previous node. */
    Node *prev;
};

/** Structure for the whole list, including head and tail pointers. */
typedef struct {
    /** Pointer to the first node on the list (or NULL ). */
    Node *head;

    /** Pointer to the first node on the list (or NULL ). */
    Node *tail;
} List;

/**
 * This function dynamically allocates a new list struct,
 * initializes it as an empty list and returns it.
 *
 * @return the empty list
 */
List *makeList();

/**
 * This function implements the report command.
 * It prints out all the strings on the list out to standard output, one per line.
 *
 * @param list the list to report
 */
void report(List *list);

/**
 * This function frees all the memory for the given list.
 *
 * @param list the list to free
 */
void freeList(List *list);

#endif
