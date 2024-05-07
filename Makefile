main: main.o init.o bfs.o dijkstr.o pq.o
	$(CC) main.o init.o bfs.o dijkstr.o pq.o
	
init.o: init.c init.h
	$(CC) -c init.c
	
dijkstr.o: dijkstr.c dijkstr.h pq.o
	$(CC) -c dijkstr.c

bfs.o: bfs.c bfs.h pq.o
	$(CC) -c bfs.c

pq.o: pq.c pq.h
	$(CC) -c pq.c

.PHONY: clean

clean:
	rm *.o a.out