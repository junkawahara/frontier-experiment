
#include <stdarg.h>

typedef struct Znum_ {
    int I;
} Znum;

typedef struct Vertex_ {
    struct Arc_ *arcs;
    char *name;
    struct Znum_ z;
} Vertex;

typedef struct Arc_ {
    struct Vertex_ *tip;
    struct Arc_ *next;
    int len;
} Arc;

typedef struct Graph_ {
    struct Vertex_ *vertices;
    int n;
    int m;
} Graph;

long int panic_code = 0;

Graph *restore_graph(char *filename) {
    Graph *g;

    int m = 0;
    int n = 0;
    int max_val = 0;
    int i, c, src, dest, found;

    int arcSrc[maxm]; // maxm is defined in simpath.c
    int arcDest[maxm];

    FILE* fp;
    char ss[65536];
    char *p;
    Arc *arc;
    
    if (filename == NULL || strcmp(filename, "-") == 0) {
        fp = stdin;
    } else {
        fp = fopen(filename, "r");
    }
    if (fp == NULL) {
        fprintf(stderr, "%s cannot be opened.\n", filename);
        exit(-99);
    }

    while (fgets(ss, sizeof(ss), fp) != NULL) {
        ++n;
        if (n > maxn) {
            fprintf(stderr, "The number of vertices must be smaller than %d\n", maxn);
            exit(-99);
        }

        p = strtok(ss, " ");
        while (p != NULL) {
            c = atoi(p);
            if (c > 0) {
                src = (c > n ? n : c);
                dest = (c > n ? c : n);
                if (dest > max_val) {
                    max_val = dest;
                }

                found = 0;
                for (i = 0; i < m; ++i) {
                    if (arcSrc[i] == src && arcDest[i] == dest) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    if (m >= maxm) {
                        fprintf(stderr, "The number of edgess must be smaller than %d\n", maxm);
                        exit(-99);
                    }
                    arcSrc[m] = src;
                    arcDest[m] = dest;
                    ++m;
                }
            }

            p = strtok(NULL, " ");
        }
    }
    if (fp != stdin) {
        fclose(fp);
    }
    if (max_val > n) {
        n = max_val;
    }
    if (n > maxn) {
        fprintf(stderr, "The number of vertices must be smaller than %d\n", maxn);
        exit(-99);
    }

    g = (Graph *)malloc(sizeof(Graph));
    g->vertices = (Vertex *)malloc(n * sizeof(Vertex));
    g->n = n;
    g->m = m;

    for (i = 0; i < n; ++i) {
        g->vertices[i].name = (char *)malloc(256);
        sprintf(g->vertices[i].name, "%d", i + 1);
        g->vertices[i].arcs = NULL;
        g->vertices[i].num = i + 1; // num is defined as z.I in simpath.c
    }

    for (i = 0; i < m; ++i) {
        arc = g->vertices[arcSrc[i] - 1].arcs;
        if (arc != NULL) {
            for ( ; arc->next != NULL; arc = arc->next) { }
            arc->next = (Arc *)malloc(sizeof(Arc) * 2);
            arc = arc->next;
        } else {
            g->vertices[arcSrc[i] - 1].arcs = (Arc *)malloc(sizeof(Arc) * 2);
            arc = g->vertices[arcSrc[i] - 1].arcs;
        }

        arc->len = 1;
        arc->tip = &g->vertices[arcDest[i] - 1];
        (arc + 1)->tip = &g->vertices[arcSrc[i] - 1];
    }

    return g;
}

void myprintf(char *format,...) {
#ifdef NO_OUTPUT
    const long long int buffsize = 100000000ll;
    static long long int count = 0;
    static int isfirst = 1;
    static char *output_p;

	va_list list;

    if (isfirst) {
        isfirst = 0;
        output_p = (char *)malloc(buffsize);
        if (output_p == NULL) {
            fprintf(stderr, "malloc failed!\n");
            exit(-99);
        }
    }

    if (count >= buffsize - 1024) {
        output_p = (char *)malloc(buffsize);
        if (output_p == NULL) {
            fprintf(stderr, "malloc failed!\n");
            exit(-99);
        }
        count = 0;
    }

	va_start(list, format);

	count += vsprintf(output_p + count, format, list);

	va_end(list);
#else
	va_list list;

	va_start(list, format);

	vprintf(format, list);

	va_end(list);
#endif
}
