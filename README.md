# FeedbackArcSetProblem

Metaheuristic algorithm to solve Weighted Feedback Arc Set Problem.

This problem was assigned as a semestral project for Combinatorial Optimisation class at FEE CTU.
My solution placed 4th in the whole class on benchmark graphs unknown to students.

## Problem Description

Given an directed graph with non-negative weigts, remove edges in such a way that the remaining graph is Directed Acyclic Graph.
The sum of weights of the removed edges should be minimal.

## Implemented algorithm

The Metaheuristic algorithm is Adaptive Large Neighborhood Search (ALNS) with Tournament selection and Simulated Annealing acceptance criterion.
