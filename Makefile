CC = clang 
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all : banhammer

banhammer: banhammer.o node.o ll.o hash.o speck.o bv.o bf.o parser.o
	$(CC) -o banhammer banhammer.o node.o ll.o hash.o speck.o bv.o bf.o parser.o

banhammer.o : banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

node.o : node.c
	$(CC) $(CFLAGS) -c node.c 

ll.o : ll.c
	$(CC) $(CFLAGS) -c ll.c 

hash.o : hash.c
	$(CC) $(CFLAGS) -c hash.c

speck.o : speck.c
	$(CC) $(CFLAGS) -c speck.c  

bv.o : bv.c
	$(CC) $(CFLAGS) -c bv.c

bf.o : bf.c
	 $(CC) $(CFLAGS) -c bf.c  

parser.o : parser.c
	$(CC) $(CFLAGS) -c parser.c

clean :
	rm -f banhammer *.o

format :
	clang-format -i -style=file banhammer.c node.h node.c ll.c ll.h hash.h hash.c speck.c speck.h bv.h bv.c bf.c bf.h parser.c parser.h



