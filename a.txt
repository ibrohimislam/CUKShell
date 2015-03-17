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
	std::string s;
	
	if (argc < 2)
		return 0;

	while (getline(std::cin, s))
		std::cout << argv[1] << s << std::endl;

	return 0;
}