#!/bin/bash
install_bins="apt-get yum"
ins=""

for insb in $install_bins ; do
    which $insb
    if [ $? -eq 0 ]; then
        ins=$insb
        break
    fi   
done

if [ $ins == "" ] ; then
    echo "cannot found install tools"
    exit -1
fi

deps="wget automake cmake libtool meson "

$ins -y install $deps

install_cmake_src(){

    url=$1
    src=$2
    zfile=$3

    wget $url
    if [ ! -f $zfile ]; then 
        echo "download zip file failed from $url!"
        exit -1
    fi 
    unzip $zfile

    if [ ! -d $src ]; then 
        echo "unzip zip file failed from $zfile "
        exit -1
    fi
    cd $src 
    cmake .

    if [ ! -f Makefile ]; then 
        echo 'cannot generate makefile !'
        exit -1
    fi 

    make 
    make install

    rm -rf $src $zfile
    cd ../
}

googletestUrl=https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
googletestzip=v1.13.0.zip
googletest=googletest-1.13.0

install_cmake_src $googletestUrl $googletest $googletestzip

msgpackUrl=https://github.com/msgpack/msgpack-c/archive/refs/heads/c_master.zip
msgpackZip=c_master.zip
msgpack=msgpack-c-c_master

install_cmake_src $msgpackUrl $msgpack $msgpackZip

cjsonUrl=https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.15.zip
cjsonZip=v1.7.15.zip
cjson=cJSON-1.7.15

install_cmake_src $cjsonUrl $cjson $cjsonZip