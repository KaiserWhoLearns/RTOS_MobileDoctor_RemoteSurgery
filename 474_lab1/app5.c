#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void f1Data(unsigned long delay1);
void f2Clear(unsigned long delay2);

int main(){
	int time1 = 0, time2 = 0;
    printf("Please enter two numbers:");
    scanf("%d%d", &time1, &time2);
	// assume time1 > time2

    while(1) {
		if(time2 % time1 != 0) {
		system("clear");
        f1Data(time1);
		f2Clear(time2-time1);
		printf("C D\n");
		sleep(1);
		}
	}
	return 0;
}

// the function that display and flash A B
void f1Data(unsigned long delay1) {
    sleep(delay1 - 1);
	printf("A B\n");
	sleep(1);
}

// the function that display and flash C D
void f2Clear(unsigned long delay2) {
	system("clear");
	sleep(delay2 - 1);
}
