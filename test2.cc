#include  <iostream>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <sys/stat.h>
#include  <termios.h>
#include  <unistd.h>

using namespace std;

int main()
{
	int n = 4;
	char* envp[] = {(char*)"HOME=/", (char*)"PATH=/bin:/usr/bin", (char*)"TZ=UTC0", 0};

	char* argv[4][3] = {{(char*)"./gen", 0, 0},{(char*)"./test", (char*)"2", 0},{(char*)"./test", (char*)"3", 0},{(char*)"./test", (char*)"4", 0}};
	int	mypipe[4][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
	pid_t pid[4];

	printf("%d\n", n);


	for (int i = 0; i < n; ++i)
	{
		if (i<n-1)
			pipe(mypipe[i]);

		pid[i] = fork();

		if (pid[i]==0)
		{
			if (i>0)
			{
				close(mypipe[i-1][1]);
				dup2(mypipe[i-1][0], 0);
				close(mypipe[i-1][0]);
			}
			
			if (i<n-1)
			{
				close(mypipe[i][0]);
				dup2(mypipe[i][1], 1);
				close(mypipe[i][1]);
			}

			execve(argv[i][0], argv[i], envp);
			printf("WOOOOII\n");
			_exit(0);
		}
		else
		{
			if (i<n-1)
			{
				close(mypipe[i][1]);
			}
		}
	}
	for (int i = 0; i < n; ++i)
	{
		waitpid(pid[i], NULL, 0);
	}

}