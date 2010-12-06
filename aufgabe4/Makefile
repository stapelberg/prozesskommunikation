all:
	gcc -Wall -c -o conv.o conv.c
	gcc -Wall -c -o log.o log.c
	gcc -Wall -c -o statistic.o statistic.c
	gcc -Wall -c -o monitor.o monitor.c
	gcc -Wall -c -o main.o main.c
	gcc -Wall -c -o queue.o queue.c
	gcc -o pk main.o conv.o log.o statistic.o monitor.o queue.o -lrt

clean:
	rm -f *.o
