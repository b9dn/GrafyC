#include <stdlib.h>
#include <stdio.h>
#include "pq.h"
#include "bfs.h"
#include "init.h"
#define SIZE 16


int bfs(graph_t* gr,int height,int width) {
    pq_t pq = init_pq(SIZE);
    d_t* visited = malloc(sizeof(d_t) * height*width);
    visited[0].odl = 0;
    visited[0].k = 0;
    visited[0].p = 0;
    add_to_pq(pq, visited[0]);

    for(int k = 1; k < height*width; k++) {
        visited[k].p = -1;
    }

    d_t i;
    while(not_empty_pq( pq )) {
        i = pop_from_pq(pq);
        for(int j = 0; j < 4; j++) {
            if(gr[i.k].edg[j] != NULL && visited[gr[i.k].edg[j]->node].p == -1) {
                int x = gr[i.k].edg[j]->node;
                visited[x].k = x;
                visited[x].p = i.k;
                visited[x].odl = gr[i.k].val_edg[j];
                add_to_pq(pq, visited[x]);
            }
        }
    }
    free_pq(pq);

    for(int k = 0; k < height* width; k++) {
        if(visited[k].p == -1) {
            printf("\nGraf niespojny\n");
            return 0;
        }
    }
    printf("\nGraf spojny\n");
    free(visited);

    return 1;
}
