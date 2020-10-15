#!/bin/sh
N=0
while [ $N -lt $1 ]
do
	randprob $2 > selman-$2-$N.prob
	N=`expr $N + 1`
done
