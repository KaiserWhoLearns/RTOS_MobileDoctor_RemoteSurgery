#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//main

int main(void)
{
    printf("A B C D\n");
    sleep(1);
    system("clear");
    printf("B C D\n");
    sleep(1);
    system("clear");
    printf("C D\n");
    sleep(1);
    system("clear");
    printf("D\n");
    sleep(1);
    system("clear");
    printf("\n");
    return 0;

}

void app2()
{
    //const
    char s[] = "A B C D";
    int i;
    int l = strlen(s);
    //char sub[7] = {0};
    for (i = 0; i < l; i = i + 2) {
        
        printf("%.*s",l-i, s+i);
        Sleep(500);
        system("cls");
        Sleep(500);
    }

}
