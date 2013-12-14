CC=colorgcc
CFLAGS=-Wall -g
PKG_CONFIG=$(shell pkg-config --cflags --libs x11)

make:
	$(CC) $(CFLAGS) run_or_raise.c $(PKG_CONFIG) -o run_or_raise
clean:
	rm run_or_raise
