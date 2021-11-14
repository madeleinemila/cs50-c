#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

// Declare block size and buffer for bytes in a block
const int BLOCK_SIZE = 512;
BYTE block[BLOCK_SIZE];

// Function prototype
bool is_jpg_sig();

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open image for reading
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        printf("File could not be opened for reading\n");
        return 1;
    }

    // Declare counter for output files, init to 0
    int file_num = 0;

    while (fread(&block, sizeof(BYTE), BLOCK_SIZE, infile))
    {
        if (is_jpg_sig(block))
        {
            // Generate output file name
            char output_name[8];
            sprintf(output_name, "%03i.jpg", file_num);

            // Open output file for writing
            FILE *outfile = fopen(output_name, "w");
            if (outfile == NULL)
            {
                printf("Error creating output file\n");
                fclose(infile);
                return 1;
            }

            // Declare vars to track if we have reached the start of the
            // next jpg, or the end of the data
            bool is_start_next_jpg = false;
            int bytes_read = -1; // Init to non-zero

            while (bytes_read && !is_start_next_jpg)
            {
                // Write block
                fwrite(&block, sizeof(BYTE), BLOCK_SIZE, outfile);

                // Read next block
                bytes_read = fread(&block, sizeof(BYTE), BLOCK_SIZE, infile);

                // If there is more data
                if (bytes_read)
                {
                    // Check if this block is the start of a new jpg
                    is_start_next_jpg = is_jpg_sig(block);
                }
            }

            fclose(outfile);

            // If there are more bytes
            if (bytes_read)
            {
                // Increment the number for the filename
                file_num++;

                // Seek back to start of next jpg
                fseek(infile, -BLOCK_SIZE, SEEK_CUR);
            }
        }
    }

    fclose(infile);
    return 0;
}

bool is_jpg_sig()
{
    BYTE mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
    if
    (
        block[0] == 0xff
        && block[1] == 0xd8
        && block[2] == 0xff
        && (block[3] & mask[0]) == mask[0] // 1
        && (block[3] & mask[1]) == mask[1] // 1
        && (block[3] & mask[2]) == mask[2] // 1
        && (block[3] & mask[3]) == 0       // 0
    )
    {
        return true;
    }
    return false;
}