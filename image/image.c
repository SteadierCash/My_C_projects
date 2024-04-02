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


int read_data_u16(FILE* image);
int read_data_u8(FILE* image);

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

        // if (marker >= SOF_MARKER_START && marker <= SOF_MARKER_END)
        // {
        //     read_data(image);
        // }

        if (marker >= SOF_MARKER_START && marker <= SOF_MARKER_END)
        {
            read_data_u8(image);
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














int read_data_u16(FILE* image)
{
    uint16_t segment_size;
    if (fread(&segment_size, sizeof(uint16_t), 1, image) != 1)
    {
        printf("Error reading segment size\n");
        fclose(image);
        return ERR_READ_ERROR;
    }
    segment_size = ntohs(segment_size);
    fseek(image, 1, SEEK_CUR);

    segment_size -= 3; //minus 2 bites for size_segment and 1 for size or something like that
    segment_size /= 2; // because to uint16 type

    uint16_t *segment_data = (uint16_t *)malloc(segment_size * sizeof(uint16_t));
    if (!segment_data)
    {
        printf("Memory allocation error\n");
        fclose(image);
        return ERR_READ_ERROR;
    }
    
    printf("segment size: %u\n", segment_size);
    // fseek(image, 1, SEEK_CUR);

    if (fread(segment_data, sizeof(uint16_t), segment_size, image) != segment_size)
    {
        printf("Error reading segment data\n");
        free(segment_data);
        fclose(image);
        return ERR_READ_ERROR;
    }
    printf("segment data: ");

    for (int i = 0; i < segment_size; i++)
            printf("%d ", ntohs(segment_data[i]));
    printf("\n");

    free(segment_data);
}


int read_data_u8(FILE* image)
{
    uint16_t segment_size;
            if (fread(&segment_size, sizeof(uint16_t), 1, image) != 1)
            {
                printf("Error reading segment size\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            segment_size = ntohs(segment_size);
            fseek(image, 1, SEEK_CUR);

            segment_size -= 3; //minus 2 bites for size_segment

            uint8_t *segment_data = (uint8_t *)malloc(segment_size * sizeof(uint8_t));
            if (!segment_data)
            {
                printf("Memory allocation error\n");
                fclose(image);
                return ERR_READ_ERROR;
            }
            
            printf("segment size: %u\n", segment_size);
            // fseek(image, 1, SEEK_CUR);

            if (fread(segment_data, sizeof(uint8_t), segment_size, image) != segment_size)
            {
                printf("Error reading segment data\n");
                free(segment_data);
                fclose(image);
                return ERR_READ_ERROR;
            }
            printf("segment data: ");

            for (int i = 0; i < segment_size; i++)
                    printf("%c ", ntohs(segment_data[i]));
            printf("\n");

            free(segment_data);
}