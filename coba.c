#include <stdio.h>
#include <string.h>

int main() {
  char str[] = "Aku cinta kamu tapi galau";
  char* temp = strtok(str, " ");
  while(temp) 
    printf("%s\n", temp);
    temp = strtok(NULL, " ");
  }
  return 0;
}