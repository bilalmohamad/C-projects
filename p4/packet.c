/**
 * @file packet.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component is responsible for functionality pertaining to representing and matching packets.
 * It contains features used by the top-level component,  fwsim.c, policy.c, and command.c but
 * it should never make calls to those components.
 */

#include "packet.h"

/**
 * This function checks if @packet is matched by @match.
 *
 * @return 1 if match and 0 if no match.
 */
int packet_match(packet_match_t match, packet_t packet) {

    if (match.protocol != packet.protocol) {
        return 0;
    }

    if (match.src_ip.a != packet.src_ip.a || match.src_ip.b != packet.src_ip.b
            || match.src_ip.c != packet.src_ip.c
            || match.src_ip.d != packet.src_ip.d) {
        return 0;
    }

    if (match.src_port != MATCH_PORT_ANY
            && match.src_port
                    != packet.src_port && packet.src_port != MATCH_PORT_ANY) {
        return 0;
    }

    if (match.dst_ip.a != packet.dst_ip.a || match.dst_ip.b != packet.dst_ip.b
            || match.dst_ip.c != packet.dst_ip.c
            || match.dst_ip.d != packet.dst_ip.d) {
        return 0;
    }

    if (match.dst_port != MATCH_PORT_ANY
            && match.dst_port
                    != packet.dst_port && packet.dst_port != MATCH_PORT_ANY) {
        return 0;
    }

    return 1;
}
