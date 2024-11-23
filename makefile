CFLAGS= -std=c11 -g -static

aucc: aucc.c

test: aucc
	./test.sh

clean:
	rm -f aucc *.o *~ tmp*

.PHONY: test clean