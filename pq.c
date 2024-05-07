#include "pq.h"

#include <stdlib.h>

pq_t init_pq( int size ) {
    pq_t q = malloc( sizeof * q );
    q->q = malloc( size * sizeof *(q->q) );
    q->s = size;
    q->n = 0;
    return q;
}

static void double_pq_size( pq_t pq ) {
    queue_elem_t *nq = realloc( pq->q, 2 * pq->s * sizeof *(pq->q) );
    if( nq == NULL )
        exit( 1 );
    pq->q = nq;
    pq->s *= 2;
}

static void heap_up( queue_elem_t* h, int n ) {
// before: h[0]...h[n-2] - a heap, h[n-1] - new element
// after: h[0]...h[n-1] - a heap
    int i = n-1;
    while( i > 0 ) {
        int p = (i-1) / 2;
        if( h[p].odl <= h[i].odl )
            return;
        queue_elem_t tmp = h[p];
        h[p] = h[i];
        h[i] = tmp;
        i = p;
    }
}

void add_to_pq( pq_t pq, queue_elem_t x ) {
    if( pq->n == pq->s )
        double_pq_size( pq );
    pq->q[pq->n++] = x;
    heap_up( pq->q, pq->n );
}

int not_empty_pq( pq_t pq ) {
    return pq->n != 0;
}

static void heap_down(queue_elem_t* h, int n ) {
// before: h[0]...h[n-1] - a heap but relation h[0] -> children can be false
// after: h[0]..h[n-1] - a heap
    int i = 0;
    int c = 2*i + 1;
    while( c < n ) {
        if( c+1 < n && h[c+1].odl < h[c].odl )
            c++;
        if( h[i].odl <= h[c].odl )
            return;
        queue_elem_t tmp = h[i];
        h[i] = h[c];
        h[c] = tmp;
        i = c;
        c = 2*i + 1;
    }
}

queue_elem_t pop_from_pq( pq_t pq ) {
    queue_elem_t ret = pq->q[0];
    pq->q[0] = pq->q[--pq->n];
    heap_down( pq->q, pq->n );
    return ret;
}

void free_pq( pq_t pq ) {
    free( pq->q );
    free( pq );
}

