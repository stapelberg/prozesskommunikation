all:
	gcc -Wall -c -g -o conv.o conv.c
	gcc -Wall -c -g -o log.o log.c
	gcc -Wall -c -g -o statistic.o statistic.c
	gcc -Wall -c -g -o monitor.o monitor.c
	gcc -Wall -c -g -o main.o main.c
	gcc -o pk main.o conv.o log.o statistic.o monitor.o -lrt

clean:
	rm -f *.o
