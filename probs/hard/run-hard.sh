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
*)
	ALG=""
	ALGNAME="a"
	;;
esac
N=4
while [ $N -le $1 ]
do
	echo -n "${N} "
	../../bin/blocks $ALG < hard-$N.prob
	N=`expr $N + 1`
done
