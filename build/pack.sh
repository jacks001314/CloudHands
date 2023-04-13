#!/bin/bash

self_path=$(cd $(dirname "$0"); pwd)
pack_dir=pack_dir
pack_tar_dir=$pack_dir/package/tar
pack_zip_dir=$pack_dir/package/zip
pack_src_dir=$pack_dir/package/CloudHands
pack_conf_dir=$pack_dir/package/conf
pack_bin_dir=$pack_dir/package/bin

usage="bash pack.sh <install_dir> <data_dir> <log_dir> <rules_dir> <dpdk_version>"

cloudhands_src=" \
    api \
    filter \
    lib \
    MProcess \
    packet \
    PDispatcher \
    pdump \
    psink \
    rule \
    statistic \
    StreamAnalyze \
    tcp \
    udp \
    meson.build
    VERSION
"

if [ $# -lt 5 ]; then

	echo $usage
	exit
fi

install_dir=$1
data_dir=$2
log_dir=$3
rules_dir=$4
dpdk_ver=$5
cloudhands_ver=`cat ../VERSION`
cloudhands_bin_name=CloudHands_$cloudhands_ver.bin

prepare_pack(){

    if [ -f $product_name ]
    then
        rm -rf $product_name
    fi


    if [ ! -d $pack_dir ]; then
        mkdir $pack_dir
    else
        rm -rf $pack_dir/*
    fi

    mkdir -p $pack_tar_dir
    mkdir -p $pack_zip_dir
    mkdir -p $pack_src_dir
    
}

make_config(){

    install_dir_rep=${install_dir//\//\\\/}
    data_dir_rep=${data_dir//\//\\\/}
    log_dir_rep=${log_dir//\//\\\/}
    rules_dir_rep=${rules_dir//\//\\\/}

    echo "make config.sh ++++++++++++++++++++++++++++++++++++++++"
    sed -i "s/{{.install_dir}}/$install_dir_rep/g" config.sh
    sed -i "s/{{.data_dir}}/$data_dir_rep/g" config.sh
    sed -i "s/{{.log_dir}}/$log_dir_rep/g" config.sh
    sed -i "s/{{.rules_dir}}/$rules_dir_rep/g" config.sh
    sed -i "s/{{.dpdk_ver}}/$dpdk_ver/g" config.sh
}

download_zip_file(){

    url=$1
    zip_name=$2
    zip_fpath=$pack_zip_dir/$zip_name

    wget $url -q -O $zip_fpath

    if [ ! -f $zip_fpath ]; then 
        echo "download zip file:$zip_name from url:$url failed!"
        exit -1
    fi 

    echo "download zip file:$zip_name from url:$url ok!"
}

download_and_patch_dpdk(){

    if [ ! -f $pack_tar_dir/dpdk-${dpdk_ver}.tar.gz ] ; then
	    cp -r ../DPDK .
	    cd DPDK
	    bash make_dpdk_package.sh $dpdk_ver
	    cd ../
	    cp DPDK/build/* $pack_tar_dir
	    rm -rf DPDK
    fi
}

generate_conf_files(){

    cp -r ../conf .

    cd conf

    bash make_conf.sh $install_dir $data_dir $log_dir $rules_dir

    cp -fr conf $pack_conf_dir

    rm -rf conf 

    cd ../
}

copy_cloudhands_src(){

    for src in $cloudhands_src; do 
        cp -fr $src $pack_src_dir
    done
}

copy_build_shells(){

    cp -fr *.sh $pack_dir
    cp -fr makeself $pack_dir

    chmod a+x $pack_dir/*.sh
    chmod a+x $pack_dir/makeself/*.sh

}

do_pack(){

    cd $pack_dir

    bash ./makeself/makeself.sh --gzip package $cloudhands_bin_name 'build cloudhands ....' ./install.sh

    if [ ! -f $cloudhands_bin_name ]; then 
        echo "pack cloudhands package:$cloudhands_bin_name failed!"
        exit -1
    fi 

    echo "pack cloudhands package:$cloudhands_bin_name ok!"
}

echo "prepare pack++++++++++++++++++++++++++++++++"
prepare_pack

echo "download some required zip files+++++++++++++++++++++++++++++++++"

download_zip_file https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip googletest-1.13.0.zip
download_zip_file https://github.com/msgpack/msgpack-c/archive/refs/heads/c_master.zip msgpack-c-c_master.zip
download_zip_file https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.15.zip cJSON-1.7.15.zip

echo "download dpdk_$dpdk_ver and patch it++++++++++++++++"
download_and_patch_dpdk


echo "generate config files +++++++++++++++++++++++++++"
generate_conf_files

cp -fr ../bin $pack_bin_dir

echo "copy cloudhands ++++++++++++++++++++++++++++++++++++"
copy_cloudhands_src

echo "make config.sh file +++++++++++++++++++++++++++++++"

make_config

echo "copy build script shells +++++++++++++++++++++++++++++++"
copy_build_shells

echo "start to pack++++++++++++++++++++++++++++++++++++++++"
do_pack





