CC=gcc
MASTER_OBJECTS=master.o function_library.o
PRODUCER_OBJECTS=producer.o function_library.o
CONSUMER_OBJECTS=consumer.o function_library.o
LINKEDLBS=-lrt
CFLAGS=-w

default: master producer consumer

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

master: $(MASTER_OBJECTS)
	$(CC) $(CFLAGS) $(LINKEDLBS) $(MASTER_OBJECTS) -o master

producer: producer.o function_library.o
	$(CC) $(CFLAGS) $(LINKEDLBS) $(PRODUCER_OBJECTS) -o producer

consumer: consumer.o function_library.o
	$(CC) $(CFLAGS) $(LINKEDLBS) $(CONSUMER_OBJECTS) -o consumer

clean:
	rm -f master
	rm -f producer
	rm -f consumer
	rm -f *.o
	rm -f *.log
