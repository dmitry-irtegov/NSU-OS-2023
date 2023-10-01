all: perm

rebuild: clean all

lab3:
	cc -v -o lab3 lab3.c

test.txt:
	echo "Test data...." > test.txt

perm: test.txt lab3
	chmod 600 test.txt
	chmod 4755 lab3

clean:
	rm -rf test.txt
	rm -rf lab3