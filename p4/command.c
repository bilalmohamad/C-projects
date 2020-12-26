/**
 * @file command.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component is responsible for functionality pertaining to parsing commands.
 * It contains features used by the top-level component, fwsim.c,
 * but it should never make calls to code in fwsim.c.
 */

#include "command.h"

/** Constant used for the index 3 */
#define INDEX3 3

/** Constant used for the index 4 */
#define INDEX4 4

/** Constant used for the index 5 */
#define INDEX5 5

/** Size of the nums array */
#define NUMS_SIZE 4

/** Size of the port array */
#define PORT_SIZE 10

/** Constant for the tokens of the ip */
#define IP_TOKENS 5

/**
 * This function will parse the next command from stream and populate the fw_cmd_t structure.
 *
 * @param stream the file stream being observed
 * @param cmd the command to be populated
 *
 * @return 0 if successful, and -1 if there is a failure
 */
int parse_command(FILE *stream, fw_cmd_t *cmd) {

    char buffer[LINE_SIZE];
    int i = 0;

    //Read one line from file
    while (1) {
        int ch = fgetc(stream);

        if (ch == '\n' || ch == EOF) {
            break;
        }

        buffer[i++] = ch;
    }

    //Null terminator
    buffer[i] = '\0';

    char buff[TOKENS][EST_LINE];
    int tokens = sscanf(buffer, "%s %s %s %s %s %s", buff[0], buff[1], buff[2],
            buff[INDEX3], buff[INDEX4], buff[INDEX5]);

    if (tokens == 0) {
        return 0;
    }

    //For DEFAULT
    if (strcmp(buff[0], "default") == 0) {
        cmd->cmd = DEFAULT;

        if (strcmp(buff[1], "allow") == 0) {
            cmd->action = ACTION_ALLOW;
        } else if (strcmp(buff[1], "deny") == 0) {
            cmd->action = ACTION_DENY;
        } else {
            //PRINT ERROR MESSAGE
            return -1;
        }

        return 0;
    }

    //For INSERT
    else if (strcmp(buff[0], "insert") == 0) {
        cmd->cmd = INSERT;

        //POS
        if (sscanf(buff[1], "%d", &cmd->pos) != 1) {
            //PRINT ERROR
            return -1;
        }

        //ACTION
        if (strcmp(buff[2], "allow") == 0) {
            cmd->action = ACTION_ALLOW;
        } else if (strcmp(buff[2], "deny") == 0) {
            cmd->action = ACTION_DENY;
        }

        //PROTOCOL
        if (strcmp(buff[INDEX3], "tcp") == 0) {
            cmd->protocol = PROTO_TCP;
        } else if (strcmp(buff[INDEX3], "udp") == 0) {
            cmd->protocol = PROTO_UDP;
        } else {
            return -1;
        }

        //SRC
        char port[PORT_SIZE];
        unsigned int nums[NUMS_SIZE];
        if (sscanf(buff[INDEX4], "%u.%u.%u.%u:%s", &nums[0], &nums[1], &nums[2],
                &nums[INDEX3], port) != IP_TOKENS) {
            return -1;
        }

        if (nums[0] > IP_OCTET_MAX || nums[0] < IP_OCTET_MIN
                || nums[1] > IP_OCTET_MAX || nums[1] < IP_OCTET_MIN
                || nums[2] > IP_OCTET_MAX || nums[2] < IP_OCTET_MIN
                || nums[INDEX3] > IP_OCTET_MAX || nums[INDEX3] < IP_OCTET_MIN) {
            return -1;
        }

        cmd->src_ip.a = nums[0];
        cmd->src_ip.b = nums[1];
        cmd->src_ip.c = nums[2];
        cmd->src_ip.d = nums[INDEX3];

        if (port[0] == '*') {
            cmd->src_port = MATCH_PORT_ANY;
        } else if (sscanf(port, "%u", &cmd->src_port) == 1) {
            if (cmd->src_port > PORT_MAX || cmd->src_port < PORT_MIN) {
                return -1;
            }
        } else {
            return -1;
        }

        //DST
        if (sscanf(buff[INDEX5], "%u.%u.%u.%u:%s", &nums[0], &nums[1], &nums[2],
                &nums[INDEX3], port) != IP_TOKENS) {
            return -1;
        }

        if (nums[0] > IP_OCTET_MAX || nums[0] < IP_OCTET_MIN
                || nums[1] > IP_OCTET_MAX || nums[1] < IP_OCTET_MIN
                || nums[2] > IP_OCTET_MAX || nums[2] < IP_OCTET_MIN
                || nums[INDEX3] > IP_OCTET_MAX || nums[INDEX3] < IP_OCTET_MIN) {
            return -1;
        }

        cmd->dst_ip.a = nums[0];
        cmd->dst_ip.b = nums[1];
        cmd->dst_ip.c = nums[2];
        cmd->dst_ip.d = nums[INDEX3];

        if (port[0] == '*') {
            cmd->dst_port = MATCH_PORT_ANY;
        } else if (sscanf(port, "%u", &cmd->dst_port) == 1) {
            if (cmd->dst_port > PORT_MAX || cmd->dst_port < PORT_MIN) {
                return -1;
            }
        } else {
            return -1;
        }

        return 0;
    }

    //For APPEND
    else if (strcmp(buff[0], "append") == 0) {
        cmd->cmd = APPEND;

        //ACTION
        if (strcmp(buff[1], "allow") == 0) {
            cmd->action = ACTION_ALLOW;
        } else if (strcmp(buff[1], "deny") == 0) {
            cmd->action = ACTION_DENY;
        }

        //PROTOCOL
        if (strcmp(buff[2], "tcp") == 0) {
            cmd->protocol = PROTO_TCP;
        } else if (strcmp(buff[2], "udp") == 0) {
            cmd->protocol = PROTO_UDP;
        } else {
            return -1;
        }

        //SRC
        char port[PORT_SIZE];
        unsigned int nums[INDEX4];
        if (sscanf(buff[INDEX3], "%u.%u.%u.%u:%s", &nums[0], &nums[1], &nums[2],
                &nums[INDEX3], port) != IP_TOKENS) {
            return -1;
        }

        if (nums[0] > IP_OCTET_MAX || nums[0] < IP_OCTET_MIN
                || nums[1] > IP_OCTET_MAX || nums[1] < IP_OCTET_MIN
                || nums[2] > IP_OCTET_MAX || nums[2] < IP_OCTET_MIN
                || nums[INDEX3] > IP_OCTET_MAX || nums[INDEX3] < IP_OCTET_MIN) {
            return -1;
        }

        cmd->src_ip.a = nums[0];
        cmd->src_ip.b = nums[1];
        cmd->src_ip.c = nums[2];
        cmd->src_ip.d = nums[INDEX3];

        if (port[0] == '*') {
            cmd->src_port = MATCH_PORT_ANY;
        } else if (sscanf(port, "%u", &cmd->src_port) == 1) {
            if (cmd->src_port > PORT_MAX || cmd->src_port < PORT_MIN) {
                return -1;
            }
        } else {
            return -1;
        }

        //DST
        if (sscanf(buff[INDEX4], "%u.%u.%u.%u:%s", &nums[0], &nums[1], &nums[2],
                &nums[INDEX3], port) != IP_TOKENS) {
            return -1;
        }

        if (nums[0] > IP_OCTET_MAX || nums[0] < IP_OCTET_MIN
                || nums[1] > IP_OCTET_MAX || nums[1] < IP_OCTET_MIN
                || nums[2] > IP_OCTET_MAX || nums[2] < IP_OCTET_MIN
                || nums[INDEX3] > IP_OCTET_MAX || nums[INDEX3] < IP_OCTET_MIN) {
            return -1;
        }

        cmd->dst_ip.a = nums[0];
        cmd->dst_ip.b = nums[1];
        cmd->dst_ip.c = nums[2];
        cmd->dst_ip.d = nums[INDEX3];

        if (port[0] == '*') {
            cmd->dst_port = MATCH_PORT_ANY;
        } else if (sscanf(port, "%u", &cmd->dst_port) == 1) {
            if (cmd->dst_port > PORT_MAX || cmd->dst_port < PORT_MIN) {
                return -1;
            }
        } else {
            return -1;
        }

        return 0;
    }

    //For DELETE
    else if (strcmp(buff[0], "delete") == 0) {
        cmd->cmd = DELETE;

        //POS
        if (sscanf(buff[1], "%d", &cmd->pos) != 1) {
            //PRINT ERROR
            return -1;
        }

        return 0;
    }

    //For TEST
    else if (strcmp(buff[0], "test") == 0) {
        cmd->cmd = TEST;

        if (strcmp(buff[1], "tcp") == 0) {
            cmd->protocol = PROTO_TCP;
        } else if (strcmp(buff[1], "udp") == 0) {
            cmd->protocol = PROTO_UDP;
        } else {
            return -1;
        }

        //For SRC
        char port[PORT_SIZE];
        unsigned int nums[NUMS_SIZE];
        if (sscanf(buff[2], "%u.%u.%u.%u:%s", &nums[0], &nums[1], &nums[2],
                &nums[INDEX3], port) != IP_TOKENS) {
            return -1;
        }

        if (nums[0] > IP_OCTET_MAX || nums[0] < IP_OCTET_MIN
                || nums[1] > IP_OCTET_MAX || nums[1] < IP_OCTET_MIN
                || nums[2] > IP_OCTET_MAX || nums[2] < IP_OCTET_MIN
                || nums[INDEX3] > IP_OCTET_MAX || nums[INDEX3] < IP_OCTET_MIN) {
            return -1;
        }

        cmd->src_ip.a = nums[0];
        cmd->src_ip.b = nums[1];
        cmd->src_ip.c = nums[2];
        cmd->src_ip.d = nums[INDEX3];

        if (port[0] == '*') {
            cmd->src_port = MATCH_PORT_ANY;
        } else if (sscanf(port, "%u", &cmd->src_port) != 1) {
            return -1;
        }

        if (cmd->src_port > PORT_MAX || cmd->src_port < PORT_MIN) {
            return -1;
        }

        //For DST
        if (sscanf(buff[INDEX3], "%u.%u.%u.%u:%s", &nums[0], &nums[1], &nums[2],
                &nums[INDEX3], port) != IP_TOKENS) {
            return -1;
        }

        if (nums[0] > IP_OCTET_MAX || nums[0] < IP_OCTET_MIN
                || nums[1] > IP_OCTET_MAX || nums[1] < IP_OCTET_MIN
                || nums[2] > IP_OCTET_MAX || nums[2] < IP_OCTET_MIN
                || nums[INDEX3] > IP_OCTET_MAX || nums[INDEX3] < IP_OCTET_MIN) {
            return -1;
        }

        cmd->dst_ip.a = nums[0];
        cmd->dst_ip.b = nums[1];
        cmd->dst_ip.c = nums[2];
        cmd->dst_ip.d = nums[INDEX3];

        if (port[0] == '*') {
            cmd->dst_port = MATCH_PORT_ANY;
        } else if (sscanf(port, "%u", &cmd->dst_port) != 1) {
            return -1;
        }

        if (cmd->dst_port > PORT_MAX || cmd->dst_port < PORT_MIN) {
            return -1;
        }

        return 0;
    }

    //For PRINT
    else if (strcmp(buff[0], "print") == 0) {
        cmd->cmd = PRINT;

        if (strcmp(buff[1], "all") == 0) {
            cmd->pos = -1;
        } else if (sscanf(buff[1], "%d", &cmd->pos) != 1) {
            //PRINT ERROR
            return -1;
        }

        return 0;
    }

    //For HELP
    else if (strcmp(buff[0], "help") == 0) {
        cmd->cmd = HELP;

        return 0;
    }

    //For QUIT
    else if (strcmp(buff[0], "quit") == 0) {
        cmd->cmd = QUIT;

        return 0;
    }

    return 0;
}
