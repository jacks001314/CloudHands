package com.antell.cloudhands.api.utils;

import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.List;
import java.util.stream.Collectors;

public class FileUtils {

    private static final String getDirs(String fpath,String prefix){

        StringBuffer sb = new StringBuffer(prefix);
        sb.append("/");
        for(int i=0;i<fpath.length();i++){

            if(Character.isDigit(fpath.charAt(i))){
                sb.append(fpath.substring(i));
                return sb.toString();
            }
        }

        sb.append(fpath);
        return sb.toString();
    }

    public static final String fileCopy(String fpath,String dstFDirPrefix){

        String dstPath = getDirs(fpath,dstFDirPrefix);
        Path path = Paths.get(dstPath);
        if(!Files.exists(path)){

            try {
                Files.createDirectories(path.getParent());
               // Files.createFile(path);
            } catch (IOException e) {
                e.printStackTrace();
                return fpath;
            }
        }

        try {
            Files.copy(Paths.get(fpath),path);
        } catch (IOException e) {
            e.printStackTrace();
            return fpath;
        }

        return dstPath;
    }

    public static final String copy(String fpath,String dstPath){

        Path path = Paths.get(dstPath);
        if(!Files.exists(path)){

            try {
                Files.createDirectories(path.getParent());
                // Files.createFile(path);
            } catch (IOException e) {
                e.printStackTrace();
                return fpath;
            }
        }

        try {
            Files.copy(Paths.get(fpath),path);
        } catch (IOException e) {
            e.printStackTrace();
            return fpath;
        }

        return dstPath;
    }

    public static final List<String> dirs(String rootDir) throws IOException {

        return Files.list(Paths.get(rootDir)).map(path->path.getFileName().toString())
                .filter(name->!name.startsWith("."))
                .collect(Collectors.toList());
    }

    public static final boolean isExisted(String fpath){

        if(TextUtils.isEmpty(fpath))
            return false;

        return Files.exists(Paths.get(fpath));
    }

    public static final boolean hasContent(String fpath){

        Path path = Paths.get(fpath);
        try {
            return Files.exists(path)&&Files.size(path)>0;
        } catch (IOException e) {

            return false;
        }
    }
    public static final void delDirs(String dir) throws IOException {

        Path dpath = Paths.get(dir);

        if(!Files.exists(dpath))
            return;

        Files.walkFileTree(dpath,new SimpleFileVisitor<>(){

            @Override
            public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                Files.delete(file);
                return FileVisitResult.CONTINUE;
            }

            @Override
            public FileVisitResult postVisitDirectory(Path dir, IOException e) throws IOException {
                if (e == null) {
                    Files.delete(dir);
                    return FileVisitResult.CONTINUE;
                } else {
                    throw e;
                }
            }
        });

    }


}
