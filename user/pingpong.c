#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p2c[2];
    int c2p[2];

    pipe(p2c);
    pipe(c2p);

    int pid = fork();
    if(pid == 0)
    {
        close(p2c[1]);
        close(c2p[0]);

        char buf;

        read(p2c[0],&buf,1);
        printf("%d: received ping\n",getpid());
        write(c2p[1],&buf,1);

        close(p2c[0]);
        close(c2p[1]);
    }else{
        close(p2c[0]);
        close(c2p[1]);

        char buf = 'a';

        write(p2c[1],&buf,1);

        read(c2p[0],&buf,1);
        printf("%d: received pong\n",getpid());

        close(p2c[1]);
        close(c2p[0]);
    }
    exit(0);

}
