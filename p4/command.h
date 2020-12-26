/**
 * @file command.h
 * @author Bilal Mohamad (bmohama)
 *
 * This file acts as the interface for the command.c file
 */

#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include "packet.h"
#include "policy.h"

/** Constant used for Default command */
#define DEFAULT 1
/** Constant used for Insert command */
#define INSERT 2
/** Constant used for Append command */
#define APPEND 3
/** Constant used for Delete command */
#define DELETE 4
/** Constant used for Test command */
#define TEST 5
/** Constant used for Print command */
#define PRINT 6
/** Constant used for Help command */
#define HELP 7
/** Constant used for Quit command */
#define QUIT 8

/** Constant used for the size of the line */
#define LINE_SIZE 128

/** Constant used for the number of tokens */
#define TOKENS 6

/** Constant used for an estimated line length from the input */
#define EST_LINE 40

/** Representation for a command that has been parsed. */
typedef struct fw_cmd {
    // Fill in your own representation.
    int cmd;
    unsigned int action;
    int pos;
    protocol_t protocol;
    ipaddr_t src_ip;
    port_match_t src_port;
    ipaddr_t dst_ip;
    port_match_t dst_port;

} fw_cmd_t;

/**
 * This function will parse the next command from stream and populate the fw_cmd_t structure.
 *
 * @param stream the file stream being observed
 * @param cmd the command to be populated
 *
 * @return 0 if successful, and -1 if there is a failure
 */
int parse_command(FILE *stream, fw_cmd_t *cmd);

#endif
