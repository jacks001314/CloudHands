package com.antell.cloudhands.api.packet.udp.dns.common;

import com.antell.cloudhands.api.packet.udp.dns.common.Name;

/**
 * An exception thrown when a relative name is passed as an argument to
 * a method requiring an absolute name.
 *
 */

public class RelativeNameException extends IllegalArgumentException {

    public RelativeNameException(Name name) {
        super("'" + name + "' is not an absolute name");
    }

    public RelativeNameException(String s) {
        super(s);
    }

}
