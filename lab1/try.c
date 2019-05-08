#include<stdio.h>
#include<unistd.h>
int main(void) {

	for(int i = 0; i < 10; i ++) {
	//fflush(stdout);
        printf("A B C D",i);
	sleep(2);
	fflush(stdout);
	}
	printf("Do what you want.");
	return 0;
}
