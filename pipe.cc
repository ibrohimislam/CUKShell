// reference: http://unixwiz.net/techtips/remap-pipe-fds.html

// user views of pipe : http://www.usna.edu/Users/cs/aviv/classes/ic221/s14/lec/09/lec.html

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
	int	writepipe[2] = {-1,-1},
		readpipe [2] = {-1,-1};
	pid_t childpid;

	char *envp[] = {"HOME=/", "PATH=/bin:/usr/bin", "TZ=UTC0", 0};


	if ( pipe(readpipe) < 0  ||  pipe(writepipe) < 0 )
	{
		fprintf(stderr,"PIPE ERROR\n");
	}
	else
	{
		#define	PARENT_READ	readpipe[0]
		#define	CHILD_WRITE	readpipe[1]
		#define CHILD_READ writepipe[0]
		#define PARENT_WRITE writepipe[1]

		if ( fork() == 0 )	/* in the child */
		{
			close(PARENT_WRITE);
			close(PARENT_READ);

			dup2(CHILD_READ,  0);  close(CHILD_READ);
			dup2(CHILD_WRITE, 1);  close(CHILD_WRITE);

			char *argv[100];

			argv[0] = "/bin/ls";
			argv[1] = "-al";
			argv[2] = "/bin/";
			argv[3] = 0;

	    	execve(argv[0], argv, envp);
	    	_exit(0);
		}
		else
		{
			close(CHILD_READ);
			close(CHILD_WRITE);

			char s[1048576];
			wait(NULL);
			read(PARENT_READ, &s, 1048576);
			printf("Child: \n%s\n", s);
\
		}
	}

	return 0;
}
