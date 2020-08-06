package com.antell.cloudhands.api.packet.udp.dns.record;

/**
 * Text - stores text strings
 *
 */

public class TXTRecord extends TXTBase {


    public TXTRecord() {
    }

    @Override
    public Record getObject() {
        return new TXTRecord();
    }


}
