#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void f1Data(unsigned long delay1);
void f2Clear(unsigned long delay2);
int printab = 0, printcd = 0;

int main(){
	int time1 = 0, time2 = 0;
    printf("Please enter two numbers:");
    scanf("%d%d", &time1, &time2);
	// assume time1 > time2
	int times = time2/time1;
	while(1) {
		for(int i = 0; i < times; i ++) {
			f1Data(time1);
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
	return 0;
}

// the function that display and flash A B
void f1Data(unsigned long delay1) {
	f2Clear(delay1);
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

// the function that display and flash C D
void f2Clear(unsigned long delay2) {
	sleep(delay2);
	system("clear");
}
