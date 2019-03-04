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

#define MAX_BUFFER_SIZE 5
#define MAX_EPOLL_EVENTS 20
#define EPOLL_LT 0
#define EPOLL_ET 1
#define FD_BLOCK 0
#define FD_NONBLOCK 1

int set_nonblock(int fd)
{
	int old_flags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, old_flags | O_NONBLOCK);
	return old_flags;
}

void addfd_to_epoll(int epoll_fd, int fd, int epoll_type, int block_type)
{
	struct epoll_event ep_event;
	ep_event.data.fd = fd;
	ep_event.events = EPOLLIN;
	
	if (epoll_type == EPOLL_ET)
		ep_event.events |= EPOLLET;

	if (block_type == FD_NONBLOCK)
		set_nonblock(fd);
		
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ep_event);
}

void epoll_lt(int sockfd)
{
	char buffer[MAX_BUFFER_SIZE];
	int ret;
	
	memset(buffer, 0, MAX_BUFFER_SIZE);
	printf("开始recv()...\n");
	ret = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
	printf("ret = %d\n", ret);
	if (ret > 0)
		printf("收到消息:%s, 共%d个字节\n", buffer, ret);
	else
	{
		if (ret == 0)
			printf("客户端主动关闭！！！\n");
		close(sockfd);
	}
	
	printf("LT处理结束！！！\n");
}

void epoll_et_loop(int sockfd)
{
	char buffer[MAX_BUFFER_SIZE];
	int ret;

	printf("带循环的ET读取数据开始...\n");
	while (1)
	{
		memset(buffer, 0, MAX_BUFFER_SIZE);
		ret = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
		if (ret == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				printf("循环读完所有数据！！！\n");
				break;
			}
			close(sockfd);
			break;
		}
		else if (ret == 0)
		{
			printf("客户端主动关闭请求！！！\n");
			close(sockfd);
			break;
		}
		else
			printf("收到消息:%s, 共%d个字节\n", buffer, ret);

	}
	printf("带循环的ET处理结束！！！\n");
}

void epoll_et_nonloop(int sockfd)
{
	char buffer[MAX_BUFFER_SIZE];
	int ret;

	printf("不带循环的ET模式开始读取数据...\n");
	memset(buffer, 0, MAX_BUFFER_SIZE);
	ret = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
	if (ret > 0)
	{
		printf("收到消息:%s, 共%d个字节\n", buffer, ret);
	}
	else
	{
		if (ret == 0)
			printf("客户端主动关闭连接！！！\n");
		close(sockfd);
	}
	printf("不带循环的ET模式处理结束！！！\n");
}

void epoll_process(int epollfd, struct epoll_event *events, int number, int sockfd, int epoll_type, int block_type)
{
	struct sockaddr_in client_addr;
	socklen_t client_addrlen;
	int newfd, connfd;
	int i;
	
	for (i = 0; i < number; i++)
	{
		newfd = events[i].data.fd;
		if (newfd == sockfd)
		{
			//while(1){
			printf("=================================新一轮accept()===================================\n");
			printf("accept()开始...\n");

			/* 休眠3秒，模拟一个繁忙的服务器，不能立即处理accept连接 */
			//printf("开始休眠3秒...\n");
			//sleep(3);
			//printf("休眠3秒结束！！！\n");

			client_addrlen = sizeof(client_addr);
			connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addrlen);
			printf("connfd = %d\n", connfd);

			//if(connfd == -1)
			//{
			//	if((errno == EAGAIN) || (errno == EWOULDBLOCK))
			//		break;
			//}
			
			addfd_to_epoll(epollfd, connfd, epoll_type, block_type);
			printf("accept()结束！！！\n");
			//}
		}
		else if (events[i].events & EPOLLIN)
		{
			if (epoll_type == EPOLL_LT)    
			{
				printf("============================>水平触发开始...\n");
				epoll_lt(newfd);
			}
			else if (epoll_type == EPOLL_ET)
			{
				printf("============================>边缘触发开始...\n");

				/* 带循环的ET模式 */
				epoll_et_loop(newfd);

				/* 不带循环的ET模式 */
				//epoll_et_nonloop(newfd);
			}
		}
		else
			printf("其他事件发生...\n");
	}
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

	int sockfd, epollfd, number;

	sockfd = create_socket(argv[1], atoi(argv[2]));
	struct epoll_event events[MAX_EPOLL_EVENTS];

	clock_t start, stop;
	// clock_t 是 clock() 函数返回的变量类型
	double duration, total = 0;

	if ((epollfd = epoll_create1(0)) == -1)
		err_exit("epoll_create1() error");

	/* sockfd：非阻塞的LT模式 */
	addfd_to_epoll(epollfd, sockfd, EPOLL_LT, FD_NONBLOCK);

	/* sockfd：非阻塞的ET模式 */
	//addfd_to_epoll(epollfd, sockfd, EPOLL_ET, FD_NONBLOCK);
	
	while (1)
	{
		number = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
		if (number == -1)
			err_exit("epoll_wait() error");
		else
		{
			start = clock();
			/* 以下的LT，ET，以及是否阻塞都是是针对accept()函数返回的文件描述符，即函数里面的connfd */

			/* connfd:阻塞的LT模式 */
			//epoll_process(epollfd, events, number, sockfd, EPOLL_LT, FD_BLOCK);

			/* connfd:非阻塞的LT模式 */
			//epoll_process(epollfd, events, number, sockfd, EPOLL_LT, FD_NONBLOCK);

			/* connfd:阻塞的ET模式 */
			//epoll_process(epollfd, events, number, sockfd, EPOLL_ET, FD_BLOCK);

			/* connfd:非阻塞的ET模式 */
			epoll_process(epollfd, events, number, sockfd, EPOLL_ET, FD_NONBLOCK);
			stop = clock();
			duration = (double)(stop - start);
			printf("%f\n",duration);
			total = total + duration;
			printf("total:%f=====\n",total);  
		}
	}
	//printf("total:%f=====\n",total);
	close(sockfd);
	return 0;
}
