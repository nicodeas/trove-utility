#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    switch (fork())
    {
    case 0:
        printf("hello from parent!\n");
        break;

    default:
        printf("hello from child!\n");
        break;
    }

    return 0;
}
