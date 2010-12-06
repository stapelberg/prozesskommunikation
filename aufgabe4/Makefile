all:
	gcc -g -Wall -c -o conv.o conv.c
	gcc -g -Wall -c -o log.o log.c
	gcc -g -Wall -c -o statistic.o statistic.c
	gcc -g -Wall -c -o monitor.o monitor.c
	gcc -g -Wall -c -o main.o main.c
	gcc -g -Wall -c -o queue.o queue.c
	gcc -g -o pk main.o conv.o log.o statistic.o monitor.o queue.o -lrt

clean:
	rm -f *.o
