#include <cs50.h>
#include <stdio.h>

int get_num_years(int startSize, int endSize);

int main(void)
{
    int startSize, endSize;

    // Prompt for start size
    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < 9);

    // Prompt for end size
    do
    {
        endSize = get_int("End size: ");
    }
    while (endSize < startSize);

    // Get number of years until we reach threshold
    int numYears = get_num_years(startSize, endSize);

    // Print number of years
    printf("Years: %i\n", numYears);
}

int get_num_years(int startSize, int endSize)
{
    int numYears = 0;

    while (startSize < endSize)
    {
        startSize = startSize + (startSize / 3) - (startSize / 4);
        numYears++;
    }

    return numYears;
}
