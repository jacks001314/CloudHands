make -C lib/common
make -C lib/thread
make -C lib/fstore
make -C lib/ptables
make -C lib/shm
make -C MProcess
make -C packet
make -C rule
make -C filter
make -C SessionMonitor
make -C tcp/app/http
make -C tcp/app/ftp
make -C tcp/app/mail
make -C tcp/app/smon
make -C tcp/app/ssh
make -C tcp/app/telnet
make -C tcp/app
cp -r tcp/app/http/*.o tcp/app
cp -r tcp/app/ftp/*.o tcp/app
cp -r tcp/app/mail/*.o tcp/app
cp -r tcp/app/mail/smtp/*.o tcp/app
cp -r tcp/app/mail/pop3/*.o tcp/app
cp -r tcp/app/mail/imap/*.o tcp/app
cp -r tcp/app/smon/*.o tcp/app
cp -r tcp/app/ssh/*.o tcp/app
cp -r tcp/app/telnet/*.o tcp/app

make -C tcp

