CC = cc

SRCS = main.c 11.c

EXECS = $(SRCS:.c=.out)

all: $(EXECS)

main.out: main.c
		$(CC) main.c -o main.out

11.out: 11.c
		$(CC) 11.c -o 11.out

clean:
		rm -f $(EXECS)
