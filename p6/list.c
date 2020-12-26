/**
 * @file list.c
 * @author Bilal Mohamad (bmohama)
 *
 * This implements the basic functionality of the doubly-linked list.
 * The list.h header has a struct definition for individual nodes on the list and a definition
 * of a List struct representing the whole list.
 */

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

/** The initial capacity of the list */
#define CAPACITY 100

/**
 * This function dynamically allocates a new list struct,
 * initializes it as an empty list and returns it.
 *
 * @return the empty list
 */
List *makeList() {

    List *this = (List *) malloc(CAPACITY * sizeof(List));

    this->head = NULL;
    this->tail = NULL;
    /*
     this->head.str = "";
     this->head.next = NULL;
     this->head.prev = NULL;

     this->tail.str = "";
     this->tail.next = NULL;
     this->tail.prev = NULL;
     */
    return this;
}

/**
 * This function implements the report command.
 * It prints out all the strings on the list out to standard output, one per line.
 *
 * @param list the list to report
 */
void report(List *list) {

    Node *current = list->head;

    while (current != NULL) {
        printf("%s\n", current->str);
        current = current->next;
    }

}

/**
 * This function frees all the memory for the given list.
 *
 * @param list the list to free
 */
void freeList(List *list) {

    if (list) {
        Node *temp = list->head;
        while (temp) {
            Node *next = temp->next;
            free(temp->str);
            free(temp);
            temp = next;
        }
    }

    free(list);
}
