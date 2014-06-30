/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

//type of unsigned int named byte
typedef uint8_t BYTE;

bool reader (BYTE* data, FILE* input, bool* end);

int main(int argc, char* argv[])
{
    
   
    // start of a jpg counter for file naming
    int counter = 0;
    
    // variables to see if there is a jpg file open and if we are at the beginning of a jpg
    bool opened = false;
    bool beginning = false;
    
    //bool to see if we reached the end of the file
    bool end = false;
    
    // place to hold title for.jpg files
    char *title = malloc(sizeof(char)*7);
    
    // open the card file
    FILE* card = fopen("card.raw", "r");
    if (card == NULL)
        return 1;
    
    FILE* output;
    // buffer created to store memory blocks   
    BYTE* buffer = malloc(512);
        if (buffer == NULL)
            return 2;
    
    while (true)
    {
        //uses function to read into buffer and check if it is the beginning of a jpg
        beginning  = reader(buffer, card, &end);
        
        if (end == true &&  beginning == false)
            break;
        
        if (beginning == true && opened == false)
        {
            opened = true;
            sprintf(title, "%0*d.jpg", 3, counter);
            counter++;
            output = fopen(title, "a");
            fwrite(buffer, sizeof(BYTE), 512, output);
            beginning = false;
        }
        
        else if (beginning == false && opened == true)
        {
            fwrite(buffer, sizeof(BYTE), 512, output);
        }
        
        else if (beginning == true && opened == true)
        {
            fclose(output);
            sprintf(title, "%0*d.jpg",3, counter);
            counter++;
            FILE* output = fopen(title, "a");
            fwrite(buffer, sizeof(BYTE), 512, output);
            beginning = false;
            
        }
    } 
    
    free(title);
    free(buffer);
}

/*
/ function to read in information from data card and compare the first 4 bytes to jpg types, returns true if this is the start of a 
/ jpg
*/

bool reader (BYTE* data, FILE* input, bool* end)
{
    if (fread(data, sizeof(BYTE), 512, input) < 512)
        *end = true;
        
    if (data[0] == 0xff && (data[1] == 0xd8 && data[2] == 0xff) && (data[3] == 0xe0 || data[3] == 0xe1))
        return true;
    
    else
        return false;
}

