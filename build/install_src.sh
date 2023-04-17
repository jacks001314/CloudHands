#!/bin/sh
#
#      Filename: install.sh
#
#        Author: csp001314@163.com
#   Description: ---
#        Create: 2016-11-03 20:09:47
# Last Modified: 2018-10-29 18:56:07
#

[ `id -u` -ne 0 ] && {
	echo "You must run this by root" >&2
	exit 1
}


self_path=$(cd $(dirname "$0"); pwd)

. $self_path/config.sh

make_dir(){
	
	if [ ! -d $1 ]; then
   		mkdir -p $1
	fi
}

prefare_install(){

	make_dir $install_dir
	make_dir $data_dir
	make_dir $log_dir
	make_dir $rules_dir

	cp -fr conf $install_dir
	cp -fr bin $install_dir

}

install_cmake_src(){
    
	src=$1
    zfile=$2

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

install_deps() {

	cd zip 

	install_cmake_src googletest-1.13.0 googletest-1.13.0.zip
	install_cmake_src msgpack-c-c_master msgpack-c-c_master.zip
	install_cmake_src cJSON-1.7.15 cJSON-1.7.15.zip

	cd ../
}

install_dpdk(){

    cd tar
    tar -xvf dpdk-${dpdk_ver}.tar.gz

    cd dpdk-${dpdk_ver}

	meson dpdk_build
	ninja -C dpdk_build
	ninja -C dpdk_build install

    cd ../../
}

check_file(){
	fpath=$1

	if [ ! -f $fpath ]; then 
		echo "file $fpath not existed,will exit++++++++++++++++++++++++++"
		exit -1
	fi 

}

install_cloudhands(){

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
		cp -fr $b $install_dir/bin
	done

	chmod a+x $install_dir/bin/*
	cd ../
}

install_all(){

	source /etc/profile

	echo "make prepare for install+++++++++++++++++++++++++++++++++"
	prefare_install

	echo "install deps+++++++++++++++++++++++++++++++++++++++++++++"
	install_deps

	echo "install dpdk++++++++++++++++++++++++++++++++++++++++"
	install_dpdk

	echo "install cloudhands+++++++++++++++++++++++++++++"

	install_cloudhands
}

install_all

