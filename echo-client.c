# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <arpa/inet.h>
# include <sys/socket.h>

# include "Pthread.h"

# ifndef __BOOLEAN
# define __BOOLEAN

# define true 1
# define false 0

# endif
# define BUFSIZE 66536

int port = 0;
struct in_addr ip;

void usage(void) {
	printf("syntax : echo-client <ip> <port>\n");
	printf("sample : echo-client 192.168.10.2 1234\n");
}

void* recvThread(void* arg) {
	int sd = *( (int*) arg );
	char buf[BUFSIZE];
	
	printf("connected\n");
	while (true) {
		ssize_t res = recv(sd, buf, BUFSIZE - 1, 0);
		if (res == -1 || res == 0) {
			perror("recv");
			break;
		}
		buf[res] = '\0';
		printf("%s\n", buf);
	}
	printf("disconnected\n");
	close(sd);
	exit(0);
	return NULL;
}

int parse(int argc, char* argv[]) {
        if (argc != 3) return false;

        memset(&ip, 0, sizeof(struct in_addr));
        int res = inet_pton(AF_INET, argv[1], &ip);
        if (res == -1 || res == 0) {
                perror("inet_ptons");
                return false;
        }
        port = atoi(argv[2]);
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

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port= htons(port);
	addr.sin_addr = ip;
	memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));


	int res = connect(sd, (struct sockaddr*)&addr, sizeof(addr));
	if (res == -1) {
		perror("connect");
		return -1;
	}

	pthread_t p;
	Pthread_create(&p, NULL, recvThread, (void*)&sd);
	
	char buf[BUFSIZE];
	while (true) {
		fgets(buf, BUFSIZE, stdin);
		buf[strlen(buf) - 1] = '\0';
		ssize_t res = send(sd, buf, strlen(buf), 0);
		if (res == -1 || res == 0) {
			perror("send");
			break;
		}
	}
	close(sd);
	return 0;
}

