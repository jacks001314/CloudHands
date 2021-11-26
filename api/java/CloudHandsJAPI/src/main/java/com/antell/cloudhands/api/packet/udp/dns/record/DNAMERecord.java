package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.Name;

/**
 * DNAME Record  - maps a nonterminal alias (subtree) to a different domain
 *
 */

public class DNAMERecord extends SingleNameBase {


    public DNAMERecord() {
    }

    public Record getObject() {
        return new DNAMERecord();
    }

    /**
     * Gets the target of the DNAME Record
     */
    public Name getTarget() {
        return getSingleName();
    }

    /**
     * Gets the alias specified by the DNAME Record
     */
    public Name getAlias() {
        return getSingleName();
    }

}
