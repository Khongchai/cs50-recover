#include <stdio.h>
#include <stdlib.h>
//typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    if (argc != 2)
        return 1;
//--------------------------------------------------------
    int k = 0;
    FILE *fptr = fopen(argv[1], "rb");
    unsigned char *buffer = malloc(512);
//--------------------------------------------------------
    if (fptr == NULL || buffer == NULL)
    {
        printf("Memory allocation failed");
        return 2;
    }
//--------------------------------------------------------
    long long filesize = 0;
    long long filesize2 = 0;

    while (fread(buffer, 1, 512, fptr) > 0)
    {
        filesize++;
    }
    printf("Filesize = %lld", filesize);

    fseek(fptr, 0, SEEK_SET);
//--------------------------------------------------------
    int jpegcount = 0;
    size_t temp = 0;
    while ((temp = fread(buffer, 1, 512, fptr)) > 0)
    {
        filesize2++;

        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {

            filesize2--;
            char filename[3];
            sprintf(filename, "%03d.jpg", jpegcount);
            jpegcount++;
            FILE *newjpeg = fopen(filename, "ab");

            if (newjpeg == NULL)
            {
                printf("Memory allocation failed");
                return 3;
            }
            do
            {
               fwrite(buffer, 1, 512, newjpeg); //write info from buffer to newjpeg
               fread(buffer, 1, 512, fptr);
//--------------------------------------------------------
               filesize2++;
               if (filesize2 == filesize)
               {
                free(buffer);
                fclose(fptr);
                printf("\nend\n");
                return 0;
//--------------------------------------------------------
               }

            }while(!((buffer[0] == 0xff) && (buffer[1] == 0xd8)  && (buffer[2] == 0xff)  && ((buffer[3] & 0xf0) == 0xe0)));

            fseek(fptr, -512, SEEK_CUR);
            fclose(newjpeg);
        }
    }

return 0;
}


