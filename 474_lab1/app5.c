#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void f1Data(unsigned long time1, unsigned long time2);
void f2Clear(unsigned long delay2);

int main(){
	int time1 = 0, time2 = 0;
    printf("Please enter two numbers:");
    scanf("%d%d", &time1, &time2);
    f1Data(time1, time2);
	return 0;
}

// the function that display and flash A B
// pre: take in delay1, the shorter one;
// take in delay2, the longer one
void f1Data(unsigned long time1, unsigned long time2) {
    // 0 stand for did not print, 1 stand for printed;
    // if printed, then we clear the screen;
    int printab = 0;
    int printcd = 0;
    while(1) {
	for(int i = 0; i < time2; i = i + time1) {
        f2Clear(time1);
        if(printab == 0) {
          system("clear");
          printf("A C\n");
          printab = 1;
        } else {
          system("clear");
          printab = 0;
        }
        if(printcd == 1) {
          printf("B D\n");
        }
      }
      if(printcd == 0) {
        system("clear");
        printf("B D\n");
        printcd = 1;
      } else {
        system("clear");
        printcd = 0;
      }
    }
}

// the function that display and flash C D
void f2Clear(unsigned long delay2) {
	sleep(delay2);
	system("clear");
}
