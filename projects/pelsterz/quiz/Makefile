CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme -g testme.c $(CFLAGS)

testmeresults: clean testme

	./testme >> testmeresults.out

	gcov testme.c -f -b >> testmeresultsgcov.out
	cat testme.c.gcov >> testmeresultsgcov.out

all: testmeresults

clean:
	rm -f testme testmeresults.out testmeresultsgcov.out
