package com.antell.cloudhands.api.utils;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import org.apache.commons.io.FileUtils;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * @author assenge
 * @date 2020/4/17 18:00
 */

/**
 * 最新写入的 cold data 文件信息，包含前写入文件和当前写入行数
 */
class LatestInfo{
    String date = "";
    int fileNamePre = 1;
    int line = 0;

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public int getFileNamePre() {
        return fileNamePre;
    }

    public void setFileNamePre(int fileNamePre) {
        this.fileNamePre = fileNamePre;
    }

    public int getLine() {
        return line;
    }

    public void setLine(int line) {
        this.line = line;
    }
}

/**
 * 冷数据
 */
public class ColdDataUtils {

    private static final long ERROR = -1;
    private static final long MAXLINE = 10000;
    private final static SimpleDateFormat dft = new SimpleDateFormat("yyyyMMdd");
    public static String coldDataPath;
    private final static String infoFile = ".lastInfo.json";

    /**
     * 读取 .lastInfo.json
     * @return
     */
    private static LatestInfo readLatestInfo() {
        String info = null;
        File file = new File(new StringBuffer(coldDataPath).append(infoFile).toString());
        if (!file.exists()) {
            return new LatestInfo();
        }

        try {
            info = FileUtils.readFileToString(file, "UTF-8");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return JSON.parseObject(info, LatestInfo.class);
    }

    /**
     * 更新 .lastInfo.json
     * @param latestInfo
     */
    private static void writeLatestInfo(LatestInfo latestInfo) {
        String info = JSON.toJSONString(latestInfo);
        try {
            FileUtils.writeStringToFile(new File(new StringBuffer(coldDataPath).append(infoFile).toString()), info,"UTF-8");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 读取 cold data
     * @param pathLine
     * @return
     */
    public static JSON readColdData(String pathLine) {
        int curLine = 0;
        String str = "";
        int index = pathLine.indexOf("#");
        String path = pathLine.substring(0, index);
        int line = Integer.parseInt(pathLine.substring(index + 1)) -1;

        try (FileInputStream fileInputStream = new FileInputStream(path);
             InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream, "UTF-8");) {
            BufferedReader bufferedReader = new LineNumberReader(inputStreamReader);

            while ((str = bufferedReader.readLine()) != null) {
                if (curLine++ == line) {
                    if (str.startsWith("{")) {
                        return JSON.parseObject(str);
                    } else if (str.startsWith("[")) {
                        return JSON.parseArray(str);
                    } else {
                        break;
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    /**
     * 写入 cold data
     * @param data
     * @return
     */
    public static String writeColdData(String data) {
        String pathAndLine = null;
        String dateStr = dft.format(new Date());
        LatestInfo latestInfo = readLatestInfo();
        String datePath = new StringBuffer(coldDataPath)
                .append(dateStr)
                .toString();

        // 创建日期目录并重置LatestInfo
        File dateFile = new File(datePath);
        if (!dateFile.exists()) {
            dateFile.mkdirs();
            latestInfo = new LatestInfo();
        }

        // 更新 fileNamePre 和 line
        if (latestInfo.line > MAXLINE - 1) {
            latestInfo.setFileNamePre(latestInfo.getFileNamePre() + 1);
            latestInfo.setLine(0);
        }

        String path = new StringBuffer(datePath)
                .append(File.separator)
                .append(latestInfo.getFileNamePre())
                .append(".json")
                .toString();

        try (FileOutputStream fileOutputStream = new FileOutputStream(path, true);
             OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream, "UTF-8")) {
            BufferedWriter bufferedWriter = new BufferedWriter(outputStreamWriter);
            if (latestInfo.getLine() != 0) {
                bufferedWriter.newLine();
            }
            bufferedWriter.write(data);
            bufferedWriter.flush();

            latestInfo.setLine(latestInfo.getLine() + 1);
            writeLatestInfo(latestInfo);
            pathAndLine = new StringBuffer(path).append("#").append(getLineNumber(path)).toString();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return pathAndLine;
    }

    /**
     * 获取行数
     * @param path
     * @return
     */
    private static long getLineNumber(String path) {
        try (FileInputStream fileInputStream = new FileInputStream(path);
             InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream, "UTF-8");) {
            final LineNumberReader lineNumberReader = new LineNumberReader(inputStreamReader);
            lineNumberReader.skip(Long.MAX_VALUE);
            return lineNumberReader.getLineNumber() + 1;
        } catch (IOException e) {
            e.printStackTrace();
        }

        return ERROR;
    }
}
