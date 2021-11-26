package com.antell.cloudhands.api.packet.udp.dns;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSQuestion;
import com.antell.cloudhands.api.packet.udp.dns.raw.DNSResponse;
import com.antell.cloudhands.api.packet.udp.dns.raw.RDNSRecord;
import com.antell.cloudhands.api.packet.udp.dns.record.Type;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.stream.Collectors;

public class DNSUtils {

    public static List<String> getTypes(DNSRequst requst, DNSResponse response){

        Set<String>  sets = new TreeSet<>();

        if(requst!=null&&requst.getQuestions()!=null){

            requst.getQuestions().forEach(q->{

                sets.add(Type.string(q.getType()));
            });

        }

        if(response!=null){

            List<DNSQuestion> questions = response.getQuestions();
            if(questions!=null){
                questions.forEach(q->sets.add(Type.string(q.getType())));
            }

            for(int i = 1;i<4;i++){

                List<RDNSRecord> records = response.getRecords(i);
                if(records!=null){

                    records.forEach(r->sets.add(Type.string(r.getType())));
                }
            }
        }

        return sets.size() ==0?new ArrayList<>():sets.stream().collect(Collectors.toList());
    }
}
