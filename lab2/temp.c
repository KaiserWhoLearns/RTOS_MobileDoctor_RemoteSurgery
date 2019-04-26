#include<stdio.h>

typedef struct
{
    int* tryPtr;
}tempStruct;

int try = 0;
int* tryPtr = &try;


void intToChar(char* arr, int val);


int main() {
    // tempStruct temp = {tryPtr};
    // tempStruct* tempPtr = &temp;
    // tempStruct anotherTemp = *tempPtr;
    // int t = *(anotherTemp.tryPtr);
    // printf("%d\n", t);
    // *(anotherTemp.tryPtr) += 2;
    // t = *(anotherTemp.tryPtr);
    // printf("%d\n", t);
    // printf("%d\n", *(temp.tryPtr));
    // char tem[3];
    // // tem = String(123);
    // intToChar(tem, 12);
    // printf("%s", tem);
    int temp = 10;
    int temp2 = 4;
    int* tempPtr = &temp;
    printf("%d", temp);
    *(tempPtr) = temp2;
    printf("%d", temp);
    return 0;    
}

void intToChar(char* arr, int val) {
    arr[0] = (char) (val / 100 + 48);
    arr[1] = (char) (val % 100 / 10 + 48);
    arr[2] = (char) (val % 10 + 48);
    return;
}