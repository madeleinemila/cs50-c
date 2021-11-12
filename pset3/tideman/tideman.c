#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int c = 0; c < candidate_count; c++)
    {
        if (strcmp(name, candidates[c]) == 0)
        {
            ranks[rank] = c;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int r = 0; r < candidate_count; r++)
    {
        int candidate_index = ranks[r];

        // For each opponent they beat:
        for (int o = r + 1; o < candidate_count; o++)
        {
            int opponent_index = ranks[o];
            preferences[candidate_index][opponent_index] = preferences[candidate_index][opponent_index] + 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int i_margin = preferences[i][j];
            int j_margin = preferences[j][i];
            if (i_margin > j_margin)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            if (j_margin > i_margin)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swap_count = -1;
    while (swap_count != 0)
    {
        swap_count = 0;
        for (int p = 0; p < pair_count - 1; p++)
        {
            int l_winner_index = pairs[p].winner;
            int l_loser_index = pairs[p].loser;
            int l_strength = preferences[l_winner_index][l_loser_index] - preferences[l_loser_index][l_winner_index];
            int r_winner_index = pairs[p + 1].winner;
            int r_loser_index = pairs[p + 1].loser;
            int r_strength = preferences[r_winner_index][r_loser_index] - preferences[r_loser_index][r_winner_index];
            if (r_strength > l_strength)
            {
                int tmp_winner = pairs[p].winner;
                int tmp_loser = pairs[p].loser;
                pairs[p].winner = pairs[p + 1].winner;
                pairs[p].loser = pairs[p + 1].loser;
                pairs[p + 1].winner = tmp_winner;
                pairs[p + 1].loser = tmp_loser;
                swap_count++;
            }
        }

    }
    return;
}

bool cycle_detected(int start_node, int next_node)
{
    if (start_node == next_node)
    {
        // We've reached a cycle
        return true;
    }

    // Explore neighbours
    for (int n = 0; n < candidate_count; n++)
    {
        // If it's an edge
        if (locked[next_node][n])
        {
            // Move to next node
            if (cycle_detected(start_node, n))
            {
                // Bubble up true value from recursive call
                return true;
            }
        }
    }

    // If we got to this point, there is no cycle
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Add first edge
    locked[pairs[0].winner][pairs[0].loser] = true;

    // Iterate through remaining pairs
    for (int p = 1; p < pair_count; p++)
    {
        int winner_index = pairs[p].winner;
        int loser_index = pairs[p].loser;

        // Add edge if it doesn't create a cycle
        if (!cycle_detected(winner_index, loser_index))
        {
            locked[winner_index][loser_index] = true;
        }
    }
    return;
}

void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // For the current node, check if it has any incoming arrows.
        // If it has no incoming arrows, it is a winner - print the name.

        bool no_incoming_arrows = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                no_incoming_arrows = false;
                break;
            }
        }

        if (no_incoming_arrows)
        {
            printf("%s\n", candidates[i]);
        }
    }
}
