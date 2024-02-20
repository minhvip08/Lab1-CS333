#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" // MAXARG

int main(int argc, char *argv[])
{
  char reader;
  char buff[2048] = {0};
  char *p = buff;
  char *v[MAXARG];
  int c = 0;
  int blanks = 0;
  int offset = 0;

  if (argc <= 1)
  {
    exit(1);
  }

  for (c = 1; c < argc; c++)
  {
    v[c - 1] = argv[c];
  }
  c--;
  while (read(0, &reader, 1) > 0)
  {
    if (reader == ' ' || reader == '\t')
      blanks = 1;
    if (blanks)
    {
      buff[offset++] = '\0';
      v[c++] = p;
      p = buff + offset;
      blanks = 0;
    }
    if (reader != '\n')
      buff[offset++] = reader;
    else
    {
      v[c] = p;
      p = buff + offset;
      if (fork() == 0)
      {
        exec(v[0], v);
        exit(0);
      }
      wait(0);
      c = argc - 1;
    }
  }
  exit(0);
}