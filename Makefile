# This is an example makefile
# It is very important to add -lncursesw for the ncurses library
CC=g++
DEBUG=
CFLAGS=-c $(DEBUG)
LDFLAGS=-lncursesw $(DEBUG)
SOURCES=display.cpp Gin_Rummy.cpp Key.cpp CardSlot.cpp Point.cpp Card.cpp Deck.cpp DiscardPile.cpp Combo.cpp Player.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Play

all: clean $(SOURCES) $(EXECUTABLE)

debug: clean
debug: DEBUG +=-g
debug: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

