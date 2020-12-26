
SRCS=test_rbtree.c ../src/rbtree.c

ALL_INCS=-I../src -I./

CFLAGS=$(ALL_INCS)

build: $(SRCS)
	gcc -g -o test_rbtree $^ $(CFLAGS)

clean:
	rm test_rbtree
	
