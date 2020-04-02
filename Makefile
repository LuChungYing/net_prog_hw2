all: client server                                                                                                                                                                                      
client: client.o
	gcc  -o client client.o 
server: server.o
	gcc  -o server server.o 
client.o:
	gcc -c client.c
server.o:
	gcc -c server.c
clean:
	rm -f client.o server.o
