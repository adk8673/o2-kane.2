CC=gcc

default: master producer consumer

%.o: %.c
	$(CC) -o $< -o $@

master: master.o, function_libary.o
	$(CC) -o master master.o function_library.o

producer: producer.o function_library.o
	$(CC) -o producer producer.o function_library.o

consumer: consumer.o function_library.o
	$(CC) -o consumer consumer.o function_library.o

clean:
	rm -f master
	rm -f producer
	rm -f consumer
	rm -f *.o
