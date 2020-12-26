/**
 * @file policy.h
 * @author Bilal Mohamad (bmohama)
 *
 * This file acts as the interface for the policy.c file
 */

#ifndef POLICY_H
#define POLICY_H

#include <stdio.h>
#include <stdlib.h>
#include "packet.h"

/** Used to indicate an allow rule. */
#define ACTION_ALLOW   0

/** Used to indicate a deny rule. */
#define ACTION_DENY    1

/** Initial policy capacity */
#define POLICY_INIT_SIZE 10

/**
 * Representation of a firewall rule
 * .action: the rule action (ACTION_ALLOW or ACTION_DENY)
 * .match: the packet match
 */
typedef struct rule {
    unsigned int action;
    packet_match_t match;
} rule_t;

/**
 * This function will initialize the dynamically allocated policy structure.
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_init();

/**
 * This function will free the dynamically allocated policy structure and re-initialize
 * values as appropriate.
 */
void policy_free();

/**
 * This function will set the default policy to the specified action. The starter files
 * includes #define's for ACTION_ALLOW and ACTION_DENY.
 *
 * @param action the action for the default policy to be set to
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_set_default(int action);

/**
 * This function will append a rule to the policy.
 *
 * @param rule the rule to be appended
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_append(rule_t rule);

/**
 * This function will insert a rule at position.
 *
 * @param rule the rule to be inserted
 * @param pos the position to be inserted at
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_insert(rule_t rule, int pos);

/**
 * This function will delete the rule at position.
 *
 * @param pos the position of the rule
 *
 * @return It returns 0 if successful, -1 if unsuccessful.
 */
int policy_delete(int pos);

/**
 * This function will test if @pkt is allowed or denied by the policy.
 * It returns ACTION_ALLOW or ACTION_DENY.
 * Additionally the value pointed to by @pos will be updated with the position
 * number of the rule that is matched.
 * If no rule is matched, the value will be set to -1.
 *
 * @param pkt the packet being tested
 * @param pos the position containing the value to be updated position of the rule that is matches
 *
 * @return It returns 0 if successful, -1 if unsuccessful.
 */
int policy_test(packet_t pkt, int *pos);

/**
 * This function will print to @stream the rule at position @pos.
 *
 * @param stream the file stream to print to
 * @param pos the rule at the position to be printed
 *
 * @return It returns 0 if successful and -1 if unsuccessful (e.g., @pos does not exist).
 */
int policy_print_rule(FILE *stream, int pos);

/**
 * This function will print the default policy followed by the policy rules in order to @stream.
 *
 * @param stream the file stream to print to
 */
void policy_print(FILE *stream);

#endif
