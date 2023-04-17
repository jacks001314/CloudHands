#!/bin/bash

self_path=$(cd $(dirname "$0"); pwd)
pack_dir=pack_dir
pack_pkg_dir=$pack_dir/package
pack_conf_dir=$pack_dir/package/conf
pack_bin_dir=$pack_dir/package/bin

usage="bash pack.sh <install_dir> <data_dir> <log_dir> <rules_dir> <dpdk_ver>"

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
cloudhands_bin_name=CloudHands_bin_$cloudhands_ver.bin

prepare_pack(){

    if [ -f $cloudhands_bin_name ]
    then
        rm -rf $cloudhands_bin_name
    fi


    if [ ! -d $pack_dir ]; then
        mkdir $pack_dir
    else
        rm -rf $pack_dir/*
    fi

    mkdir -p $pack_pkg_dir    
}

make_config(){
    
    rm -rf config.sh

    cp config.sh.tpl config.sh

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

    cp -fr config.sh $pack_pkg_dir
    chmod a+x $pack_pkg_dir/config.sh

    rm -rf config.sh
}

generate_conf_files(){

    cp -r ../conf .

    cd conf

    bash make_conf.sh $install_dir $data_dir $log_dir $rules_dir

    cd ../
    cp -fr conf $pack_conf_dir
    rm -rf conf 
}

check_file(){
	fpath=$1

	if [ ! -f $fpath ]; then 
		echo "file $fpath not existed,will exit++++++++++++++++++++++++++"
		exit -1
	fi 

}

compile_cloudhands(){

    mkdir CloudHands

    for src in $cloudhands_src; do
        cp -fr ../$src CloudHands
    done

	bdir=cbuild
	bins="\
		$bdir/PDispatcher/PDispatcher \
		$bdir/pdump/PcapMain \
		$bdir/pdump/PDumpMain \
		$bdir/psink/PSinkMain \
		$bdir/statistic/StatDump \
        $bdir/lib/shm/SHMInfoDump \
		$bdir/StreamAnalyze/SAMain \
		$bdir/tcp/main/TCPMain \
		$bdir/udp/main/UDPMain \

		"

	cd CloudHands

	meson cbuild

	ninja -C cbuild

	for b in $bins; do 
		check_file $b
		cp -fr $b ../$pack_bin_dir
	done

	chmod a+x ../$pack_bin_dir/bin/*

    cd ../
    rm -rf CloudHands
}

copy_build_shells(){

    cp -fr install_bin.sh $pack_dir
    cp -fr makeself $pack_dir
    cp -fr install_bin.sh $pack_pkg_dir/

    chmod a+x $pack_dir/*.sh
    chmod a+x $pack_pkg_dir/*.sh
    chmod a+x $pack_dir/makeself/*.sh
}

do_pack(){

    cd $pack_dir

    bash ./makeself/makeself.sh --gzip package $cloudhands_bin_name 'install cloudhands ....' ./install_bin.sh

    if [ ! -f $cloudhands_bin_name ]; then 
        echo "pack cloudhands package:$cloudhands_bin_name failed!"
        exit -1
    fi 

    mv $cloudhands_bin_name ../

    echo "pack cloudhands package:$cloudhands_bin_name ok!"

    

    cd ../

    rm -rf $pack_dir
}

echo "prepare pack++++++++++++++++++++++++++++++++"
prepare_pack


echo "generate config files +++++++++++++++++++++++++++"
generate_conf_files

cp -fr ../bin $pack_bin_dir


echo "make config.sh file +++++++++++++++++++++++++++++++"
make_config

echo "compile cloudhands ++++++++++++++++++++++++++++++++++++"
compile_cloudhands

echo "copy build script shells +++++++++++++++++++++++++++++++"
copy_build_shells

echo "start to pack++++++++++++++++++++++++++++++++++++++++"
do_pack





