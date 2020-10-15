#!/bin/awk -f
END {
	printf "%d\n\n", n
	for (i = 1; i <= n - 2; i++)
	  printf "%d\n", i
	printf "-1\n-1\n\n"
	for (i = 1; i <= n - 1; i++)
	  printf "%d\n", i
	printf "-1\n"
}
