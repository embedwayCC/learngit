#include "wrap.h"
 
void perr_exit(const char *s)
{
 	perror(s);
	exit(1);
}

int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
	int newfd;
again:
	if((newfd = accept(sockfd, cliaddr, addrlen)) < 0)
	{
		if((errno == ECONNABORTED) || (errno == EINTR))
		{
			goto again;
		}
		else
		{
			perr_exit("accept");
		}
	}
	return newfd;
}

void Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen)
{
	if(bind(sockfd, myaddr, addrlen) < 0)
	{
		perr_exit("bind");
	}
}

void Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
	if(connect(sockfd, servaddr, addrlen) < 0)
	{
		perr_exit("connect");
	}
}

void Listen(int sockfd, int backlog)
{
	if(listen(sockfd, backlog) < 0)
	{
		perr_exit("listen");
	}
}

int Socket(int family, int type, int protocol)
{
	int socketfd;
	if((socketfd = socket(family, type, protocol)) < 0)
	{
		perr_exit("socket");
	}
	return socketfd;
}

ssize_t Writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char* ptr;
			 
	ptr = vptr;
	nleft = n;
	while(nleft > 0)
	{
		if((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if(nwritten < 0 && errno == EINTR)
			{
				nwritten = 0;
			}
			else	
			{
				return -1;
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

ssize_t my_read(int fd, char *ptr)
{
	static int read_cnt;
	static char *read_ptr;
	static char read_buf[100];

	//printf("in my_read\n");
	
	if(read_cnt <= 0)
	{
again:
		if((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)
		{
			if(errno == EINTR)
			{
				goto again;
			}
			else
			{
				return -1;
			}
		}
		else if(read_cnt == 0)
		{
			return 0;
		}
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return 1;
}

ssize_t Readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;

	//printf("maxlen=%d\n",maxlen);
	for(n = 1; n < maxlen; n++)
	{
		if((rc = my_read(fd, &c)) == 1)
		{
			*ptr++ = c;
			if(c == '\n')
			{
				//printf("n=%d\n",n);
				break;
			}
		}																																																			else if(rc == 0)
		{
			*ptr = 0;
			return n - 1;
		}
		else
		{
			return -1;
		}
	}
	*ptr = 0;
	return n;	
}

void Close(int sockfd)
{
	if(close(sockfd) == -1)
	{
		perr_exit("close");
	}
}

void Fputs(char *ptr, FILE *stream)    
{    
    if (fputs(ptr, stream) == EOF)
		perr_exit("fputs");    
}

char *Fgets(char *ptr, int n, FILE *stream)    
{    
    char *rptr = fgets(ptr, n, stream);
	if ( rptr == NULL && ferror(stream) )
		perr_exit("fgets");
	return rptr;    
}
