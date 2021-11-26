package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.Name;

/**
 * Pointer Record  - maps a domain name representing an Internet Address to
 * a hostname.
 *
 */

public class PTRRecord extends SingleCompressedNameBase {


    public PTRRecord() {
    }

    @Override
    public Record getObject() {
        return new PTRRecord();
    }

    /**
     * Gets the target of the PTR Record
     */
    public Name getTarget() {
        return getSingleName();
    }

}
