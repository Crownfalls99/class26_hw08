# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <arpa/inet.h>
# include <sys/socket.h>

# include "IntArray.h"
# include "Pthread.h"

# ifndef __BOOLEAN
# define __BOOLEAN

# define true 	1
# define false 	0

# endif
# define BUFSIZE 65536

int echo = false, broadcast = false, port = 0;
intArray sdlist;

void usage(void) {
	printf("syntax : echo-server <port> [-e [-b]]\n");
	printf("sample : echo-client 1234 -e -b\n");
}

void* recvThread(void* arg) {
	int sd = *( (int*) arg );
	char buf[BUFSIZE];

	printf("connected (sd: %d)\n", sd);
	while (true) {
		ssize_t res = recv(sd, buf, BUFSIZE - 1, 0);
		if (res == -1 || res == 0) {
			perror("recv");
			break;
		}
		buf[res] = '\0';
		printf("%s\n", buf);

		if (broadcast = true) {
			for (int i = 0; i < sdlist.end; i++) {
				res = send(*(sdlist.root + i), buf, res, 0);
				if (res == -1 || res == 0) {
					perror("send");
					break;
				}
			}
		} else if (echo = true) {
			res = send(sd, buf, res, 0);
			if (res == -1 || res == 0) {
				perror("send");
				break;
			}
		}
	}
	printf("disconnected (sd: %d)\n", sd);

	int idx;
	for (idx = 0; idx < sdlist.end; idx++) {
		if ( *(sdlist.root + idx) == sd ) break;
	}

	remove(&sdlist, idx);
	close(sd);
	return NULL;
}

int parse(int argc, char* argv[]) {
	if (argc < 2 || argc > 4) return false;

	port = atoi(argv[1]);
	for (int i = 2; i < argc; i++) {
		if (strcmp(argv[i], "-e") == 0) echo = true;
		else if (strcmp(argv[i], "-b") == 0) broadcast = true;
		else return false;
	}
	return ( (port != 0) ? true : false );
}

int main(int argc, char* argv[]) {
	if (parse(argc, argv) == false) {
		usage();
		exit(1);
	}

	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("socket");
		exit(1);
	}

	int optval = 1;
	int res1 = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (res1 == -1) {
		perror("setsockopt");
		exit(1);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	ssize_t res2 = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
	if (res2 == -1) {
		perror("bind");
		exit(1);
	}

	int res3 = listen(sd, 5);
	if (res3 == -1) {
		perror("listen");
		exit(1);
	}

	init(&sdlist);

	while (true) {
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);
		int cli_sd = accept(sd, (struct sockaddr*)&cli_addr, &len);
		if (cli_sd == -1) {
			perror("accept");
			exit(1);
		}
		add(&sdlist, cli_sd);
		
		pthread_t p;
		Pthread_create(&p, NULL, recvThread, (void*)&cli_sd);
	}
	close(sd);
	free(&sdlist);
	return 0;
}

