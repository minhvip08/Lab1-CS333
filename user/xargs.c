#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  // MAXARG

#define is_blank(chr) (chr == ' ' || chr == '\t') 

int
main(int argc, char *argv[])
{
  // Declare and initialize variables
  char buf[2048] = {0}; // buffer to store input
  char ch; // character to read from input
  char *p = buf; // pointer to current position in buffer
  char *v[MAXARG]; // array to store arguments for command
  int c = 0; // counter for arguments
  int blanks = 0; // flag for blank characters
  int offset = 0; // offset for buffer

  // Check if there is at least one argument for command
  if(argc <= 1){
    exit(1);
  }

  // Copy arguments from argv to v, skipping the first one
  for (c = 1; c < argc; c++) {
    v[c-1] = argv[c];
  }
  c--; // decrement c to point to the last argument in v

  // Read input one character at a time
  while (read(0, &ch, 1) > 0) {
    // If character is blank, set blanks to 1 and skip it
    if (is_blank(ch)) {
      blanks = 1;
      continue;
    }

    // If there were blanks before, add a null terminator to buffer
    // and set the next argument in v to point to the current position in buffer
    // Reset blanks and p
    if (blanks) { 
      buf[offset++] = '\0';
      v[c++] = p;
      p = buf + offset;
      blanks = 0;
    }

    // If character is not a newline, add it to buffer
    if (ch != '\n') {
      buf[offset++] = ch;
    } 
    // If character is a newline, end of input line reached
    else {
      // Set the next argument in v to point to the current position in buffer
      // Reset p
      v[c++] = p;
      p = buf + offset;

      // Fork a child process and execute the command in v
      // Wait for the child process to finish
      // Reset c to point to the last argument in v
      if (fork() == 0) {
        exec(v[0], v);
        exit(0);
      }
      wait(0);
      c = argc - 1;
    }
  }

  // Exit the program
  exit(0);
}
