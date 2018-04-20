### The Gas Station Problem

Similar to fair sleeping barber shop problem.

Our solution uses three semaphores, cars, which counts waiting cars (excluding the car in the petrol pump, who is not waiting), attenders, the number of attenders (3 in total) who are idle, waiting for cars, and mutex, which is used for mutual exclusion.  We also need a variable, waiting, which also counts the waiting customers.  
The reason for having waiting is that there is no way to read the current value of a semaphore.  In this solution, a car entering the pump has to count the number of waiting cars.  If it is less than the number of available waiting slots, he stays; otherwise, he leaves.
