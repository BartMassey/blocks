#!/bin/awk -f
END {
  for (i = 3; i <= 20; i++) {
    k=0.853675 * i
    printf "%d %g\n", i, k*k*k
  }
}
