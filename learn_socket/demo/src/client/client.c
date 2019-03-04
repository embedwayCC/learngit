#include "wrap.h"
 
#define MAXLINE 1024
#define SERV_PORT 9877

int max(int a, int b)
{
	if(a >= b)
		return a;
	else
		return b;
}

void str_cli(FILE *fp, int sockfd)
{
	/*char sendline[MAXLINE], recvline[MAXLINE];
	
	while(Fgets(sendline, MAXLINE, fp) != NULL)
	{
		Writen(sockfd, sendline, strlen(sendline));
		if(Readline(sockfd, recvline, MAXLINE) == 0)
		{
			printf("str_cli: server terminated prematurely\n");
			exit(1);
		}
		Fputs(recvline,stdout);
	}*/

	int maxfdpl;
	fd_set rset;
	char sendline[MAXLINE], recvline[MAXLINE];
	int s;
	ssize_t t;

	//printf("%d\n",sockfd);

	FD_ZERO(&rset);
	for( ; ; )
	{
		//memset(sendline, 0, MAXLINE);
		//memset(recvline, 0, MAXLINE);
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdpl = max(fileno(fp), sockfd) + 1;
		select(maxfdpl, &rset, NULL, NULL, NULL);
		
		if(FD_ISSET(sockfd, &rset))
		{
			s = read(sockfd, recvline, MAXLINE);
			if(s == 0)
			{
				printf("str_cli: server terminated prematurely\n");
				close(sockfd);
				break;
			}
			printf("%d----\n",s);
			//Fputs(recvline,stdout);
			write(1,recvline,s);
			memset(recvline, 0, MAXLINE);
		}
		if(FD_ISSET(fileno(fp), &rset))
		{
			if(Fgets(sendline, MAXLINE, fp) == NULL)
				return;
			s = write(sockfd, sendline, strlen(sendline));
			printf("%d===\n",s);
			memset(sendline, 0, MAXLINE);
		}
	}
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

	//printf("111111111\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	//printf("22222222222222\n");
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	//printf("33333333333333333333\n");
	Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
	//printf("++++++++++++++++\n");
	str_cli(stdin, sockfd);
	exit(0);
}
