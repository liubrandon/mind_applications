IDIR = .
CC=gcc
CFLAGS=-I$(IDIR) -lpthread -g -D __TEST__ -O2 -D_GNU_SOURCE

obj-y += test_program.cpp

default: test_protocol ralloc

ralloc: ralloc.c
	@mkdir -p bin
	$(CC) -shared -fPIC -o ralloc.so ralloc.c -ldl

api_test: $(obj-y)
	@mkdir -p bin
	$(CC) -o bin/$@ $^ $(CFLAGS) $(LIBS)

clean:
	@rm -f *.o
	@rm -f *.so
	@rm -f bin/*

