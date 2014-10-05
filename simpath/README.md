Knuth's simpath program
========

This package provides a header file and Makefile to compile Knuth's simpath program without Stanford Graphbase Library.
'ctangle' is needed for installation.

Installation:

1. Clone this project.

2. Download simpath.w (written by D. Knuth) from http://www-cs-faculty.stanford.edu/~uno/programs/simpath.w
    and put it into the directory same as that of myheader.h.

3. make

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

'simpath-silent' program is the same as 'simpath' except for outputting the constructed ZDD into the memory
instead of stdout.

