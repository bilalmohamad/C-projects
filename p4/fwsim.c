/**
 * @file fwsim.c
 * @author Bilal Mohamad (bmohama)
 *
 * This is the top-level component.
 * It contains the main() function and code for working with the policy.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "packet.h"
#include "policy.h"
#include "command.h"

/** Command prompt shown to the user. */
#define PROMPT "> "

/** Used for checking the number of arguments */
#define ARG_CHECK 3

/** Print out a usage message. */
static void usage() {
    fprintf(stderr, "Usage: fwsim [-h] [-r <rule_file>]\n");
}

/** Print out an error message. */
static void parseError() {
    fprintf(stdout, "Error: Could not parse command.\n");
}

/** Print out an error message. */
static void addError() {
    fprintf(stdout, "Error: Could not add rule.\n");
}

/** Print out an error message. */
static void deleteError() {
    fprintf(stdout, "Error: Could not delete rule.\n");
}

/** Print out an error message. */
static void ruleError(int pos) {
    fprintf(stdout, "[%d] \n", pos);
    fprintf(stdout, "Error: Rule %d does not exist.\n", pos);
}

/** Function used for printing help */
void helpCommand() {

    printf("Firewall Command Language:\n");
    printf("\n");
    printf("default (allow|deny)\n");
    printf(
            "insert <pos> (allow|deny) (tcp|udp) <src_ip>:(*|<src_port>) <dst_ip>:"
                    "(*|<dst_port>)\n");
    printf("append (allow|deny) (tcp|udp) <src_ip>:(*|<src_port>) <dst_ip>:"
            "(*|<dst_port>)\n");
    printf("delete <pos>\n");
    printf("test (tcp|udp) <src_ip>:<src_port> <dst_ip>:<dst_port>\n");
    printf("print (all|<pos>)\n");
    printf("help\n");
    printf("quit\n");
}

/**
 * Load firewall rules from a file.
 *
 * @param filename name of a file from which to load the rules.
 */
static void load_rules(char *filename) {

    FILE *fp = fopen(filename, "r");

    if (!fp) {
        return;
    }

    while (1) {

        fw_cmd_t cmd;
        cmd.cmd = 0;
        parse_command(fp, &cmd);

        if (feof(fp)) {
            break;
        }

        if (cmd.cmd == 0) {
            continue;
        } else if (cmd.cmd == DEFAULT) {
            if (policy_set_default(cmd.action) == -1) {

            }
        } else if (cmd.cmd == APPEND) {
            rule_t rule;
            rule.action = cmd.action;
            rule.match.protocol = cmd.protocol;
            rule.match.src_ip = cmd.src_ip;
            rule.match.src_port = cmd.src_port;
            rule.match.dst_ip = cmd.dst_ip;
            rule.match.dst_port = cmd.dst_port;

            if (policy_append(rule) == -1) {
                //PRINT ERROR
            }

        } else {
            //PRINT ERROR
        }
    }

    fclose(fp);

    return;
}

/**
 * Starting point for the program.  Process command-line arguments then
 * read and execute user commands.
 *
 * @param argc number of command-line arguments.
 * @param argv list of command-line arguments.
 *
 * @return program exit status
 */
int main(int argc, char *argv[]) {

    if (argc == 2) {
        usage();
        return EXIT_SUCCESS;
    }

    policy_init();
    policy_set_default(ACTION_DENY);

    if (argc == ARG_CHECK && strcmp("-r", argv[1]) == 0) {
        load_rules(argv[2]);
    }

    while (!feof(stdin)) {

        printf(PROMPT);

        fw_cmd_t cmd = { };
        if (parse_command(stdin, &cmd) == -1) {
            parseError();
        } else if (cmd.cmd == DEFAULT) {
            if (policy_set_default(cmd.action) == -1) {
                //Print Error
            }
        } else if (cmd.cmd == INSERT) {
            rule_t rule;
            rule.action = cmd.action;
            rule.match.protocol = cmd.protocol;
            rule.match.src_ip = cmd.src_ip;
            rule.match.src_port = cmd.src_port;
            rule.match.dst_ip = cmd.dst_ip;
            rule.match.dst_port = cmd.dst_port;

            if (policy_insert(rule, cmd.pos) == -1) {
                addError();
            }
        } else if (cmd.cmd == APPEND) {
            rule_t rule;
            rule.action = cmd.action;
            rule.match.protocol = cmd.protocol;
            rule.match.src_ip = cmd.src_ip;
            rule.match.src_port = cmd.src_port;
            rule.match.dst_ip = cmd.dst_ip;
            rule.match.dst_port = cmd.dst_port;

            if (policy_append(rule) == -1) {
                addError();
            }

        } else if (cmd.cmd == DELETE) {
            if (policy_delete(cmd.pos) == -1) {
                deleteError();
            }

        } else if (cmd.cmd == TEST) {

            packet_t packet;
            packet.protocol = cmd.protocol;
            packet.src_ip = cmd.src_ip;
            packet.src_port = cmd.src_port;
            packet.dst_ip = cmd.dst_ip;
            packet.dst_port = cmd.dst_port;

            if (policy_test(packet, &cmd.pos) == ACTION_ALLOW) {
                if (cmd.pos == -1) {
                    printf("Allowed via default policy.\n");
                } else {
                    printf("Allowed via ");
                    policy_print_rule(stdout, cmd.pos + 1);
                }
            } else {
                if (cmd.pos == -1) {
                    printf("Denied via default policy.\n");
                } else {
                    printf("Denied via ");
                    policy_print_rule(stdout, cmd.pos + 1);
                }
            }
        } else if (cmd.cmd == PRINT) {

            if (cmd.pos == -1) {
                policy_print(stdout);
            } else if (policy_print_rule(stdout, cmd.pos) == -1) {
                ruleError(cmd.pos);
            }
        } else if (cmd.cmd == HELP) {
            helpCommand();
        } else if (cmd.cmd == QUIT) {
            break;
        } else {
            //Do nothing
        }

    }

    policy_free();

    return EXIT_SUCCESS;
}
