#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    char buf[512];
    int pid = fork();

    if (pid == 0) {
        write(p[1], "ping", 1);
        printf("%d: received ping\n", getpid());
      
    } else {
        write(p[1], "1", 1);
        wait(0);
        read(p[0], buf, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);



}
