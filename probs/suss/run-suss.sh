#!/bin/sh
N=3
while [ $N -le $1 ]
do
	echo "${N}:"
	( time ./blocks < suss-$N.prob | sed '1!d' ) 2>&1
	N=`expr $N + 1`
done
