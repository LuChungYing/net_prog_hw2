all: client server                                                                                                                                                                                      
client: client.o huffman.o
	gcc  -o client client.o 
server: server.o
	gcc  -o server server.o 
client.o: huffman.h
	gcc -c client.c
server.o:
	gcc -c server.c
huffman.o:
	gcc -c huffman.c
clean:
	rm -f client.o server.o
