#!/bin/sh
N=1
while [ $N -le $1 ]
do
	echo -n "$N "
	../blocks < rand-20-$N.prob
	N=`expr $N + 1`
done > rand-stats
