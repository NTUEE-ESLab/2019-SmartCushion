# CC and CFLAGS are varilables
CC=g++
CFLAGS=-c
OPTFLAGS=-O3
all : server client
	@echo -n "hello"
server : server.o
	$(CC) $(OPTFLAGS) server.o -o server
client : client.o
	$(CC) $(OPTFLAGS) client.o -o client
server.o : socket_server_pc_2.cpp
	$(CC) $(CFLAGS) $< -o $@
client.o : socket_client_2.cpp
	$(CC) $(CFLAGS) $< -o $@
clean :
	rm -rf *.o server client