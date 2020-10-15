#!/bin/sh
(
	sed 1d < $1 | awk "{print \"$1\", NR, \$1, \$NF;}"
	sed 1d < $2 | awk "{print \"$2\", NR, \$1, \$NF;}"
) |
sort -n +1 |
awk '{print $1, $3, $4;}' |
uniq -u -1
