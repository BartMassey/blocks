#!/bin/sh
I=$1
while [ $I -le $2 ]
do
	genrand.sh $3 $I
	I=`expr $I + 1`
done
