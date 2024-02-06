#include <stdio.h>
#include <stdlib.h>

int main(void){
    if (puts("Hello, word!") == EOF){
        return EXIT_FAIURE;
    }
    return EXIT_SUCCESS;
}

