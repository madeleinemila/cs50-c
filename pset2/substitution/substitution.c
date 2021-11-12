#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>

bool is_alpha_only_string(int length, string str);
bool contains_unique_letters(int length, string str);
int encrypt(char plain_char, string key);

const int ALPHABETIC_LETTERS = 26;
const int LOWERCASE_ASCII_OFFSET = 97;

int main(int argc, string argv[])
{
    // Check CLI args are valid
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];

    // Validate key
    int key_length = strlen(key);
    if (key_length != ALPHABETIC_LETTERS)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    bool is_alpha_only_key = is_alpha_only_string(key_length, key);
    if (!is_alpha_only_key)
    {
        printf("Key must contain only alphabetic characters.\n");
        return 1;
    }
    bool is_unique = contains_unique_letters(key_length, key);
    if (!is_unique)
    {
        printf("Key must contain only one of each character.\n");
        return 1;
    }

    // Get plaintext to encrypt
    string plaintext;
    do
    {
        plaintext = get_string("plaintext: ");
    }
    while (!plaintext[0]);

    // Encrypt using key
    int text_length = strlen(plaintext);
    char ciphertext[text_length + 1];
    for (int i = 0; i < text_length; i++)
    {
        ciphertext[i] = encrypt(plaintext[i], key);
    }
    ciphertext[text_length] = 0;

    // Print result
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

bool is_alpha_only_string(int length, string str)
{
    for (int i = 0; i < length; i++)
    {
        if (!isalpha(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool contains_unique_letters(int length, string str)
{
    bool seen[ALPHABETIC_LETTERS];
    for (int i = 0; i < ALPHABETIC_LETTERS; i++)
    {
        seen[i] = false;
    }
    for (int i = 0; i < length; i++)
    {
        int letter_index = tolower(str[i]) - LOWERCASE_ASCII_OFFSET;
        if (seen[letter_index])
        {
            return false;
        }
        seen[letter_index] = true;
    }
    return true;
}

int encrypt(char plain_char, string key)
{
    if (!isalpha(plain_char))
    {
        return plain_char;
    }
    bool is_lower = islower(plain_char);
    int letter_index = tolower(plain_char) - LOWERCASE_ASCII_OFFSET;
    char cipher_char = key[letter_index];
    return is_lower ? tolower(cipher_char) : toupper(cipher_char);
}