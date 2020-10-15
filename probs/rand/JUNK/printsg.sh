#!/bin/sh

awk '
BEGIN {
	i = 1
	nr = 1
}
/^$/ {
	nr++
	next
}
{
	if (nr == 1) {
		n = $0
	} else {
		val[i] = $0
		nrr[i] = nr
		i++
	}
}
END {
   j = 1
   for (i = 1; i <= nr; i++) {
     printf "%d %d\n%d\n", i, n, i
     while (nrr[j] == i)
       printf "%d %d\n", i, val[j++]
   }
}' |
