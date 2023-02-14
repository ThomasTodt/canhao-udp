CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
CLIENT_SOURCES=cliente.c
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)
CLIENT_EXECUTABLE=cliente
SERVER_SOURCES=servidor.c
SERVER_OBJECTS=$(SERVER_SOURCES:.c=.o)
SERVER_EXECUTABLE=servidor

all: client server

client: $(CLIENT_SOURCES) $(CLIENT_EXECUTABLE)

server: $(SERVER_SOURCES) $(SERVER_EXECUTABLE)

$(CLIENT_EXECUTABLE): $(CLIENT_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(CLIENT_OBJECTS) $(SERVER_OBJECTS)

purge: clean
	rm -rf $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE)