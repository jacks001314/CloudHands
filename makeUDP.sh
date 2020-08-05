make -C lib/common
make -C lib/thread
make -C lib/fstore
make -C lib/ptables
make -C lib/shm
make -C MProcess
make -C packet
make -C rule
make -C filter
make -C udp/app/sdns
make -C udp/app/tftp
make -C udp/app/smon
make -C udp/app
cp -r udp/app/sdns/*.o udp/app
cp -r udp/app/tftp/*.o udp/app
cp -r udp/app/smon/*.o udp/app

make -C udp
