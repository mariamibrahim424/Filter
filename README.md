# The Great Wall of Santa Cruz

In this project, The Grear Firewall of Santa Cruz, the program takes in a list
of "badspeak" words and a pair of "oldspeak" and "newspeak" words.
It hashes the oldspeak and badspeak words and inserts them into the specified
hash table index as well as set the 3 bits, that resulted from hashing,
into the bloom filter location. If the user uses any of the badspeak and or oldspeak words,
which are in the hash table, they will be notified that they commited a crime.

## build

$ clang -Wall -Werror -Wextra -Wpedantic -c banhammer.c
$ clang -Wall -Werror -Wextra -Wpedantic -c node.c
$ clang -Wall -Werror -Wextra -Wpedantic -c ll.c
$ clang -Wall -Werror -Wextra -Wpedantic -c hash.c
$ clang -Wall -Werror -Wextra -Wpedantic -c speck.c  
 $ clang -Wall -Werror -Wextra -Wpedantic -c bv.c
$ clang -Wall -Werror -Wextra -Wpedantic -c bf.c  
 $ clang -Wall -Werror -Wextra -Wpedantic -c parser.c
$ clang -o banhammer banhammer.o node.o ll.o hash.o speck.o bv.o bf.o parser.o

## Running

$ ./banhammer

## Command Line Options

$ -h: specify hash table size
$ -f: specify bloom filter size
$ -m: enables the move to front option

## Cleaning

    $ rm -f banhammer*.o


## Format

    $ clang-format -i -style=file banhammer.c node.h node.c ll.c ll.h hash.h hash.c speck.c speck.h bv.h bv.c bf.c bf.h parser.c parser.h
