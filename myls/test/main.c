#include<signal.h>
#include<execinfo.h>
#include<time.h>
#include<assert.h>

#include"ls.h"

#define TRACE_SIZE 512
void *tr_buffer[TRACE_SIZE];
#define APP_DBG_DIR "/home/debian/myls/test"

static char prog_name[256];

void fault_trap(int n, siginfo_t *siginfo, void *myact)
{
    int i, num;
    char **calls;
    struct tm *ptm;
    struct timeval tv;
    FILE *fd = NULL;
    struct stat f_s;
    char APP_DBG_FILE[256];
    struct sigaction act;

    assert(prog_name != NULL);
    sprintf(APP_DBG_FILE,"%s/%s-fault.txt",APP_DBG_DIR,prog_name);

    if (access(APP_DBG_FILE, F_OK) != 0) {
        mkdir(APP_DBG_DIR, 0755);
    }
    if (-1 != stat(APP_DBG_FILE, &f_s) && f_s.st_size > 1024000)
        fd = fopen(APP_DBG_FILE, "w+");
    else
        fd = fopen(APP_DBG_FILE, "a+");
    if (fd) {
        setenv("TZ", "UTC", 1);
        gettimeofday(&tv, NULL);
        ptm = localtime(&tv.tv_sec);
        fprintf(fd, "\nTime: %d-%02d-%02d %02d:%02d:%02d %s\n",
                (1900 + ptm->tm_year), (1 + ptm->tm_mon),
                ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, "UTC");

        fprintf(fd, "Siginfo: Signal Num: %d(n=%d)\n", siginfo->si_signo, n);
        fprintf(fd, "         Fault Addr: %p\n", siginfo->si_addr);
        fprintf(fd, "           Error No: %d\n", siginfo->si_errno);
        fprintf(fd, "               Code: %d\n", siginfo->si_code);
        fprintf(fd, "                Pid: %d\n", siginfo->si_errno);
        fprintf(fd, "                Uid: %d\n", siginfo->si_code);
        fprintf(fd, "             Status: %d\n", siginfo->si_status);

        num = backtrace(tr_buffer, TRACE_SIZE);
        calls = backtrace_symbols(tr_buffer, num);
        for (i = 0; i < num; i++)
            fprintf(fd, "   [%d]:%s\n", i,calls[i]);

        free(calls);
        fclose(fd);
    }

    //sigemptyset(&act.sa_mask);
    //act.sa_flags     = SA_SIGINFO;
    //act.sa_sigaction = SIG_DFL;

    //sigaction(n, &act, NULL);
    signal(n, SIG_DFL);
    raise(n);
}

void setup_fault_trap(void)
{
    struct sigaction act;
    
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = fault_trap;
    sigaction(SIGSEGV, &act, NULL);
    sigaction(SIGABRT, &act, NULL);
    sigaction(SIGILL, &act, NULL);
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);
	sigaction(SIGBUS, &act, NULL);
}

int main(int argc, char* argv[])
{
	//extern bool flags[PARAM_NUM];
	//int i;

	openlog("My_ls", LOG_CONS | LOG_PID, LOG_USER);
	
	strcpy(prog_name,argv[0]);
	setup_fault_trap();
	GetParam(argc, argv);
	/*for(i=0;i<PARAM_NUM;i++)
	{
		if(flags[i]==1)
			printf("%d  ",i);
	}*/

	closelog();
	return 0;
}
