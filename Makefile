

all: client server

client: client.cpp
	g++ -lwsock32 client.cpp -lws2_32 -o client.exe

server: server.cpp
	g++ -lwsock32 server.cpp -lws2_32 -o server.exe