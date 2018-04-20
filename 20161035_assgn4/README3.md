
### Concurrent Mergesort
Time analysis and comparision of concurrent mergesort with normal mergesort.

Details
There are two types of merge sort I have implemented.
1. mergesort_normal (without forking, just one process, and also without selection sort).
2. mergesort_concurrent (forking for every part containing more than 5 elements).

#### random number generation to test
Test Runs
1. n = 10^5
a. mergesort_normal
real	0m0.040s
user	0m0.062s
sys	0m0.010s

b. mergesort_concurrent
real	0m5.190s
user	0m0.510s
sys	0m26.812s
(RUNS SOMETIMES)

2. n = 10^7
a. mergesort_normal
real	0m4.811s
user	0m4.782s
sys	0m0.380s

Runs for smaller numbers

b. mergesort_concurrent
CPU can't run for this much big process. Resource allocation not possible.
