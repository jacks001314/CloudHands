package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.Name;

/**
 * Mailbox Rename Record  - specifies a rename of a mailbox.
 *
 */

public class MRRecord extends SingleNameBase {


    public MRRecord() {
    }

    @Override
    public Record getObject() {
        return new MRRecord();
    }

    /**
     * Gets the new name of the mailbox specified by the domain
     */
    public Name getNewName() {
        return getSingleName();
    }

}
