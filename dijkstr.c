#include <stdio.h>
#include <stdlib.h>

#include "init.h"
#include "pq.h"
#define SIZE 32

static int dijkstra_ile_pol(queue_elem_t* dij, int da, int db) {
    int i = db;
    int j = 1;
    while(i != da) {
        i = dij[i].p;
        j++;
    }
    return j;
}

static void dijkstra_print(queue_elem_t* dij, int da, int db) {

    printf("Najkrotsza droga z wezla %i do %i:\n", da, db);

    int x = dijkstra_ile_pol(dij, da, db);
    int* rev_dij = malloc(sizeof(int)*x);
    
    int i = db;
    for(int j = x-1; j >= 0; j--) {
        rev_dij[j] = dij[i].k;
        i = dij[i].p;
    }


    for(int j = 0; j < x; j++)
        printf("%i ", rev_dij[j]);
    printf("\n");

    free(rev_dij);
}

void dijkstra(graph_t* gr, int n, int da, int db) { // n = rows*columns

    queue_elem_t dij[n];// tablica odleglosci i poprzednikow (p) od wezla da
    pq_t queue = init_pq(SIZE);

    for(int i = 0; i < n; i++) {
        dij[i].k = i;
        dij[i].p = -1;// ujemne czyli ze nie ma
        if(i == da) {
            dij[i].odl = 0;
            add_to_pq(queue, dij[i]);
        }
        else
            dij[i].odl = INT_MAX;
    }
    
    queue_elem_t tmp;
    while( not_empty_pq(queue) ) {
        queue_elem_t act = pop_from_pq(queue);
        for(int j = 0; j < 4; j++) {
            if( gr[act.k].edg[j] != NULL ) {
                if( dij[act.k].odl + gr[act.k].val_edg[j] < dij[ gr[act.k].edg[j]->node ].odl ) {
                    dij[ gr[act.k].edg[j]->node ].odl = dij[ act.k ].odl + gr[act.k].val_edg[j];
                    dij[ gr[act.k].edg[j]->node ].p = dij[act.k].k;
                    add_to_pq(queue, dij[gr[act.k].edg[j]->node]);
                }
            }
        }
    }
    dijkstra_print(dij, da, db);
    free_pq(queue);
}
