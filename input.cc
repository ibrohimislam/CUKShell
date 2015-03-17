#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

int main()
{
  char* command;

	while (1)
	{
    char* buffer = (char*) malloc(256);
    getcwd(buffer, 256);
		command = readline(buffer);

    
	}
}