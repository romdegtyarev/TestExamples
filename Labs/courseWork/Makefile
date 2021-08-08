all: proto server client clientManager

proto:
	protoc-c --c_out=. amessage.proto

server : server.o
	gcc -std=c99 -o server server.o -lpthread -lprotobuf-c
client : client.o
	gcc -std=c99 -o client client.o -lprotobuf-c
clientManager : clientManager.o
	gcc -std=c99 -o clientManager clientManager.o

server.o : server.c
	gcc -std=c99 -c server.c
client.o : client.c
	gcc -std=c99 -c client.c
clientManager.o : clientManager.c
	gcc -std=c99 -c clientManager.c

run:
	xterm -e ./server localhost 57000 &
	xterm -e ./client localhost 57000 &
	xterm -e ./clientManager localhost 57002 &

clean :
	rm -f *.o server client clientManager amessage.pb-c.h amessage.pb-c.c