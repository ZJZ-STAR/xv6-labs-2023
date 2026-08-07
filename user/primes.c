#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
sieve(int left)
{
    int n;
    if (read(left, &n, sizeof(n)) != sizeof(n)) {
        close(left);
        exit(0);
    }
    printf("prime %d\n",n);

    int p[2];
    pipe(p);

    int pid = fork();
    if(pid == 0)
    {
        close(left);
        close(p[1]);
        sieve(p[0]);
        exit(0);
    }else
    {
        close(p[0]);

        int num;
        while(read(left, &num, sizeof(num)) == sizeof(num)){
            if(num % n != 0){
                write(p[1], &num, sizeof(num));
            }
        }
        close(left);
        close(p[1]);

        wait(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    int pid = fork();
    if(pid == 0)
    {
        close(p[1]);
        sieve(p[0]);
        exit(0);
    }else
    {
        close(p[0]);
        for(int i=2; i<=35; i++)
        {
            write(p[1],&i,sizeof(i));
        }
        close(p[1]);

        wait(0);
        exit(0);
    }

}
