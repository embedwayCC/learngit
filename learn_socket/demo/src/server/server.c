#include "wrap.h"
#include<fcntl.h>
#include<sys/epoll.h>

#define SERV_PORT 9877
#define MAXLINE 1024
#define LISTENQ 1024
#define MAXEVENTS 256

/*void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];

again:
	while((n = read(sockfd, buf, MAXLINE)) > 0)
		Writen(sockfd, buf, n);

	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		printf("str_echo: read error\n");
}

void sig_chld(int signo)
{
	pid_t pid;
	int stat;
			
	while((pid=waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated.\n",pid);
	return;
}
Sigfunc *Signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;
		
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo==SIGALRM)
	{
#ifdef SA_INTERRUPT
		act.sa_flags|=SA_INTERRUPT;
#endif
	}else{
#ifdef SA_RESTART
		act.sa_flags|=SA_RESTART;
#endif
	}
	if(sigaction(signo,&act,&oact)<0)
		return(SIG_ERR);
	return(oact.sa_handler);
}*/

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
	/*int listenfd, connfd;
	struct sockaddr_in cliaddr, servaddr;
	socklen_t clilen;
	pid_t childpid;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	// 解决在close之后会有一个WAIT_TIME，导致bind失败的问题
	int val = 1;
	int ret = setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(void *)&val,sizeof(int));
	if(ret == -1)
	{
		printf("setsockopt\n");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	//system("netstat -a | grep 9877 >> server.txt");

	Signal(SIGCHLD, sig_chld);

	printf("into server\n");

	for( ; ; )
	{
		//printf("LISTEN!!!!!");
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
		//system("netstat -a | grep 9877 >> server.txt");
		if((childpid = fork()) == 0)
		{
			Close(listenfd);
			str_echo(connfd);
			//system("netstat -a | grep 9877 >> server.txt");
			exit(0);
		}
		Close(connfd);
	}*/

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
						s = write(1,buf,count);
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
					printf("buf after loop=%s\n",buf);
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
