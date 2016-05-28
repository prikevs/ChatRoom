PARAMS=-I ../common

server: client.o main.o msgdispose.o network.o session.o linkedlist.o message.o libmsg.o
	gcc -o server client.o main.o msgdispose.o network.o session.o linkedlist.o message.o libmsg.o -lpthread
client.o: client.c
	gcc -c client.c $(PARAMS)
main.o: main.c
	gcc -c main.c $(PARAMS)
msgdispose.o: msgdispose.c
	gcc -c msgdispose.c $(PARAMS)
network.o: network.c
	gcc -c network.c $(PARAMS)
session.o: session.c
	gcc -c session.c $(PARAMS)
linkedlist.o: ../base/linkedlist.c
	gcc -c ../base/linkedlist.c $(PARAMS)
message.o: ../base/message.c
	gcc -c ../base/message.c $(PARAMS)
libmsg.o: ../base/libmsg.c
	gcc -c ../base/libmsg.c $(PARAMS)

clean:
	rm *.o
