#include  <iostream>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <sys/stat.h>
#include  <termios.h>
#include  <unistd.h>


int main(int argc, char* argv[])
{
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "1" << std::endl;
	}
}