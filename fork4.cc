#include  <fcntl.h>
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
  pid_t  pid[256];

  char reserved[][100] = {"cd", "ll"};
  int mypipe[256][2];

  char* long_cmd;

  char* envp[] = {(char*)"HOME=/", (char*)"PATH=/bin:/usr/bin", (char*)"TZ=UTC0", 0};
  char* default_path[] = {(char*)"/usr/bin/", (char*)"/bin/"};

  while (1)
  {
    memset(mypipe, 255, sizeof mypipe);

  	char* curdir = (char*) malloc(256);
  	getcwd(curdir,256);

    curdir = concat(curdir, (char*)" > ");

    long_cmd = readline(curdir);
    add_history(long_cmd);

    char* cmd[256];

    int n = 0;
    cmd[n] = strtok(long_cmd, "|");
    while (cmd[n] != NULL){
      cmd[++n] = strtok(NULL, "|");
    }

    for (int iii = 0; iii < n; ++iii)
    {
      if (iii<n-1)
        pipe(mypipe[iii]);

      char* word = strtok(cmd[iii], " ");

      if (strcmp(word, reserved[0])==0)
      {
        word = strtok(NULL, "");
        chdir(word);
      }
      else
      {
        pid[iii] = fork();

        if (pid[iii] == 0)
        {
          //sprintf(x, "/bin/%s", cmd);
          char *argv[100];
          char *in = 0;
          char *out = 0;

          argv[0] = (char*) malloc(256);
          strcpy(argv[0], word);

          int i = 0;
          word = strtok(NULL, " ");
          while(word != NULL) {
            

            if(word[0] == '>') {
              if(strlen(word) == 1) {
                word = strtok(NULL, " ");
                if(word == NULL) {
                  printf("cuk! syntax error near unexpected token `newline'\n");
                  _exit(0);
                }
                out = (char*) malloc(256);
                strcpy(out, word);
              } else {
                out = (char*) malloc(256);
                strcpy(out, word + 1);
              }
            } 
            else if(word[0] == '<') {
              if(strlen(word) == 1) {
                word = strtok(NULL, " ");
                if(word == NULL) {
                  printf("cuk! syntax error near unexpected token `newline'\n");
                  _exit(0);
                }
                in = (char*) malloc(256);
                strcpy(in, word);
              } else {
                in = (char*) malloc(256);
                strcpy(in, word + 1);
              } 
            }
            else {
              i++;
              argv[i] = (char*) malloc(256);
              strcpy(argv[i],word);
            }

            word = strtok(NULL, " ");
          }

          argv[i+1] = 0;

          if (iii>0)
          {
            close(mypipe[iii-1][1]);
            dup2(mypipe[iii-1][0], 0);
            close(mypipe[iii-1][0]);
          }
          
          if (iii<n-1)
          {
            close(mypipe[iii][0]);
            dup2(mypipe[iii][1], 1);
            close(mypipe[iii][1]);
          }

          if (in != 0)
          {
            printf("%s\n", in);
            int fin = open(in, O_RDWR | O_CREAT, 0666);
            dup2(fin, 0);
            close(fin);
          }

          if (out != 0)
          {
            printf("%s\n", out);
            int fout = open(out, O_RDWR | O_CREAT, 0666);
            dup2(fout, 1);
            close(fout);
          }

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
          if(exe[0] == '<' || exe[0] == '>') {
            printf("cuk! syntax error near unexpected token `newline'\n");
            _exit(0);
          }
            printf("%s: command not found\n", exe);
          _exit(0);
        }
        else
        {
          if (iii<n-1)
          {
            close(mypipe[iii][1]);
          }
        }
      }
    }
    for (int iii = 0; iii < n; ++iii)
    {
      waitpid(pid[iii], NULL, 0);
    }
  }

  return 0;
}