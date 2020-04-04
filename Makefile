all: client server                                                                                                                                                                                      
client: huffman.o client.o
	gcc  -o client client.o huffman.o
server: server.o
	gcc  -o server server.o 
huffman.o:
	gcc -c huffman.c
client.o: huffman.h huffman.c
	gcc -c client.c huffman.c
server.o:
	gcc -c server.c
clean:
	rm -f client.o server.o
