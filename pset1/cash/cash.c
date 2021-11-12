#include <stdio.h>
#include <cs50.h>
#include <math.h>

int get_min_coins(int cents);

int coins[] = {25, 10, 5, 1};

int main(void)
{
    // Prompt user for how much change is owed
    float change_dollars;
    do
    {
        change_dollars = get_float("Change owed: ");
    }
    while (change_dollars < 0.0);

    // Convert to cents to avoid floating point imprecision
    int change_cents = round(change_dollars * 100);

    int min_coins = get_min_coins(change_cents);
    printf("%i\n", min_coins);
}

int get_min_coins(int cents)
{
    int num_coins = 0;
    int cents_remaining = cents;
    int coin_index = 0; // Try the highest coin first

    while (cents_remaining)
    {
        if (cents_remaining >= coins[coin_index])
        {
            // Include one of that size coin
            cents_remaining -= coins[coin_index];
            num_coins++;
        }
        else
        {
            // Move to the next highest coin
            coin_index++;
        }
    }
    return num_coins;
}