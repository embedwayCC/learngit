#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <time.h>

#define SERV_PORT 9877
#define SA struct sockaddr
#define MAXLINE 20

int main(int argc, char **argv)
{
	int sockfd, i;
	struct sockaddr_in servaddr;
	char sendline[MAXLINE];

	if(argc != 2)
	{
		printf("usage: tcpcli <IPaddress>");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);

	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

	memset(sendline, 0, MAXLINE);
	strcpy(sendline,"hello,server!");
	for(i = 0; i < 50; i++)
		write(sockfd, sendline, strlen(sendline));

	exit(0);
}
