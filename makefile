CFLAGS= -std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

aucc: $(OBJS)
	$(CC) -o aucc $(OBJS) $(LDFLAGS) -Os

$(OBJS): aucc.h


test: aucc
	./test.sh

clean:
	rm -f aucc *.o *~ tmp*

.PHONY: test clean