all: ja

ja: ja.c
	gcc ja.c -o ja -pthread

clean: rm ja *.o
