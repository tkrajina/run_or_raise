CC=colorgcc
CFLAGS=-Wall -g
PKG_CONFIG=$(shell pkg-config --cflags --libs x11)

make:
	$(CC) $(CFLAGS) raise_window.c $(PKG_CONFIG) -o raise_window
clean:
	rm raise_window
