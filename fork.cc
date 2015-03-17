#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <termios.h>
#include  <unistd.h>
using namespace std;

#define   MAX_COUNT  200
#define   BUF_SIZE   100

char getch();

int  main(int argc, char* argv[])
{
	pid_t  pid;

	char cd[][100] = {"cd", "ll"};
	char cmd[100];

	char *envp[] = {"HOME=/", "PATH=/bin:/usr/bin", "TZ=UTC0", 0};

    while (1)
    {

            char c;
            int i=0;
            char* curdir = (char*) malloc(256);
            getcwd(curdir,256);
            printf("%s > ", curdir); scanf("%s", cmd);
            if (strcmp(cmd, cd[0])==0)
            {
                    char x[100];
                    scanf("%s", x);
                    chdir(x);
            }
            else
            {
                    pid = fork();

                    if (pid == 0)
                    {
                            char x[100];
                            sprintf(x, "/bin/%s", cmd);
                            char *argv[100];
                            argv[0] = x;
                            argv[1] = 0;

                    execve(argv[0], argv, envp);
                            _exit(0);
                    }
                    else
                    {
                            wait(NULL);
                            //printf("END\n");
                    }
            }
    }

	return 0;
}

// char getch() {
// 	char buf = 0;

// 	system("stty raw"); 
// 	buf = getchar(); 
// 	system("stty cooked"); 

// 	return buf;
// }
