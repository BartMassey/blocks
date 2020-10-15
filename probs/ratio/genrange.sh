#!/bin/sh
INCR=${3-1}
I=1
while [ $I -le $2 ]
do
	J=1
	while [ $J -le $2 ]
	do
		genrand.sh $1 $2 $I $J
		J=`expr $J + $INCR`
	done
	I=`expr $I + $INCR`
done
