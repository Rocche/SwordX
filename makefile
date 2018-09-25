CC=gcc
CFLAGS= -I$(IDIR)

BDIR=bin
ODIR=obj
IDIR=inc
SDIR=src

_DEPS = trie.h sbolist.h memory_operations.h file_operations.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o trie.o sbolist.o memory_operations.o file_operations.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

swordx: $(OBJ)
	$(CC) $(CFLAGS) -o $(BDIR)/$@ $^

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all install uninstall

all: swordx

install:
	@echo "Installing swordx....\n"
	@cp ./bin/swordx /usr/local/bin/swordx
	@echo "Done."

uninstall:
	@echo "Uninstalling swordx..\n"
	@rm -f /usr/local/bin/swordx
	@echo "Done."
