#include <stdio.h>

int main(int argc, char* argv[])
{
    // arguments check
    if (argc != 2)
    {
        printf("Program usage: ./image <file_name>");
        return 1;
    }

    FILE* image = fopen(argv[1], "rb");

    // check if file exists
    if (!image)
    {
    printf("Cannot open this file");
    return 1;
    }

    //check file format
    unsigned char marker[2];
    fread(marker, sizeof(unsigned char), 2 , image);
    if (marker[0] != 0xFF || marker[1] != 0xD8)
    {
    printf("Wrong file format.\n");
    fclose(image);
    return 1;
    }

    while(1)
    {
        
    }

     
}