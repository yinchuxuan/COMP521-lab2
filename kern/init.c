#include <unistd.h>
#include <comp421/yalnix.h>

int
main()
{
    write(2, "init!\n", 6);
    printf("this is process %d\n", GetPid());
    Delay(5);
    printf("init process is back!\n");
    Delay(3);
    printf("init process is back!\n");
    Delay(6);
    printf("init process is back!\n");
    while(1);
}