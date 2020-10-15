#!/bin/sh
N=0
while [ $N -lt $1 ]
do
	./bwstates -s 2 $2 > slaney-$2-$N.prob
	N=`expr $N + 1`
done
