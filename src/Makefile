IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = roomGen.h tiles.h stack.h defs.h item.h dungeon.h player.h title.h enemy.h merchant.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = roomGen.o stack.o main.o dungeon.o player.o defs.o title.o enemy.o merchant.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

room: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 