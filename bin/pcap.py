#!/usr/bin/python

import os

def fcopy(sf):
    sfpath = "/opt/data/pcapTmp/"+sf
    dfpath = "/opt/data/pcap/"+sf+"tmp"
    rdfpath = "/opt/data/pcap/"+sf;
    wfd = open(dfpath,"w")
    rfd = open(sfpath,"r")
    wfd.write(rfd.read())
    rfd.close()
    wfd.close()
    os.rename(dfpath,rdfpath)

if __name__ == '__main__':

    files = os.listdir('/opt/data/pcapTmp')
    files.sort(key=lambda a:int(a[4:len(a)-5]))

    for f in files:
        print('copy file:'+f)
        fcopy(f)
