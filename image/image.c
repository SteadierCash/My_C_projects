#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>

//constatns for markers 
#define SOI_MARKER          0xFFD8      //start of image marker
#define SOF_MARKER_START    0xFFC0      //start of frame - start mmarker
#define SOF_MARKER_END      0xFFCF      //start of frame - end marker
#define COM_MARKER          0xFFFE      //comment_marker

//errors
#define ERR_FILE_OPEN   -1
#define ERR_NOT_JPEG    -2
#define ERR_NO_SOF      -3
#define ERR_READ_ERROR  -4

//pixel structure
typedef struct 
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

int main(int argc, char* argv[])
{
    // arguments check
    if (argc != 2)
    {
        printf("Program usage: ./image <file_name>\n");
        return 1;
    }

    FILE* image = fopen(argv[1], "rb");

    // check if file exists
    if (!image)
    {
        printf("Cannot open this file\n");
        return ERR_FILE_OPEN;
    }

    //check if this is jpeg file 
    uint16_t magic_number;
    if (fread(&magic_number, sizeof(uint16_t), 1, image) != 1)
    {
        printf("Error reading magic number.\n");
        fclose(image);
        return ERR_READ_ERROR;
    }

    magic_number = ntohs(magic_number);    //converting to system endiannees 

    if (magic_number != SOI_MARKER)
    {
        printf("Not a JPEG file.\n");
        return ERR_NOT_JPEG;
    }

    

    //search for frame marker
    uint16_t marker;
    while (fread(&marker, sizeof(uint16_t), 1, image) == 1)
    {
        marker = ntohs(marker);
        
        printf("marker: %x\n", marker);

        if (marker >= SOF_MARKER_START && marker <= SOF_MARKER_END)
        {
            fseek(image, 3, SEEK_CUR);

            uint16_t height, width;
            if (fread(&height, sizeof(uint16_t), 1, image) != 1 || fread(&width, sizeof(uint16_t), 1, image) != 1)
            {
                printf("Error reading image dimensions.\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            height = ntohs(height); // Convert from network byte order
            width = ntohs(width);   // Convert from network byte order

            printf("Width: %d, Height: %d\n", width, height);
        }

        else if (marker == COM_MARKER)
        {
            printf("Comment segment found\n");

            // Read the segment size
            uint16_t segment_size;
            if (fread(&segment_size, sizeof(uint16_t), 1, image) != 1)
            {
                printf("Error reading segment size\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            segment_size = ntohs(segment_size);

            // Read and print the comment data
            char comment[segment_size - 2]; // Subtract 2 for the size field

            if (fread(comment, sizeof(char), segment_size - 2, image) != segment_size - 2)
            {
                printf("Error reading comment data\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            printf("Comment: %s\n", comment);
        }

        else
        {
            uint16_t segment_size;
            if (fread(&segment_size, sizeof(uint16_t), 1, image) != 1)
            {
                printf("Error reading segment size\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            segment_size = ntohs(segment_size);
            fseek(image, segment_size - 2, SEEK_CUR);
        }
    }

    if (feof(image))
    {
        printf("End of the file");
        fclose(image);
        return ERR_NO_SOF;
    }
    

    fclose(image);
    
    return 0;
}