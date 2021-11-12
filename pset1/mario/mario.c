#include <stdio.h>
#include <cs50.h>

void print_pyramid(int height);
void print_blocks(int num_blocks);

int main(void)
{
    // Prompt for pyramid height (must be between 1 and 8 inc.)
    int height;
    do
    {
        height = get_int("Height (between 1 and 8 inc.): ");
    }
    while (height < 1 || height > 8);

    print_pyramid(height);
}

void print_pyramid(int height)
{
    for (int i = 1; i <= height; i++)
    {
        for (int space = 1; space <= height - i; space++)
        {
            printf(" ");
        }
        print_blocks(i);
        printf("  ");
        print_blocks(i);
        printf("\n");
    }
}

void print_blocks(int num_blocks)
{
    for (int block = 1; block <= num_blocks; block++)
    {
        printf("#");
    }
}