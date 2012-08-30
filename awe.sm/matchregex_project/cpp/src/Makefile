CC = /usr/bin/g++
ODIR = obj

INCLUDES= \
	-I. \
	-I./deps/include \
	-I/usr/include \
	$(NULL)

LDDLIBS= \
	-L./deps/lib -Wl,-rpath,./deps/lib -lcppunit \
	$(NULL)

DEPS = matchregex.h 

_OBJ = test_matchregex.o matchregex.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ2 = main_matchregex.o matchregex.o 
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))

TEST = test_matchregex

EXECUTE = main_matchregex

# Applies foreach *.o
$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(INCLUDES)

all:
	gmake $(TEST)
	./test_matchregex

$(TEST): $(OBJ)
	gcc -o $@ $^ $(LDDLIBS)

$(EXECUTE): $(OBJ2)
	gcc -o $@ $^ $(LDDLIBS)

clean:
	rm -f *.o *~ obj/* $(TEST) $(EXECUTE)


