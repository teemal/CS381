CC = gcc
DEBUG = -g
DEFINES =

CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
        -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
        -Wdeclaration-after-statement $(DEFINES)
PROGS = primesMT


all: $(PROGS)


primesMT: primesMT.o
	$(CC) $(CFLAGS) -o $@ $^
	chmod a+rx,g-w $@

primesMT.o: primesMT.c
	$(CC) $(CFLAGS) -c $<


# clean up the compiled files and editor chaff
clean cls:
	rm -f $(PROGS) *.o *~ \#*

ci:
	ci -m"auto-checkin" -l *.[ch] ?akefile

git:
	git add .
	git commit -m "commit so you dont cry later"