#include <stdlib.h>
#include <stdio.h>

int increment(void){
    static unsigned int counter = 0;
    counter++;
    //printf("%d ", counter);
    return counter;
}

int main(void){
    for (int i = 0; i<5; i++){
        int a = increment();
        printf("%d \n", a + 1);
    }
    return 0;
}