#!/bin/sh
N=0
while [ $N -lt $1 ]
do
	./randprob $2 > rand-20-$N.prob
	N=`expr $N + 1`
done
