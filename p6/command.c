/**
 * @file command.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component defines the Command structure. It also maintains the undo / redo lists
 * and provides functions to apply, undo and redo commands.
 */

#include "command.h"
#include <stdlib.h>

/** Maximum number of commands on the undo or redo lists. */
#define HIST_MAX 10

/** List of commands that have been applied, available for undo-ing. */
static Command *undoList[HIST_MAX];

/** Number of commands on the past list. */
static int undoLen = 0;

/** List of commands that have been undone, available for redo-ing. */
static Command *redoList[HIST_MAX];

/** Number of commands on the future list. */
static int redoLen = 0;

/**
 * Apply the given edit command to the given list, and put it on
 * the undo list.
 *
 * @param list The list to modify.
 * @param cmd The command to apply.
 */
void applyCommand(Command *cmd, List *list) {
    cmd->apply(cmd, list);

    // If the undo list is full, slide everything down to make room (a circular
    // queue would be more efficient here.)
    if (undoLen >= HIST_MAX) {
        undoList[0]->destroy(undoList[0]);
        for (int i = 0; i + 1 < undoLen; i++)
            undoList[i] = undoList[i + 1];
        undoLen -= 1;
    }

    // The redo-history goes away whenever we apply a new command (since we may
    // no longer be able to redo those commands).
    for (int i = redoLen - 1; i >= 0; i--)
        redoList[i]->destroy(redoList[i]);
    redoLen = 0;

    // Put the new command at the end of the undo list.
    undoList[undoLen++] = cmd;
}

/**
 * This function performs the undo operation,
 * undo-ing the most recent command in the undo list and putting it on the redo-list.
 * If there's no command to undo, it should return false.
 *
 * @param list the list used for the command
 *
 * @return true if there is a command to undo, otherwise false
 */
bool undoCommand(List *list) {

    if (undoLen == 0) {
        return false;
    }

    Command *cmd = undoList[undoLen - 1];
    undoLen--;
    redoList[redoLen++] = cmd;
    cmd->undo(cmd, list);

    return true;
}

/**
 * This function performs the redo operation,
 * redo-ing the most recent command in the redo list and putting it back on on the undo-list.
 * If there's no command to redo, it should return false.
 *
 * @param list the list used for the command
 *
 * @return true if there is a command to undo, otherwise false
 */
bool redoCommand(List *list) {

    if (redoLen == 0) {
        return false;
    }

    Command *cmd = redoList[redoLen - 1];
    redoLen--;
    undoList[undoLen++] = cmd;
    cmd->apply(cmd, list);

    return true;
}

/**
 * When the program exits,
 * there may still be commands on the undo list and the redo list that are using some memory.
 * This function frees all that memory.
 */
void freeHistory() {

    for (int i = 0; i < undoLen; i++) {
        free(undoList[i]);
    }
//  free(undoList);

    for (int i = 0; i < redoLen; i++) {
        free(redoList[i]);
    }
//  free(redoList);
}
