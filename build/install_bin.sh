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

do_install(){

	make_dir $install_dir
	make_dir $data_dir
	make_dir $log_dir
	make_dir $rules_dir

	cp -fr conf $install_dir
	cp -fr bin $install_dir

}


install_all(){

	source /etc/profile
	echo "install+++++++++++++++++++++++++++++++++"
	do_install
}

install_all

