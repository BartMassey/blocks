#!/bin/sh
TMP1=/tmp/check1.$$
TMP2=/tmp/check2.$$
awk '{print $(NF-1), $NF;}' < $1 > $TMP1
awk '{print $(NF-1), $NF;}' < $2 > $TMP2
diff $TMP1 $TMP2
