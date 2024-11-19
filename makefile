CFLAGS=std=c11 -g -static

aucc: aucc.c

test: auc
	./test.sh

clean: rm -f aucc *.o *~ tmp*

.PHONY: test clean