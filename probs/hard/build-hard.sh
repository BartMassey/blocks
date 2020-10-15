#!/bin/sh
N=4
while [ $N -le $1 ]
do
	./hardprob $N > hard-$N.prob
	N=`expr $N + 1`
done
