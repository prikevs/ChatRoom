PARAMS=-I ../common

server: main.o network.o msghandler.o linkedlist.o message.o libmsg.o
	gcc -o client main.o msghandler.o network.o linkedlist.o message.o libmsg.o
main.o: main.c
	gcc -c main.c $(PARAMS)
msghandler.o: msghandler.c
	gcc -c msghandler.c $(PARAMS)
network.o: network.c
	gcc -c network.c $(PARAMS)
linkedlist.o: ../base/linkedlist.c
	gcc -c ../base/linkedlist.c $(PARAMS)
message.o: ../base/message.c
	gcc -c ../base/message.c $(PARAMS)
libmsg.o: ../base/libmsg.c
	gcc -c ../base/libmsg.c $(PARAMS)

clean:
	rm *.o
