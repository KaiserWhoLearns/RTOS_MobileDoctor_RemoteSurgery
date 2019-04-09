#include <stdio.h>
#include <unistd.h>
//main

int main(void)
{
     int time1 = 0, time2 = 0;
     printf("Please enter two numbers:");
     scanf("%d%d", &time1, &time2);
     for(;;) {
        if(time1 <= time2){
        printf("A C\n");
        sleep(time1);
        printf("A B C D\n");
        sleep(time2-time1);
      } else {
        printf("A C\n");
        sleep(time1);
        printf("A B C D\n");
        sleep(time1-time2);
       }
      return 0;   

      }
    return 0;
}
