#!/bin/sh
N=1
for i in 1 2 3 4 5 6 7 8
do
  for j in 1 2 3 4 5 6 7 8
  do
    (
      echo 20
      echo ""
      cat $i
      echo ""
      cat $j
    ) > rand-20-$N.prob
    N=`expr $N + 1`
  done
done
