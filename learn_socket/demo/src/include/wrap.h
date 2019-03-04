#ifndef WRAP_H
#define WRAP_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>

#define SA struct sockaddr

typedef void Sigfunc(int);
  
extern void perr_exit(const char *s);
extern int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
extern void Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);
extern void Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
extern void Listen(int sockfd, int backlog);
extern int Socket(int family,int type,int protocol);
//extern ssize_t Read(int fd,void *ptr,size_t nbytes);
//extern ssize_t Write(int fd,const void *ptr,size_t nbytes);
//extern ssize_t Readn(int fd,void *vptr,size_t n);
extern ssize_t Writen(int fd, const void *vptr, size_t n);
extern ssize_t my_read(int fd,char *ptr);
extern ssize_t Readline(int fd,void *vptr,size_t maxlen);
extern void Close(int sockfd);
extern void Fputs(char *ptr, FILE *stream);
extern char* Fgets(char *ptr, int n, FILE *stream);
extern Sigfunc *signal(int signo, Sigfunc *func);

#endif
