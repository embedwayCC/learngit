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

#define MAXLINE 80

int set_nonblock(int fd)
{
	int old_flags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, old_flags | O_NONBLOCK);
	return old_flags;
}

void err_exit(char *msg)
{
	perror(msg);
	exit(1);
}

int create_socket(const char *ip, const int port_number)
{
	struct sockaddr_in server_addr;
	int sockfd, reuse = 1;

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);

	if (inet_pton(PF_INET, ip, &server_addr.sin_addr) == -1)
		err_exit("inet_pton() error");

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		err_exit("socket() error");

	/* 设置复用socket地址 */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
		err_exit("setsockopt() error");

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		err_exit("bind() error");

	if (listen(sockfd, 5) == -1)
		err_exit("listen() error");

	return sockfd;
}

int main(int argc, const char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "usage:%s ip_address port_number\n", argv[0]);
		exit(1);
	}

	int listenfd, i, maxi, maxfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	fd_set rset, allset;
	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr, servaddr;
	char buf[MAXLINE];
	ssize_t n;
	char str[INET_ADDRSTRLEN];
	clock_t start, stop;
	double duration, total = 0;

	listenfd = create_socket(argv[1], atoi(argv[2]));

	maxfd = listenfd;
	maxi = -1;
	
	for(i = 0; i < FD_SETSIZE; i++)
	{
		client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	while(1)
	{
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

		if(nready < 0)
		{
			printf("select error!\n");
		}
		if(FD_ISSET(listenfd, &rset))
		{
			cliaddr_len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);
			printf("received from %s at port %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),ntohs(cliaddr.sin_port));
			for(i = 0; i < FD_SETSIZE; i++)
			{
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}
			}
			if(i == FD_SETSIZE)
			{
				fputs("too many clients\n", stderr);
				exit(1);
			}

			FD_SET(connfd, &allset);
			if(connfd > maxfd)
			{
				maxfd = connfd;
			}
			if(i > maxi)
			{
				maxi = i;
			}
			if(--nready == 0)
			{
				continue;
			}
		}
		start = clock();
		for(i = 0; i <= maxi; i++)
		{
			if((sockfd = client[i]) < 0)
			{
				continue;
			}
			if(FD_ISSET(sockfd, &rset))
			{
				if((n = read(sockfd, buf, 5)) == 0)
				{
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				else
				{
					/*int j;
					for(j = 0; j < n; j++)
					{
						buf[j] = toupper(buf[j]);
					}
					write(sockfd, buf, n);*/
					printf("收到消息:%s, 共%d个字节\n", buf, (int)n);
				}
				if(--nready == 0)
				{
					break;
				}
			}  
		}
		stop = clock();
		duration = (double)(stop - start);
		printf("%f\n",duration);
		total = total + duration;
		printf("total:%f=====\n",total);
	}
}
