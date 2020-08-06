package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.Name;

/**
 * Key Exchange - delegation of authority
 */

public class KXRecord extends U16NameBase {

    public KXRecord() {
    }

    @Override
    public Record getObject() {
        return new KXRecord();
    }

    /**
     * Returns the target of the KX record
     */
    public Name
    getTarget() {
        return getNameField();
    }

    /**
     * Returns the preference of this KX record
     */
    public int
    getPreference() {
        return getU16Field();
    }

    @Override
    public Name getAdditionalName() {
        return getNameField();
    }

}
