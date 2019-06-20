#include "wrap.h"
#include<fcntl.h>
#include<sys/epoll.h>
#include <time.h>
 
#define MAXLINE 1024
#define SERV_PORT 9877
#define MAXEVENTS 256

int max(int a, int b)
{
	if(a >= b)
		return a;
	else
		return b;
}

void str_cli(FILE *fp, int sockfd)
{
	int s, n, i, j;
        ssize_t count;
        char buf[MAXLINE];
	int epollfd;
	epollfd = epoll_create1(0);
	if(epollfd == -1)
	        perr_exit("epoll_create1");

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	s = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
        if(s == -1)
                perr_exit("epoll_ctl");

        struct epoll_event events[MAXEVENTS];

	memset(buf, 0, MAXLINE);
        strcpy(buf,"hello,socket world!");
	
        //clock_t start, stop;
        // clock_t 是 clock() 函数返回的变量类型
        //double duration = 0;
	struct timeval t1,t2;

	gettimeofday(&t1,NULL);
	//start = clock();
	for(j = 0; j < 10000; j++)
	{
		write(sockfd, buf, strlen(buf));
		n = epoll_wait(epollfd, events, MAXEVENTS, -1);
		if ( n == -1 )
			perr_exit( "epoll_wait" );
		for(i = 0; i < n; i++)
		{
			if(sockfd == events[i].data.fd)
			{
				count = read(events[i].data.fd, buf, MAXLINE);
				//write(events[i].data.fd, buf, strlen(buf));
			}
		}

	}
	gettimeofday(&t2,NULL);
	long deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
	printf("time is %ld\n",deltaT);
	//stop = clock();
	//duration = (double)(stop - start)/CLOCKS_PER_SEC;
	//printf("duration:%f\n",duration);
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc != 2)
	{
		printf("usage: tcpcli <IPaddress>");
		exit(1);
	}

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd);
	close(sockfd);
	return 0;
}
