package com.antell.cloudhands.api.utils;

import com.google.common.base.Preconditions;


public class ByteDataUtils {

    static final char[] digits = new char[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    private ByteDataUtils() {
    }

    public static boolean equals(byte[] a1, int a1Offset, byte[] a2, int a2Offset, int length) {
        if (a1.length >= a1Offset + length && a2.length >= a2Offset + length) {
            for(int l = 0; l < length; ++l) {
                if (a1[a1Offset + l] != a2[a2Offset + l]) {
                    return false;
                }
            }

            return true;
        } else {
            return false;
        }
    }

    public static boolean equals(byte[] a,byte[] b){

        return equals(a,0,b,0,b.length);
    }


    public static String printHex(byte[] array) {
        return printHex(array, 0, array.length);
    }

    public static String printHex(byte[] array, int offset, int len) {
        StringBuilder sb = new StringBuilder();

        for(int i = 0; i < len; ++i) {
            byte b = array[offset + i];
            if (sb.length() > 0) {
                sb.append(' ');
            }

            sb.append(digits[b >> 4 & 15]);
            sb.append(digits[b & 15]);
        }

        return sb.toString();
    }



    public static String toHex(long value,int n,boolean isLittle){

        byte[] data = new byte[n];

        if(isLittle){
            for(int i=0;i<n;i++){
                data[i] = (byte)(value>>(i*8));
            }

        }else {

            for(int i=0;i<n;i++){
                data[i] = (byte)(value>>((n-i-1)*8));
            }
        }

        return toHex(data);
    }

    public static String toHex(String data){
        return toHex(data.getBytes());
    }

    public static String toHex(byte[] array) {
        return toHex(array, 0, array.length);
    }

    public static String toHex(byte[] array, int offset, int len) {
        StringBuilder sb = new StringBuilder();

        for(int i = 0; i < len; ++i) {
            byte b = array[offset + i];
            sb.append(digits[b >> 4 & 15]);
            sb.append(digits[b & 15]);
        }

        return sb.toString();
    }

    public static byte[] parseHex(String hex) {
        if (hex == null) {
            throw new IllegalArgumentException("Hex string is null");
        } else if (hex.length() % 2 != 0) {
            throw new IllegalArgumentException("Hex string '" + hex + "' should have even length.");
        } else {
            byte[] result = new byte[hex.length() / 2];

            for(int i = 0; i < result.length; ++i) {
                int hi = parseHexDigit(hex.charAt(i * 2)) << 4;
                int lo = parseHexDigit(hex.charAt(i * 2 + 1));
                result[i] = (byte)(hi + lo);
            }

            return result;
        }
    }

    public static byte[] copyof(byte[] src,int pos,int end){

        int len = end-pos;
        byte[] data = new byte[len];

        System.arraycopy(src,pos,data,0,len);
        return data;
    }

    public static byte[] parseHex(String hex,int n){

        StringBuffer sb = new StringBuffer();
        for(int i=0;i<n;i++){

            sb.append(hex);
        }

        return parseHex(sb.toString());
    }

    private static int parseHexDigit(char c) {
        if (c >= '0' && c <= '9') {
            return c - 48;
        } else if (c >= 'a' && c <= 'f') {
            return c - 97 + 10;
        } else if (c >= 'A' && c <= 'F') {
            return c - 65 + 10;
        } else {
            throw new IllegalArgumentException("Digit '" + c + "' out of bounds [0-9a-fA-F]");
        }
    }

    public static int indexOf(byte[] array, byte[] target) {
        Preconditions.checkNotNull(array, "array");
        Preconditions.checkNotNull(target, "target");
        if (target.length == 0) {
            return 0;
        } else {
            label28:
            for(int i = 0; i < array.length - target.length + 1; ++i) {
                for(int j = 0; j < target.length; ++j) {
                    if (array[i + j] != target[j]) {
                        continue label28;
                    }
                }

                return i;
            }

            return -1;
        }
    }

    public static  boolean contains(byte[] array,byte[] target){

        return indexOf(array,target)!=-1;
    }

    public static final byte[] getBytes (char[] chars) {

        byte[] res = new byte[chars.length];

        for(int i=0;i<chars.length;i++){

            res[i]= (byte)chars[i];
        }

        return res;
    }




    public static char[] getChars(byte[] bytes) {

        char[] res = new char[bytes.length];

        for(int i=0;i<bytes.length;i++){

            res[i] = (char)bytes[i];
        }

        return res;
    }


}
