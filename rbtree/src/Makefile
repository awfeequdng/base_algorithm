
ALL_INCS=-I ./

CFLAGS = $(ALL_INCS)

all: build

build: rbtree.o 

*.o: *.c
	gcc -c $^ $(CFLAGS)
	

clean:
	rm *.o 
