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
        read(p[0], buf, 4);
        close(p[0]);
        
        printf("%d: received %s\n", getpid(), buf);
        write(p[1], "pong", 4);
        close(p[0]);
      
    } else {
        write(p[1], "ping", 4);
        close(p[1]);
        wait(0);
        read(p[0], buf, 4);
        close(p[0]);
        printf("%d: received %s\n", getpid(), buf);
    }
    exit(0);



}
