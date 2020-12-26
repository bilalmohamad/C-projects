/**
 * @file deque.c
 * @author Bilal Mohamad (bmohama)
 *
 * This is the top-level, main component.
 * It parses user input and either performs the command immediately
 * (for commands that don't alter the list) or creates a Command object for it and gives
 * it to the command component (for commands that do change the list).
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "list.h"
#include "command.h"

/** A default size used for the size of the command */
#define COMMAND_BUFFER 11

/** The capacity of the strings that are added to the list */
#define STRING_CAP 100

/** String constant used for one of the eight possible commands */
#define QUIT "quit"
/** String constant used for one of the eight possible commands */
#define UNDO "undo"
/** String constant used for one of the eight possible commands */
#define REDO "redo"
/** String constant used for one of the eight possible commands */
#define REPORT "report"
/** String constant used for one of the eight possible commands */
#define PUSH_BACK "push-back"
/** String constant used for one of the eight possible commands */
#define PUSH_FRONT "push-front"
/** String constant used for one of the eight possible commands */
#define POP_BACK "pop-back"
/** String constant used for one of the eight possible commands */
#define POP_FRONT "pop-front"

/**
 * Apply method for the PushBack command.
 *
 * @param this the command to store the action
 * @param list the list to store the data
 */
static void pushBack(Command *this, List *list) {
    // Get our node we've already made.
    Node *n = (Node *) this->data;

    // Link this node into the tail of the list.
    n->next = NULL;
    if (list->tail) {
        // There's already a node on the list.
        n->prev = list->tail;
        list->tail->next = n;
    } else {
        // The list was previously empty.
        n->prev = NULL;
        list->head = n;
    }

    list->tail = n;

    // The node for this command is currently on the list.
    this->data = NULL;
}

/**
 * Apply method for the PushFront command.
 *
 * @param this the command to store the action
 * @param list the list to store the data
 */
static void pushFront(Command *this, List *list) {
    // Get our node we've already made.
    Node *n = (Node *) this->data;

    // Link this node into the tail of the list.
    n->prev = NULL;
    if (list->head) {
        // There's already a node on the list.
        n->next = list->head;
        list->head->prev = n;
    }
    else {
        // The list was previously empty.
        n->next = NULL;
        list->tail = n;
    }

    list->head = n;

    // The node for this command is currently on the list.
    this->data = NULL;
}

/**
 * Undo method for the PushBack command.
 *
 * @param this the command to store the action
 * @param list the list to store the data
 */
static void popBack(Command *this, List *list) {
    // Remove the last node from the list.
    Node *n = list->tail;
    if (list->tail->prev) {
        // List will still be non-empty after the remove.
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    else {
        // The list is now empty.
        list->tail = NULL;
        list->head = NULL;
    }

    // Remember the node we just removed, in case we want to re-insert it later.
    this->data = n;
}

/**
 * Undo method for the PushFront command.
 *
 * @param this the command to store the action
 * @param list the list to store the data
 */
static void popFront(Command *this, List *list) {
    // Remove the last node from the list.
    Node *n = list->head;
    if (list->head->next) {
        // List will still be non-empty after the remove.
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    else {
        // The list is now empty.
        list->tail = NULL;
        list->head = NULL;
    }

    // Remember the node we just removed, in case we want to re-insert it later.
    this->data = n;
}

/**
 * Destroy for a pushBack or pushFront command
 *
 * @param this the command containing the current action
 */
static void destroyCommand(Command *this) {
    if (this->data) {
        // If this command has a non-null data pointer, then it's a
        // pointer to a node is not part of the list, so we need to free
        // it when we free the command.
        Node *n = (Node *) this->data;
        free(n->str);
        free(n);
    }
    free(this);
}

/**
 * Make a Command object that knows how to push a new string on
 * the back of a list.
 *
 * @param str The new string to put on the list.
 *
 * @return a pointer to the new command object.
 */
static Command *makePushBack(char *str) {
    // Make the command object and fill in its method pointers.
    Command *cmd = (Command *) malloc(sizeof(Command));
    cmd->apply = pushBack;
    cmd->undo = popBack;
    cmd->destroy = destroyCommand;

    // Go ahead and make the node we're going to append in the
    // apply operation, and store it in the command's data field.
    Node *n = (Node *) malloc(sizeof(Node));
    n->str = (char *) malloc(strlen(str) + 1);
    strcpy(n->str, str);
    cmd->data = n;

    return cmd;
}

/**
 * Make a Command object that knows how to push a new string on
 * the front of a list.
 *
 * @param str The new string to put on the list.
 *
 * @return a pointer to the new command object.
 */
static Command *makePushFront(char *str) {

    Command *cmd = (Command *) malloc(sizeof(Command));
    cmd->apply = pushFront;
    cmd->undo = popFront;
    cmd->destroy = destroyCommand;

    Node *n = (Node *) malloc(sizeof(Node));
    n->str = (char *) malloc(strlen(str) + 1);
    strcpy(n->str, str);
    cmd->data = n;

    return cmd;
}

/**
 * Make a Command object that knows how to pop a Command object on
 * the back of a list.
 *
 * @return a pointer to the new command object.
 */
static Command *makePopBack() {

    Command *cmd = (Command *) malloc(sizeof(Command));
    cmd->apply = popBack;
    cmd->undo = pushBack;
    cmd->destroy = destroyCommand;

    Node *n = (Node *) malloc(sizeof(Node));
    cmd->data = n;

    return cmd;

}

/**
 * Make a Command object that knows how to pop a Command object on
 * the front of a list.
 *
 * @return a pointer to the new command object.
 */
static Command *makePopFront() {

    Command *cmd = (Command *) malloc(sizeof(Command));
    cmd->apply = popFront;
    cmd->undo = pushFront;
    cmd->destroy = destroyCommand;

    Node *n = (Node *) malloc(sizeof(Node));
    cmd->data = n;

    return cmd;
}

/**
 * Starting point for the program.
 *
 * @return program exit status
 */
int main() {

    char *cmdBuffer = (char *) malloc(COMMAND_BUFFER * sizeof(char));
    List *list = makeList();
    printf("> ");

    while (fscanf(stdin, "%s", cmdBuffer) != EOF) {

        printf("%s", cmdBuffer);

        if (strcmp(cmdBuffer, QUIT) == 0) {
            printf("\n");
            break;
        }
        else if (strcmp(cmdBuffer, REPORT) == 0) {
            printf("\n");
            report(list);
        }
        else if (strcmp(cmdBuffer, REDO) == 0) {
            if (redoCommand(list) == false) {
                printf("\nInvalid command");
            }
            printf("\n");
        }
        else if (strcmp(cmdBuffer, UNDO) == 0) {
            if (undoCommand(list) == false) {
                printf("\nInvalid command");
            }
            printf("\n");
        }
        else if (strcmp(cmdBuffer, PUSH_BACK) == 0) {
            char *str = (char *) malloc(STRING_CAP * sizeof(char));
            fscanf(stdin, "%s", str);
            Command *cmd = makePushBack(str);
            applyCommand(cmd, list);
            printf(" %s\n", str);
        }
        else if (strcmp(cmdBuffer, PUSH_FRONT) == 0) {
            char *str = (char *) malloc(STRING_CAP * sizeof(char));
            fscanf(stdin, "%s", str);
            Command *cmd = makePushFront(str);
            applyCommand(cmd, list);
            printf(" %s\n", str);
        }
        else if (strcmp(cmdBuffer, POP_BACK) == 0) {
            Command *cmd = makePopBack();
            applyCommand(cmd, list);
            printf("\n");
        }
        else if (strcmp(cmdBuffer, POP_FRONT) == 0) {
            Command *cmd = makePopFront();
            applyCommand(cmd, list);
            printf("\n");
        }
        else {
            printf("\nInvalid command\n");
        }

        printf("> ");
    }

    freeList(list);
    freeHistory();
    free(cmdBuffer);
    return EXIT_SUCCESS;
}
