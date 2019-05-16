
PROG = primesMT

CC = gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized -pthread -Werror -Wunsafe-loop-optimizations

 
all: $(PROG)
 
$(PROG): primesMT.o 
	$(CC) -o $@ $^

primesMT.o: primesMT.c
	$(CC) -c $< 

clean:
	rm -f $(PROG) $(PROG2) *.o *~ \#*
