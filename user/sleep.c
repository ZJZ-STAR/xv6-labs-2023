#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2)
    {
        write(2, "Usage: sleep <seconds>\n", 24);
        exit(1);
    }

    int triks=atoi(argv[1]);

    sleep(triks);

    exit(0);
}
