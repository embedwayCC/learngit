#include "wrap.h"
#include<fcntl.h>
#include<sys/epoll.h>

#define SERV_PORT 9877
#define MAXLINE 1024
#define LISTENQ 1024
#define MAXEVENTS 256

int make_socket_non_blocking (int sfd) 
{
	int flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1)
	{
		return -1;
	}
	if (fcntl(sfd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		return -1;
	}
	return 0;
}

int main(int argc, char** argv)
{
	int listenfd, connfd, epollfd;
	struct epoll_event ev;
	struct epoll_event events[MAXEVENTS];
	struct sockaddr_in cliaddr, servaddr;
	socklen_t clilen;
	int s, i, n;
	ssize_t count;
	char buf[MAXLINE];

	for(i=0;i<MAXEVENTS;i++)
	{
		memset(&events[i],0,sizeof(struct epoll_event));
	}
	memset(&ev,0,sizeof(struct epoll_event));

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
	
	s = make_socket_non_blocking(listenfd);
	if(s == -1)
		perr_exit("make_socket_non_blocking");

	Listen(listenfd, LISTENQ);

	epollfd = epoll_create1(0);
	if(epollfd == -1)
		perr_exit("epoll_create1");

	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;

	s = epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
	if(s == -1)
		perr_exit("epoll_ctl");

	while(1)
	{
		n = epoll_wait(epollfd, events, MAXEVENTS, -1);
		memset(buf,0,MAXLINE);
		if ( n == -1 )
			perr_exit( "epoll_wait" );
		for(i = 0; i < n; i++)
		{
			if(events[i].events & EPOLLIN)
			{
				if(listenfd == events[i].data.fd)
				{
					while(1)
					{
						clilen = sizeof(cliaddr);
						//connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
						connfd = accept(listenfd, (SA *)&cliaddr, &clilen);
						if(connfd == -1)
						{
							if((errno == EAGAIN) || (errno == EWOULDBLOCK))
								break;
							else
							{
								perror("accept");
								break;
							}
						}
						s = make_socket_non_blocking(connfd);
						if(s == -1)
							perr_exit("make_socket_non_blocking");
						ev.data.fd = connfd;
						ev.events = EPOLLIN | EPOLLET;
						s = epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev);
						if(s == -1)
							perr_exit("epoll_ctl");
					}
				}
				else
				{
					while(1)
					{
						//ssize_t count;
						//char buf[MAXLINE];
						//buf[MAXLINE]={0};
						count = read(events[i].data.fd, buf, MAXLINE);
						//length = count;
						if(count == -1)
						{
							if(errno == EAGAIN)
							{
								perror("read");
								//close(events[i].data.fd);
							}
							break;
						}
						else if(count == 0)
						{
							close(events[i].data.fd);
							printf("Closed connection on descriptor %d\n", events[i].data.fd);
							break;
						}
						//s = write(1,buf,count);
						printf("buf=%s\n",buf);
						if(s == -1)
						{
							perror("write");
							abort();
						}
						//printf("has read %d\n",count);
						//ev.events = EPOLLOUT | EPOLLET;
						//epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
					}
					write(events[i].data.fd, buf, strlen(buf));
					//ev.events = EPOLLOUT | EPOLLET;
					//epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
					//printf("buf after loop=%s\n",buf);
				}
			}
			/*else if (events[i].events & EPOLLOUT) 
			{
				if (events[i].data.fd != listenfd) 
				{
					//write(events[i].data.fd, "hello world!\n", 13);
					//printf("%ld  %ld\n",count,strlen(buf));
					printf("buf=%s\n",buf);
					//printf("size=%d\n",strlen(buf));
					write(events[i].data.fd, buf, strlen(buf));
					//memset(buf,0,MAXLINE);
					//write(events[i].data.fd, buf, sizeof(buf));
					ev.events = EPOLLET | EPOLLIN;
					epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
				}
			}*/
		}
	}
	close(listenfd);
	return 0;
}
