all: server client

server: server.cpp
	g++ -o server server.cpp -lpthread

client: client.cpp
	g++ -o client client.cpp -lpthread

clean:
	rm server client