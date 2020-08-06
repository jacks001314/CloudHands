package com.antell.cloudhands.api.utils;

import net.lingala.zip4j.ZipFile;
import net.lingala.zip4j.exception.ZipException;

import java.io.File;
import java.util.List;


public class ZipFileUtils {

    public static void unzip(String zipFile,String dstDir) throws ZipException {

        ZipFile zf = new ZipFile(zipFile);
        zf.extractAll(dstDir);
    }

    public static void pzipDir(String zipFile, List<String> dirs) throws ZipException {

        ZipFile zf = new ZipFile(zipFile);

        for(String dir:dirs)
            zf.addFolder(new File(dir));

    }

}
