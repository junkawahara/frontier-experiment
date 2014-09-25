read_alg
===================

This program counts the number of spanning trees on an input graph
using read's algorithm.

The detail of the algorithm is described in the following paper and pptx slide:

R. C. Read and R. E. Tarjan; "Bounds on backtrack algorithms for listing
cycles, paths, and spanning trees"; Networks, No. 5, p.p. 237--252, 1975.

http://www.cs.bgu.ac.il/~dinitz/Course/SS-12/Backtrack%20Algorithm%20for%20Listing%20Spanning%20Trees.pptx

The format of the input graph is an adjacency list.
The vertex numbers are given as natural numbers which start from 1.
The i-th line of the input text has vertex numbers which are adjacent to the i-th vertex 
and are larger than i, separated by a space.

For example:
```
2 3 4
3


```
represents the graph which has four vertices and four edges
(1--2, 1--3, 1--4 and 2--3).


Usage:
```
make
./read_sp < input.txt
```

