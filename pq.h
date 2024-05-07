#ifndef _PQ_H_
#define _PQ_H_

typedef struct d_t {
    double odl;
    int p;
    int k;
} d_t;

typedef struct {
    d_t *q;  // the queue
    int n;      // actual # of doubles in it
    int s;      // it's size
} * pq_t;

pq_t init_pq( int size );

void add_to_pq( pq_t pq, d_t x );

int not_empty_pq( pq_t pq );

d_t pop_from_pq( pq_t pq );

void free_pq( pq_t pq );

#endif
