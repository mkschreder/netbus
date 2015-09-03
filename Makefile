SERVER_SOURCES:=src/netbusd.c
SERVER_OBJECTS:=$(patsubst %.c,%.o,$(SERVER_SOURCES))
SERVER_EXE:=netbusd

CLIENT_SOURCES:=src/netbus.c
CLIENT_OBJECTS:=$(patsubst %.c,%.o,$(CLIENT_SOURCES))
CLIENT_EXE:=netbus

all: $(SERVER_OBJECTS) $(CLIENT_OBJECTS)
	$(CC) -o $(SERVER_EXE) $(SERVER_OBJECTS) -lzmq -lubox
	$(CC) -o $(CLIENT_EXE) $(CLIENT_OBJECTS) -lzmq -lubox

%.o: %.c $(wildcard src/*.h)
	$(CC) -c $< -o $@

clean: 
	rm $(SERVER_EXE)
