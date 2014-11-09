# This is an example makefile
# It is very important to add -lncursesw for the ncurses library
# CC=g++
# DEBUG=
# CFLAGS=-c $(DEBUG)
# LDFLAGS=-lncursesw $(DEBUG)
# SOURCES=display.cpp Gin_Rummy.cpp Key.cpp CardSlot.cpp Point.cpp Card.cpp Deck.cpp DiscardPile.cpp Combo.cpp Player.cpp
# OBJECTS=$(SOURCES:.cpp=.o)
# EXECUTABLE=Play

DIR=`pwd`

g++ -c -I. -I$DIR/include -I$DIR/include display.cpp Gin_Rummy.cpp Key.cpp CardSlot.cpp Point.cpp Card.cpp Deck.cpp DiscardPile.cpp Combo.cpp Player.cpp
g++ -o server Gin_Rummy.o  $DIR/lib/libxmlrpc_server_abyss++.a $DIR/lib/libxmlrpc_server++.a $DIR/lib/libxmlrpc_server_abyss.a $DIR/lib/libxmlrpc_server.a $DIR/lib/libxmlrpc_abyss.a  -lpthread $DIR/lib/libxmlrpc++.a $DIR/lib/libxmlrpc.a $DIR/lib/libxmlrpc_util.a $DIR/lib/libxmlrpc_xmlparse.a $DIR/lib/libxmlrpc_xmltok.a -lsqlite3

g++ -g -c -I. -I$DIR/include -I$DIR/include testClient.cpp -o client.o
#g++ -g -c -I. -I$DIR/include -I$DIR/include display.cpp -o display.o
g++ -g testClient.o $DIR/lib/libxmlrpc_client++.a $DIR/lib/libxmlrpc_client.a $DIR/lib/libxmlrpc++.a $DIR/lib/libxmlrpc.a $DIR/lib/libxmlrpc_util.a $DIR/lib/libxmlrpc_xmlparse.a $DIR/lib/libxmlrpc_xmltok.a  -L/usr/lib/i386-linux-gnu -lcurl $DIR/lib/libxmlrpc_packetsocket.a -lncursesw -o client

# all: clean $(SOURCES) $(EXECUTABLE)

# debug: clean
# debug: DEBUG +=-g
# debug: $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS) 
# 	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# .cpp.o:
# 	$(CC) $(CFLAGS) $< -o $@

# clean:
# 	rm -rf *.o $(EXECUTABLE)

