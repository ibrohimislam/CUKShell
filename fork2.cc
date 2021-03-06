#include  <readline/history.h>
#include  <readline/readline.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <sys/stat.h>
#include  <termios.h>
#include  <unistd.h>

using namespace std;

#define   MAX_COUNT  200
#define   BUF_SIZE   100

char getch();

char* concat(char *a, char* end) {
	char *ret = (char*) malloc(strlen(a) + strlen(end) + 5);
	char *str = ret;
	while(*a) {
		*str = *a;
		a++; str++;
	}
	while(*end) {
		*str = *end;
		end++; str++;
	}
	*str = 0;
	return ret;
}

inline bool is_file_exists(const char* name) {
  struct stat buffer;   
  return (stat(name, &buffer) == 0); 
}

int main(int argc, char* argv[])
{
	pid_t  pid;

	char reserved[][100] = {"cd", "ll"};
	char* cmd;

	char* envp[] = {"HOME=/", "PATH=/bin:/usr/bin", "TZ=UTC0", 0};
	char* default_path[] = {"/usr/bin/", "/bin/"};

	while (1)
	{
    	char* curdir = (char*) malloc(256);
    	getcwd(curdir,256);

		curdir = concat(curdir, " > "); 
		cmd = readline(curdir);
		add_history(cmd);
		
		char* word = strtok(cmd, " ");

		if (strcmp(word, reserved[0])==0)
		{
			word = strtok(NULL, "");
			chdir(word);
		}
		else
		{
			pid = fork();

			if (pid == 0)
			{
				//sprintf(x, "/bin/%s", cmd);
				char *argv[100];

				argv[0] = (char*) malloc(256);
				strcpy(argv[0], word);

				int i = 0;
				word = strtok(NULL, " ");

				while(word != NULL) {
					i++;
					
					argv[i] = (char*) malloc(256);
					strcpy(argv[i],word);

					word = strtok(NULL, " ");
				}

				argv[i+1] = 0;

				execve(argv[0], argv, envp);

				//brute force tempat file
				char* exe = (char*) malloc(256);
				strcpy(exe, argv[0]);

				int len = sizeof(default_path)/sizeof(int);
				for (int i = 0; i<len; ++i)
				{
					argv[0] = concat(default_path[i],exe);
					execve(argv[0], argv, envp);
				}

		    	// jika execve gagal
		    	printf("%s: command not found\n", exe);
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
