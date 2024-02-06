#include <stdio.h>
#include <stdlib.h>

int first(int a, int b){
    return a + b;
}

int second(int a, int b){
    return a - b;
}

int third(int a, int b){
    return a * b;
}

int main(void){
    int (*arr[3])(int, int) = {first, second, third};

    printf("%d \n", (*arr[0])(1, 4));

    return 0;
}