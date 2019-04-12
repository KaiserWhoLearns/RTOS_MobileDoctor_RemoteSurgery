#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void display(char ch, int time);

int main(void) {
    int time = 0;
    char ch;
    // input character and time
    printf("Please enter a character and an integer time:");
    scanf("%c%d", &ch, &time);
    display(ch, time);
    return 0;
}

// pre: take in the character to display, and time that it flashes
// assume that time > 0
void display(char ch, int time) {
    while(1) {
      sleep(time);
      printf("%c\n", ch);
      sleep(time);
      system("clear");
    }
}