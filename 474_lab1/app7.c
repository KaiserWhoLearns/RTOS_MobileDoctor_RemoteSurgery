#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void display(int time1, int time2);

int main(void) {
    int time1 = 0, time2 = 0;
    printf("Please enter two numbers:");
    scanf("%d%d", &time1, &time2);
    display(time1, time2);
    return 0;
}

void display(int time1, int time2) {
    if(time2 % time1 != 0) {
    //specifiy that time1 != time2
   while(1) {
      for (int i = 0; i < time2-time1-1; i ++) {
        system("clear");
        for (int j = 0; j < time1-1; j ++) {
          sleep(1);
        }
        printf("A B\n");
        sleep(1);
      }
      system("clear");
      printf("C D\n");
      sleep(1);
    }
  } else if(time1 == time2){
    while(1) {
    //When time1 = time2
        printf("%s","A B C D\n");
        // must need new line character; because the string is first print to a buffer.
        sleep(1);
        system("clear");
        sleep(time1);
    }
      } else {
        while(1) {
        int i = 1;
        while(time1 * i != time2) {
          system("clear");
          sleep(time1 - 1);
          printf("%s", "A B\n");
          sleep(1);
          i++;
        }
        system("clear");
        printf("A B C D\n");
        sleep(1);
        system("clear");
      }
    }
}