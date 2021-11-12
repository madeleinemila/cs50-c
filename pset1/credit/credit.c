#include <stdio.h>
#include <cs50.h>
#include <math.h>

const int MAX_CARD_DIGITS = 16;
const string INVALID = "INVALID";

int main(void)
{
    // Prompt user for credit card number
    long card_num;
    do
    {
        card_num = get_long("Number: ");
    }
    while (card_num < 0);

    // Convert card number to an array of integer digits
    int digits[MAX_CARD_DIGITS];
    long digits_remaining = card_num;
    for (int i = 0; i < MAX_CARD_DIGITS; i++)
    {
        int position = MAX_CARD_DIGITS - i - 1;
        digits[position] = digits_remaining % 10;
        digits_remaining = round(digits_remaining / 10);
    }

    // Handle if card number was longer than the max valid digits
    if (digits_remaining)
    {
        printf("%s\n", INVALID);
        return 0;
    }

    // Check if valid by Luhn's algorithm
    int products_sum = 0;
    for (int i = MAX_CARD_DIGITS - 2; i >= 0; i = i - 2)
    {
        int mult = digits[i] * 2;
        int sum = mult % 10 + round(mult / 10);
        products_sum += sum;
    }
    int alternate_sum = 0;
    for (int i = MAX_CARD_DIGITS - 1; i >= 0; i = i - 2)
    {
        alternate_sum += digits[i];
    }
    if ((products_sum + alternate_sum) % 10)
    {
        printf("%s\n", INVALID);
        return 0;
    }

    // Check card type
    if (digits[0] == 0 && digits[1] == 3 && (digits[2] == 4 || digits[2] == 7))
    {
        printf("AMEX\n");
        return 0;
    }

    if (digits[0] == 5 && (digits[1] == 1 || digits[1] == 2 || digits[1] == 3 || digits[1] == 4 || digits[1] == 5))
    {
        printf("MASTERCARD\n");
        return 0;
    }

    if (digits[0] == 4 || (digits[0] == 0 && digits[1] == 0 && digits[2] == 0 && digits[3] == 4))
    {
        printf("VISA\n");
        return 0;
    }

    printf("%s\n", INVALID);
    return 0;
}
