#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

const int ASCII_OFFSET = 97;
const int ALPHABET_LENGTH = 26;

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print result
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
        return 0;
    }
    if (score2 > score1)
    {
        printf("Player 2 wins!\n");
        return 0;
    }
    printf("Tie!\n");
    return 0;
}

int compute_score(string word)
{
    int total_score = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        int letter_index = tolower(word[i]) - ASCII_OFFSET;
        int points = letter_index >= 0 && letter_index < ALPHABET_LENGTH ? POINTS[letter_index] : 0;
        total_score += points;
    }
    return total_score;
}
