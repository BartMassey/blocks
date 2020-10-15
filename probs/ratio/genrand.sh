#!/bin/sh
N=0
while [ $N -lt $1 ]
do
	./randprob $2 $3 $4> ratio-${2}-${3}x${4}-${N}.prob
	N=`expr $N + 1`
done
