/**
 * @file command.h
 * @author Bilal Mohamad (bmohama)
 *
 * This file acts as the header file for the command.c file
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>
#include "list.h"

// Short name for the command type.
typedef struct CommandStruct Command;

/** Structure for a Command, an edit to what's on the list. */
struct CommandStruct {
    /** Pointer to a function to apply (or redo) the current command, making a
     change to the given list.
     @param list The list to modify. */
    void (*apply)( Command *this, List *list );

    /** Pointer to a function to undo the current command, making a
     change to the given list.
     @param list The list to modify. */
    void (*undo)( Command *this, List *list );

    /** Free memory used to represent this command. */
    void (*destroy)( Command *this );

    /** Different instances of Command may need to remember some extra data
     to be able to apply or undo themselves.  They can use this pointer
     to keep up with any data they need. */
    void *data;
};

/**
 * Apply the given edit command to the given list, and put it on
 * the undo list.
 *
 * @param list The list to modify.
 * @param cmd The command to apply.
 */
void applyCommand(Command *cmd, List *list);

/**
 * This function performs the undo operation,
 * undo-ing the most recent command in the undo list and putting it on the redo-list.
 * If there's no command to undo, it should return false.
 *
 * @param list the list used for the command
 *
 * @return true if there is a command to undo, otherwise false
 */
bool undoCommand(List *list);

/**
 * This function performs the redo operation,
 * redo-ing the most recent command in the redo list and putting it back on on the undo-list.
 * If there's no command to redo, it should return false.
 *
 * @param list the list used for the command
 *
 * @return true if there is a command to undo, otherwise false
 */
bool redoCommand(List *list);

/**
 * When the program exits,
 * there may still be commands on the undo list and the redo list that are using some memory.
 * This function frees all that memory.
 */
void freeHistory();

#endif
