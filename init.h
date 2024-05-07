#ifndef _INIT_H
#define _INIT_H

typedef struct graph {
    int node;
    struct graph** edg;
    double* val_edg;
    int nmb_edg;
} graph_t;

double losuj(int a, int b);

graph_t* graph_read(char *file, int* height, int* width);

graph_t* graph_init(int height, int width, double a, double b);

void graph_free(graph_t** graf, int height, int width);

void graph_print(graph_t* graf, int height, int width);

void graph_fwrite(char* out, graph_t* graf, int height, int width);

#endif
