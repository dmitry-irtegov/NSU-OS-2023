all: test.txt lab27.out

rebuild: clean all

test.txt: 
	echo "some text\\n\\n\\n\\n" > test.txt

lab27.out: lab27.c
	gcc -Wall lab27.c -o lab27.out

clean:
	rm -rf test.txt lab27.out

