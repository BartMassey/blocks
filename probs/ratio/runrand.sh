#!/bin/sh
case $1 in
ida)
	ALG="-a ida"
	ALGNAME="ida"
	shift
	;;
rida)
	ALG="-a rida"
	ALGNAME="rida"
	shift
	;;
ra)
	ALG="-a ra"
	ALGNAME="ra"
	shift
	;;
*)
	ALG=""
	ALGNAME="a"
	;;
esac
if [ "$1" != "" ]
then
	TIME="-t $1"
else
	TIME=""
fi
for i in `cat prob-list`
do
	echo -n "$i "
	../../bin/blocks $ALG $TIME < $i
done
