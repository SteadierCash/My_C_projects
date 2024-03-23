#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Program usage: ./image <file_name>");
        return 1;
    }

     FILE* image = fopen(argv[1], "r");

     if (!image)
     {
        printf("Cannot open this file");
        return 1;
     }

     
}