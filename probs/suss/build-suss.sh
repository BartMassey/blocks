#!/bin/sh
N=3
while [ $N -le $1 ]
do
	./build-suss.awk n=$N < /dev/null > suss-$N.prob
	N=`expr $N + 1`
done
