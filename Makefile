CC=gcc
CFLAGS= -g
p1: *.c *.h
p1.zip: *.c *.h
	zip p1.zip *.c *.h
clean::
	rm -f p1 a.out *.o core tests/*.diff p1.zip