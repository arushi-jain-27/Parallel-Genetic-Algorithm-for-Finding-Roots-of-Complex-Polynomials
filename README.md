# Parallel-Genetic-Algorithm-for-Finding-Roots-of-Complex-Polynomials


## Problem Statement
We wish to find the roots of a complex polynomial equation iteratively by taking an initial guess and computing step by step approximations which finally converge to a single solution over time. We solve this problem by creating a parallel genetic algorithm using MPI Master Slave Method. Genetic Algorithms are an effective method to simulate the process of natural selection. It can find satisfied solutions for some optimization problems which are difficult to solve with traditional methods such as exhaustive search and analytic method. When Genetic Algorithms are adopted on a parallel or distributed system, it can speed up the pace of solving the problems and get better solutions.

## How to run
Clone the repo. Run the following in terminal:

$ mkdir build/
$ cd build/
$ cmake ../
$ make
$ touch input.txt
$ mpirun -host ./Polynomial-Root-Finding-pGA input.txt [options]

Options for system
--population-size := specify population size
--accepted-error := specify accepted error
--mutation-rate := specify mutation rate
--mutation-radius := specify mutation radius
--starting-radius := specify starting radius


#### Refer to the report for more detials.
