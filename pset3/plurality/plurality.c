#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Keep a record of the most votes seen;
    // init to 1 to account for case where no candidate receives valid votes
    int highest_votes = 1;

    // Keep a log of candidates with the most votes seen;
    // init each element to -1 to indicate it is empty
    int winners_indices[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        winners_indices[i] = -1;
    }

    // For each candidate:
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == highest_votes)
        {
            // If we have a tie:
            // Add this candidate to next empty slot in the winners log
            for (int w = 0; w < candidate_count; w++)
            {
                if (winners_indices[w] == -1)
                {
                    winners_indices[w] = i;
                    break;
                }
            }
        }
        else if (candidates[i].votes > highest_votes)
        {
            // If we have a new winner:
            // Update running tally to this newest highest value
            highest_votes = candidates[i].votes;

            // Clear previous winners from winners log
            for (int w = 0; w < candidate_count; w++)
            {
                winners_indices[w] = -1;
            }

            // Record new winner
            winners_indices[0] = i;
        }

    }

    // If the first element is empty, then there were no valid votes:
    if (winners_indices[0] == -1)
    {
        printf("No valid votes\n");
    }
    else
    {
        // Else print the winners:
        for (int w = 0; w < candidate_count; w++)
        {
            int winner_index = winners_indices[w];
            if (winner_index > -1)
            {
                printf("%s\n", candidates[winner_index].name);
            }
        }
    }

    return;
}

