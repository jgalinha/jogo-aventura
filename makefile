all: ja

ja: ja.c
	gcc ja.c -o ja

clean: rm ja *.o
