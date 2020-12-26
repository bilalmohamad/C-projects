/**
 * @file policy.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component is responsible for functionality pertaining to the policy and firewall rules.
 * It contains features used by the top-level component, fwsim.c, but it should never make
 * calls to code in fwsim.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include "policy.h"

/** Array of pointers to dynamically allocated firewall rules. */
static rule_t **policy;

/** Size of the array of pointers */
static unsigned int policy_len;

/** Capacity of the array of pointers */
static unsigned int policy_cap;

/** Default policy */
static unsigned int policy_default;

/**
 * This function will initialize the dynamically allocated policy structure.
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_init() {

    policy_len = 0;
    policy_cap = POLICY_INIT_SIZE;
    policy_default = ACTION_DENY;
    policy = (rule_t **) malloc(policy_cap * sizeof(rule_t*));

    if (policy) {

        for (int i = 0; i < policy_cap; i++) {
            policy[i] = NULL;
        }

        return 0;
    }

    return -1;
}

/**
 * This function will free the dynamically allocated policy structure and re-initialize
 * values as appropriate.
 */
void policy_free() {

    for (int i = 0; i < policy_len; i++) {
        free(policy[i]);
    }
    free(policy);
}

/**
 * This function will set the default policy to the specified action.
 * The starter files includes #define's for ACTION_ALLOW and ACTION_DENY.
 *
 * @param action the action for the default policy to be set to
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_set_default(int action) {

    if (action == ACTION_DENY || action == ACTION_ALLOW) {
        policy_default = action;
        return 0;
    }

    return -1;
}

/**
 * This function will append a rule to the policy.
 *
 * @param rule the rule to be appended
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_append(rule_t rule) {

    //TODO Need to check if meets capacity and reallocate
    if (policy_len >= policy_cap) {
        policy_cap *= 2;
        policy = (rule_t **) realloc(policy, policy_cap * sizeof(rule_t *));

        if (policy) {
            for (int i = policy_cap / 2; i < policy_cap; i++) {
                policy[i] = NULL;
            }
        } else {
            return -1;
        }
    }

    rule_t *temp = (rule_t *) malloc(sizeof(rule_t));
    temp->action = rule.action;
    temp->match.protocol = rule.match.protocol;
    temp->match.src_ip = rule.match.src_ip;
    temp->match.src_port = rule.match.src_port;
    temp->match.dst_ip = rule.match.dst_ip;
    temp->match.dst_port = rule.match.dst_port;

    policy[policy_len] = temp;
    policy_len++;

    return 0;
}

/**
 * This function will insert a rule at position.
 *
 * @param rule the rule to be inserted
 * @param pos the position to be inserted at
 *
 * @return 0 if successful, -1 if unsuccessful
 */
int policy_insert(rule_t rule, int pos) {

    if (policy_len >= policy_cap) {
        policy_cap *= 2;
        policy = (rule_t **) realloc(policy, policy_cap * sizeof(rule_t *));

        if (policy) {
            for (int i = policy_cap / 2; i < policy_cap; i++) {
                policy[i] = NULL;
            }
        } else {
            return -1;
        }
    }

    if (pos > policy_len) {
        policy_append(rule);
    } else {
        //Shifts all the entries in the array
        for (int i = policy_len; i > pos - 1; i--) {
            policy[i] = policy[i - 1];
        }

        rule_t *temp = (rule_t *) malloc(sizeof(rule_t));
        temp->action = rule.action;
        temp->match.protocol = rule.match.protocol;
        temp->match.src_ip = rule.match.src_ip;
        temp->match.src_port = rule.match.src_port;
        temp->match.dst_ip = rule.match.dst_ip;
        temp->match.dst_port = rule.match.dst_port;

        policy[pos - 1] = temp;
        policy_len++;
    }

    return 0;
}

/**
 * This function will delete the rule at position.
 *
 * @param pos the position of the rule
 *
 * @return It returns 0 if successful, -1 if unsuccessful.
 */
int policy_delete(int pos) {

    if (pos - 1 < 0 || pos - 1 >= policy_len) {
        return -1;
    }

    if (policy[pos - 1] == NULL) {
        return -1;
    }

    rule_t *temp = policy[pos - 1];
    free(temp);

    //Shift everything
    for (int i = pos - 1; i < policy_len; i++) {
        policy[i] = policy[i + 1];
    }

    policy[policy_len - 1] = NULL;
    policy_len--;

    return 0;
}

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
int policy_test(packet_t pkt, int *pos) {

    for (int i = 0; i < policy_cap; i++) {
        if (policy[i] && packet_match(policy[i]->match, pkt) == 1) {
            *pos = i;
            return policy[i]->action;
        }
    }

    //No rule is matched
    *pos = -1;
    return policy_default;
}

/**
 * This function will print to @stream the rule at position @pos.
 *
 * @param stream the file stream to print to
 * @param pos the rule at the position to be printed
 *
 * @return It returns 0 if successful and -1 if unsuccessful (e.g., @pos does not exist).
 */
int policy_print_rule(FILE *stream, int pos) {

    if (pos - 1 < 0 || pos - 1 >= policy_len) {
        return -1;
    }

    if (!policy[pos - 1]) {
        return -1;
    }

    fprintf(stream, "[%d] ", pos);

    if (policy[pos - 1]->action == ACTION_DENY) {
        fprintf(stream, "deny ");
    } else {
        fprintf(stream, "allow ");
    }

    if (policy[pos - 1]->match.protocol == PROTO_UDP) {
        fprintf(stream, "udp ");
    } else {
        fprintf(stream, "tcp ");
    }

    fprintf(stream, "%d.", policy[pos - 1]->match.src_ip.a);
    fprintf(stream, "%d.", policy[pos - 1]->match.src_ip.b);
    fprintf(stream, "%d.", policy[pos - 1]->match.src_ip.c);
    fprintf(stream, "%d:", policy[pos - 1]->match.src_ip.d);

    if (policy[pos - 1]->match.src_port == MATCH_PORT_ANY) {
        fprintf(stream, "* ");
    } else {
        fprintf(stream, "%d ", policy[pos - 1]->match.src_port);
    }

    fprintf(stream, "%d.", policy[pos - 1]->match.dst_ip.a);
    fprintf(stream, "%d.", policy[pos - 1]->match.dst_ip.b);
    fprintf(stream, "%d.", policy[pos - 1]->match.dst_ip.c);
    fprintf(stream, "%d:", policy[pos - 1]->match.dst_ip.d);

    if (policy[pos - 1]->match.dst_port == MATCH_PORT_ANY) {
        fprintf(stream, "*\n");
    } else {
        fprintf(stream, "%d \n", policy[pos - 1]->match.dst_port);
    }

    return 0;
}

/**
 * This function will print the default policy followed by the policy rules in order to @stream.
 *
 * @param stream the file stream to print to
 */
void policy_print(FILE *stream) {

    fprintf(stream, "default ");

    if (policy_default == ACTION_DENY) {
        fprintf(stream, "deny\n");
    } else {
        fprintf(stream, "allow\n");
    }

    int pos = 1;
    for (int i = 0; i < policy_cap; i++) {

        if (policy[i]) {
            fprintf(stream, "[%d] ", pos);

            if (policy[i]->action == ACTION_DENY) {
                fprintf(stream, "deny ");
            } else {
                fprintf(stream, "allow ");
            }

            if (policy[i]->match.protocol == PROTO_UDP) {
                fprintf(stream, "udp ");
            } else {
                fprintf(stream, "tcp ");
            }

            fprintf(stream, "%d.", policy[i]->match.src_ip.a);
            fprintf(stream, "%d.", policy[i]->match.src_ip.b);
            fprintf(stream, "%d.", policy[i]->match.src_ip.c);
            fprintf(stream, "%d:", policy[i]->match.src_ip.d);

            if (policy[i]->match.src_port == MATCH_PORT_ANY) {
                fprintf(stream, "* ");
            } else {
                fprintf(stream, "%d ", policy[i]->match.src_port);
            }

            fprintf(stream, "%d.", policy[i]->match.dst_ip.a);
            fprintf(stream, "%d.", policy[i]->match.dst_ip.b);
            fprintf(stream, "%d.", policy[i]->match.dst_ip.c);
            fprintf(stream, "%d:", policy[i]->match.dst_ip.d);

            if (policy[i]->match.dst_port == MATCH_PORT_ANY) {
                fprintf(stream, "*\n");
            } else {
                fprintf(stream, "%d \n", policy[i]->match.dst_port);
            }

            pos++;
        }
    }
}
