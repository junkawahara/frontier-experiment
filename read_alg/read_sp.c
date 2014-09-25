//
// read_sp.cpp
//
// Copyright (c) 2014 Jun Kawahara
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// This program counts the number of spanning trees on an input graph
// using read's algorithm.
//
// The detail of the algorithm is described in the following paper and pptx slide:
//
// R. C. Read and R. E. Tarjan; "Bounds on backtrack algorithms for listing
// cycles, paths, and spanning trees"; Networks, No. 5, p.p. 237--252, 1975.
//
// http://www.cs.bgu.ac.il/~dinitz/Course/SS-12/Backtrack%20Algorithm%20for%20Listing%20Spanning%20Trees.pptx
//
// The format of the input graph is an adjacency list.
// The vertex numbers are given as natural numbers which start from 1.
// The i-th line of the input text has vertex numbers which are adjacent to the i-th vertex 
// and are larger than i, separated by a space.



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTEX 1024
#define MAX_EDGE 4096

int n;
int m;
int edge_src_array[MAX_EDGE];
int edge_dest_array[MAX_EDGE];

int adj_list[MAX_VERTEX + 1][MAX_VERTEX];
int adj_num_list[MAX_VERTEX + 1];

int inc_list[MAX_VERTEX + 1][MAX_EDGE];
int inc_num_list[MAX_VERTEX + 1];

int G[MAX_VERTEX];
int PST[MAX_VERTEX];

int counter = 0;

// Check A \subseteq B
int IsInclude(int* A, int* B)
{
    int i;
    for (i = 0; i < m; ++i) {
        if (A[i] == 1 && B[i] == 0) {
            return 0;
        }
    }
    return 1;
}

// return whether a u-v path on 'graph' exists
int IsReachable(int u, int v, int* graph)
{
    int i, e, x, y;
    static int visited[MAX_VERTEX];
    static int queue[MAX_VERTEX];
    static int queue_count;

    for (i = 0; i < m; ++i) {
        visited[i] = 0;
    }
    visited[u] = 1;

    queue[0] = u;
    queue_count = 1;

    while (queue_count > 0) {
        x = queue[queue_count - 1];
        --queue_count;
        for (i = 0; i < adj_num_list[x]; ++i) {
            y = adj_list[x][i];
            e = inc_list[x][i];
            if (graph[e] == 1) {
                if (y == v) {
                    return 1;
                }
                if (!visited[y]) {
                    visited[y] = 1;
                    queue[queue_count] = y;
                    ++queue_count;
                }
            }
        }
    }
    return 0;
}

// return whether e is a bridge of G
int IsBridge(int e)
{
    int b;

    G[e] = 0;
    b = !IsReachable(edge_src_array[e], edge_dest_array[e], G);
    G[e] = 1;

    return b;
}

void Compute_B_first(int* B)
{
    int i;

    // Compute the set B of edges i such that i in G, i not in PST
    // and both endpoints of i are included in the same connected component.
    for (i = 0; i < m; ++i) {
        if (G[i] == 1 && PST[i] == 0 && IsReachable(edge_src_array[i], edge_dest_array[i], PST)) {
            B[i] = 1;
        } else {
            B[i] = 0;
        }
    }
}

void Compute_B_second(int* B)
{
    int i;

    // Compute the set B of edges i such that i in G, i not in PST
    // and i is a bridge in G.
    for (i = 0; i < m; ++i) {
        if (G[i] == 1 && PST[i] == 0 && IsBridge(i)) {
            B[i] = 1;
        } else {
            B[i] = 0;
        }
    }
}


void REC()
{
    int i, j, ex;
    int B[MAX_VERTEX];

    if (IsInclude(G, PST)) {
        ++counter;
        return;
    }
    for (i = 0; i < m; ++i) {
        if (G[i] == 1 && PST[i] == 0) {
            break;
        }
    }
    ex = i;

    PST[ex] = 1;

    Compute_B_first(B);

    // compute G = G - B
    for (i = 0; i < m; ++i) {
        if (B[i] == 1) {
            G[i] = 0;
        }
    }

    REC();

    PST[ex] = 0;

    // compute G = G \cup B
    for (i = 0; i < m; ++i) {
        if (B[i] == 1) {
            G[i] = 1;
        }
    }

    G[ex] = 0;

    Compute_B_second(B);

    // compute PST = PST \cup B
    for (i = 0; i < m; ++i) {
        if (B[i] == 1) {
            PST[i] = 1;
        }
    }

    REC();

    // compute PST = PST - B
    for (i = 0; i < m; ++i) {
        if (B[i] == 1) {
            PST[i] = 0;
        }
    }

    G[ex] = 1;
}

// parse an adjacency list
void ParseInput()
{
    int i, x, src, dest, max_v;
    char ss[1024];
    char* p;

    n = 0;
    m = 0;
    max_v = 0;

    while (fgets(ss, sizeof(ss), stdin) != NULL) {
        if (n >= MAX_VERTEX) {
            fprintf(stderr, "The number of vertices must be smaller than %d.\n", MAX_VERTEX);
            exit(1);
        }
        ++n;

        p = strtok(ss, " ");

        while (p != NULL) {
            x = atoi(p);
            if (n < x) {
                src = n;
                dest = x;
            } else {
                src = x;
                dest = n;
            }
            if (max_v < x) {
                max_v = x;
                if (max_v > MAX_VERTEX) {
                    fprintf(stderr, "The number of vertices must be smaller than %d.\n", MAX_VERTEX);
                    exit(1);
                }
            }

            for (i = 0; i < m; ++i) {
                if (edge_src_array[i] == src && edge_dest_array[i] == dest) {
                    break;
                }
            }
            if (i >= m) {
                if (m >= MAX_EDGE) {
                    fprintf(stderr, "The number of edges must be smaller than %d.\n", MAX_EDGE);
                    exit(1);
                }
                edge_src_array[m] = src;
                edge_dest_array[m] = dest;
                ++m;
            }

            p = strtok(NULL, " ");
        }
    }
    if (n < max_v) {
        n = max_v;
    }
}

int main()
{
    int i, j, v;

    ParseInput();

    for (v = 1; v <= n; ++v) {
        adj_num_list[v] = 0;
        for (j = 0; j < m; ++j) {
            if (v == edge_src_array[j]) {
                adj_list[v][adj_num_list[v]] = edge_dest_array[j];
                ++adj_num_list[v];
                inc_list[v][inc_num_list[v]] = j;
                ++inc_num_list[v];
            }
            if (v == edge_dest_array[j]) {
                adj_list[v][adj_num_list[v]] = edge_src_array[j];
                ++adj_num_list[v];
                inc_list[v][inc_num_list[v]] = j;
                ++inc_num_list[v];
            }
        }
    }

    for (i = 0; i < m; ++i) {
        G[i] = 1;
    }

    for (i = 0; i < m; ++i) {
        PST[i] = (IsBridge(i) ? 1 : 0);
    }

    REC();

    printf("# of spanning trees = %d\n", counter);

    return 0;
}

