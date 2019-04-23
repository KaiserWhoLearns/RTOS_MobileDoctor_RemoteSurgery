#include<stdio.h>

typedef struct
{
    int* tryPtr;
}tempStruct;

int try = 0;
int* tryPtr = &try;





int main() {
    tempStruct temp = {tryPtr};
    tempStruct* tempPtr = &temp;
    tempStruct anotherTemp = *tempPtr;
    int t = *(anotherTemp.tryPtr);
    printf("%d\n", t);
    *(anotherTemp.tryPtr) += 2;
    t = *(anotherTemp.tryPtr);
    printf("%d\n", t);
    printf("%d\n", *(temp.tryPtr));
    return 0;    
}