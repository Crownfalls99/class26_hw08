all: echo-server echo-client

echo-server: IntArray.h Pthread.h echo-server.c
	g++ -o echo-server echo-server.c -lpthread

echo-client: Pthread.h echo-client.c
	g++ -o echo-client echo-client.c -lpthread
clean:
	rm -f *.o echo-server echo-client

