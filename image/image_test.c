#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>

//constatns for markers 
#define SOI_MARKER          0xFFD8      //start of image marker
#define SOF_MARKER_START    0xFFC0      //start of frame - start mmarker
#define SOF_MARKER_END      0xFFCF      //start of frame - end marker
#define APP0_MARKER         0xFFE0      //comment_marker
#define APP1_MARKER         0xFFE1      //comment_marker



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

        else if (marker == APP1_MARKER)
        {
            // printf("Comment segment found\n");

            // Read the segment size
            uint16_t segment_size;
            if (fread(&segment_size, sizeof(uint16_t), 1, image) != 1)
            {
                printf("Error reading segment size\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            segment_size = ntohs(segment_size);

            // Read APP1 segment data
            // char *segment_data = (char *)malloc(segment_size);
            // if (!segment_data)
            // {
            //     printf("Memory allocation error\n");
            //     fclose(image);
            //     return ERR_READ_ERROR;
            // }
            // segment_size -= 2;
            // if (fread(segment_data, sizeof(char), segment_size, image) != segment_size)
            // {
            //     printf("Error reading segment data\n");
            //     free(segment_data);
            //     fclose(image);
            //     return ERR_READ_ERROR;
            // }

            // free(segment_data);

            uint8_t *segment_data = (uint8_t *)malloc(segment_size * sizeof(uint8_t));
            if (!segment_data)
            {
                printf("Memory allocation error\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            segment_size -= 2;

            if (fread(segment_data, sizeof(uint8_t), segment_size, image) != segment_size)
            {
                printf("Error reading segment data\n");
                free(segment_data);
                fclose(image);
                return ERR_READ_ERROR;
            }
            printf("segment data: ");
            

            for (int i = 0; i < segment_size; i++)
                if ((int)segment_data[i] != 0)
                    printf("%c ", segment_data[i]);

            printf("\n");

            free(segment_data);
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
        printf("End of the file\n");
        fclose(image);
        
        return ERR_NO_SOF;
    }
    fclose(image);

    return 0;
}