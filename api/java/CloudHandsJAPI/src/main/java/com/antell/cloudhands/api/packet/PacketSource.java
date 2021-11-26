package com.antell.cloudhands.api.packet;

import com.antell.cloudhands.api.context.Context;
import com.antell.cloudhands.api.source.AbstractSource;
import com.antell.cloudhands.api.source.Filter;
import com.antell.cloudhands.api.source.SourceException;
import com.antell.cloudhands.api.source.SourceReader;
import com.antell.cloudhands.api.utils.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;


/**
 * Created by dell on 2018/6/19.
 */
public class PacketSource extends AbstractSource {

    private static final Logger log = LoggerFactory.getLogger(PacketSource.class);

    private List<String> getRealMMapFiles(List<String> arr) throws Exception {

        List<String> res = new ArrayList<>();

        for(String file:arr){

            if(file.endsWith("*")){

                Path path = Paths.get(file);
                String name = path.getFileName().toString();
                String prefix = name.substring(0,name.length()-1);
                String p = path.getParent().toString();

                for(String fname:FileUtils.dirs(p)){

                    if(fname.startsWith(prefix)){

                        res.add(String.format("%s/%s",p,fname));
                    }
                }
            }else{

                res.add(file);
            }
        }

        return res;
    }

    private List<String> getRealMMapFiles(String[] arr) throws Exception {

        List<String> res = new ArrayList<>();

        for(String file:arr){

            if(file.endsWith("*")){

                Path path = Paths.get(file);
                String name = path.getFileName().toString();
                String prefix = name.substring(0,name.length()-1);
                String p = path.getParent().toString();

                for(String fname:FileUtils.dirs(p)){

                    if(fname.startsWith(prefix)){

                        res.add(String.format("%s/%s",p,fname));
                    }
                }
            }else{

                res.add(file);
            }
        }

        return res;
    }


    @Override
    public void init(Context context, Filter filter) throws SourceException {

        String mmaps = context.getString("com.antell.cloudhands.api.packet.source.mmapFiles",null);
        
		//Preconditions.checkArgument(mmap!=null,"must specify mmap file names to read!");

        String[] mmapFiles = mmaps.split(",");

        int i = 0;

        try {
            List<String> files = getRealMMapFiles(mmapFiles);
            for(String mmapFile :files) {


                SourceReader sourceReader = new PacketSourceReader(this, filter, i,mmapFile);
                i++;

                addSourceReader(sourceReader);

                log.info(String.format("Add MMapFile:%s to read packet ok!",mmapFile));
            }
        } catch (Exception e) {

            log.error(String.format("Add MMapFile:%s to read packet failed,error:%s!",mmapFiles,e.getMessage()));

            throw  new SourceException(e.getMessage());
        }


    }

    @Override
    public void init(List<String> mmapFiles, Filter filter) throws SourceException {

        int i = 0;

        try {
            List<String> files = getRealMMapFiles(mmapFiles);
            for(String mmapFile :files) {

                SourceReader sourceReader = new PacketSourceReader(this, filter, i,mmapFile);
                i++;

                addSourceReader(sourceReader);
                log.info(String.format("Add MMapFile:%s to read packet ok!",mmapFile));
            }
        } catch (Exception e) {
            log.error(String.format("Add MMapFile:%s to read packet failed,error:%s!",mmapFiles,e.getMessage()));
            throw  new SourceException(e.getMessage());
        }

    }


}
