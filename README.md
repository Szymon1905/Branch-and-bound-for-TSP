# C++ Implementation of the Branch and Bound Algorithm for the Traveling Salesman Problem

This project implements the **Branch and Bound (B&B)** algorithm in C++ to solve the **Traveling Salesman Problem (TSP)**. The algorithm  explores possible tours while using lower bound estimates to prune unpromising branches, reducing the search space compared to brute force.


## Algorithm Overview

The Branch and Bound algorithm works by:

1. Generating partial tours (branches).
2. Computing a lower bound on the minimum possible tour cost.
3. Pruning branches whose lower bound exceeds the current best solution.
4. Continuing until the optimal Hamiltonian cycle is found.

## Building the Project

Build with cmake


