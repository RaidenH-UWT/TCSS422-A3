make
rmmod $1
insmod $1
echo
echo LOGS
echo
tail -n 20 /var/log/syslog
