all: a.out

rebuild: clean all

a.out: lab19.c
	cc -v -D_FILE_OFFSET_BITS=64 lab19.c

clean: 
	rm -rf a.out