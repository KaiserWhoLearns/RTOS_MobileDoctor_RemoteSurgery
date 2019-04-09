#include <stdio.h>
#include <unistd.h>
//main

int main(void)
{
    printf("A B C D\n");
    sleep(1);
    printf("B C D\n");
    sleep(1);
    printf("C D\n");
    sleep(1);
    printf("D\n");
    sleep(1);
    printf("\n");
    return 0;

}
