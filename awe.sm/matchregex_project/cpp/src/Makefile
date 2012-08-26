CC = /usr/bin/g++
ODIR = obj
CFLAGS=-I. -I./deps/include
LDDLIBS=-L./deps/lib -Wl,-rpath,./deps/lib -lcppunit
DEPS = matchregex.h 

_OBJ = test_matchregex.o matchregex.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

TEST = test_matchregex

# Applies foreach *.o
$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all:
	gmake $(TEST)
	./test_matchregex

$(TEST): $(OBJ)
	gcc -o $@ $^ $(LDDLIBS)
	./test_matchregex

clean:
	rm -f *.o *~ obj/* $(TEST)

