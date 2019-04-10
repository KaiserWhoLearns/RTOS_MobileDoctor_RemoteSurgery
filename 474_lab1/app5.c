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
			while(1) {
		         for(int j = 0; j < time2 - time1 - 1; j ++) {
		        f2Clear(time1);
		        }
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
	f1Data(delay2);
}
