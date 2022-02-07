//HELP LOG:
//Received help from Sukesh Ram '23 on the use of fread and fwrite, and the construction of the while loop

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //if more than 2 command line arguments, get that weak shit outta here 
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw");
        return 1;
    }

    //initialize variable to point to file name
    char *input_name = argv[1];

    //initialize variable to point to start of file 
    FILE *input_pointer = fopen(input_name, "r");

    //if nothing is there, print an error message
    if (input_pointer == NULL)
    {
        printf("Unable to open file: %s\n", input_name);
        return 1;
    }

    //512-byte array since 512 bytes is the max size of a jpeg,
    //we'll use this to read 512 bites at a time
    BYTE buffer[512];

    //establish a variable for the file name, which will be 8 bytes
    // xxx.jpg is seven chars, plus another for the null symbol
    char filename[8];

    //will point to the new file, initialized to NULL
    //because it doesn't exist yet
    FILE *output_pointer = NULL;

    //counter will track the number of images recovered,
    //which will be helpful for naming them later on
    int counter = 0;

    //read every block from card.raw
    while (fread(buffer, 1, 512, input_pointer) == 512)
    {
        //if we have the jpeg header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if the counter isn't 0, close the output
            //in other words, we're dealing with a new image now
            if (!(counter == 0))
            {
                fclose(output_pointer);
            }
            //print a file name that's the 3-digit version of the counter 
            //with a .jpeg extension
            sprintf(filename, "%03i.jpg", counter);

            //open output file to write
            output_pointer = fopen(filename, "w");
            counter++;
        }
        //if we've added to the counter (which only happens when our jpeg tags are round)
        //then write one new elmenet of 512 bytes
        if (!(counter == 0))
        {
            fwrite(&buffer, 1, 512, output_pointer);
        }
    }
    //close out
    fclose(input_pointer);
    fclose(output_pointer);
    return 0;
}