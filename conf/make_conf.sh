#!/bin/bash

usage="make_conf.sh <install_dir> <data_dir> <log_dir> <rules_dir>"


if [ $# -lt 4 ]; then

	echo $usage
	exit
fi

install_dir=${1//\//\\\/}
data_dir=${2//\//\\\/}
log_dir=${3//\//\\\/}
rules_dir=${4//\//\\\/}

echo "{install_dir:$install_dir,data_dir:$data_dir,log_dir:$log_dir,rules_dir:$rules_dir}"

conf_files="pdispatcher.conf \
            pdump.conf \
            psink.conf \
            tcp/TCPMain.conf \
            tcp/tcp_app.conf \
            tcp/app/ftp.conf \
            tcp/app/http.conf \
            tcp/app/mail.conf \
            tcp/app/mysql.conf \
            tcp/app/smon.conf \
            tcp/app/ssh.conf \
            tcp/app/telnet.conf \
            udp/UDPMain.conf \
            udp/udp_app.conf \
            udp/app/smon.conf \
            udp/app/tftp.conf \
            sa/SAMain.conf \
            "

rule_files="Filter.json \
            tcp/app/http_filter.json \
            tcp/app/http.json \
            tcp/app/smon.json \
            udp/Filter.json \
            udp/app/smon.json \
            sa/Filter.json \
            "

for cf in $conf_files; do 

    sed -i "s/{{.install_dir}}/$install_dir/g" $cf
    sed -i "s/{{.data_dir}}/$data_dir/g" $cf
    sed -i "s/{{.log_dir}}/$log_dir/g" $cf
done

for rf in $rule_files; do 
    sed -i "s/{{.rules_dir}}/$rules_dir/g" $rf
done