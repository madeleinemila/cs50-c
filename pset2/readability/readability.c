#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<math.h>

int count_num_letters(string text);
int count_num_words(string text);
int count_num_sentences(string text);
int calc_coleman(int num_letters, int num_words, int num_sentences);

const int SPACE = 32;
const int FULLSTOP = 46;
const int EXCLAMATION_MARK = 33;
const int QUESTION_MARK = 63;

int main(void)
{
    // Prompt user for text
    string text;
    do
    {
        text = get_string("Text: ");
    }
    while (!text[0]);

    // Calculate number of letters, words and sentences
    int num_letters = count_num_letters(text);
    int num_words = count_num_words(text);
    int num_sentences = count_num_sentences(text);

    // Provide grade
    int grade = calc_coleman(num_letters, num_words, num_sentences);
    if (grade > 16)
    {
        printf("Grade 16+\n");
        return 0;
    }
    if (grade < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    printf("Grade %i\n", grade);
    return 0;
}

bool is_alpha(int char_code)
{
    return (char_code >= 65 && char_code <= 90) || (char_code >= 97 && char_code <= 122);
}

int count_num_letters(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        int char_code = (int) text[i];
        if (is_alpha(char_code))
        {
            count++;
        }
    }
    return count;
}

int count_num_words(string text)
{
    int count = 0;
    // A word is defined as characters between a space.
    // A sentence will not have an extra space at the beginning or end,
    // nor more than one space between words.
    // Count the number of spaces:
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == SPACE)
        {
            count++;
        }
    }
    // Add one to return count of words:
    return count + 1;
}

int count_num_sentences(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        int char_code = (int) text[i];
        if (char_code == FULLSTOP || char_code == EXCLAMATION_MARK || char_code == QUESTION_MARK)
        {
            count++;
        }
    }
    return count;
}

int calc_coleman(int num_letters, int num_words, int num_sentences)
{
    float average_letters = (float) num_letters / (float) num_words * 100.0;
    float average_sentences = (float) num_sentences / (float) num_words * 100.0;

    return round(0.0588 * average_letters - 0.296 * average_sentences - 15.8);
}
