#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    //for all candidates
    for (int j = 0; j < candidate_count; j++)
    {
        //if the string input to the vote function is the name of some candidates
        if (strcmp(candidates[j].name, name) == 0)
        {
            //update the voter's rank of that candidate
            preferences[voter][rank] = j;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    
    //following lines were my pseudocode for this question
    //go down preferences[voter][0] and add up values
    //somehow disregard votes for eliminated candidates
    //if candidates[i].eliminated == true, 
    //then go to preferences[voter][1]

    //for all voters
    for (int i = 0; i < voter_count; i++)
    {
        //and all candidates
        for (int j = 0; j < candidate_count; j++)
        {
            //this integer = the voter's rank for the candidate at hand
            int p = preferences[i][j];
            //if that candidate is sitll in the race
            if (candidates[p].eliminated == false)
            {
                //give them a vote!
                candidates[p].votes++;
                break;
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    //for all candidates
    for (int i = 0; i < candidate_count; i++)
        // if a candidates votecount is 50%+1
        if (candidates[i].votes > (voter_count / 2))
        {
            //they win
            printf("%s\n", candidates[i].name);
            return true;
        }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // set min_votes at the highest possible votes a candidate could have
    // (which would be a vote from every voter)
    int min_votes = voter_count;
    //for all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if a candidates vote total is less than min_votes, and
        //that candidate is still in the race
        if (candidates[i].votes < min_votes && candidates[i].eliminated == false)
        {
            //min_votes has a new value equal to that vote total
            //looped over all active candidates
            //this value will reach the lowest vote total any
            //candidate has
            min_votes = candidates[i].votes;
        }
    }
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //for all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if candidate is active and their vote is not the min, return false
        //bc this means there is not tie (as at least one candidate has the min
        //total and one has a total != to min
        if (candidates[i].eliminated == false && candidates[i].votes != min)
        {
            return false;
        }
    }
    //however, if all totals equal the min, we've got a tie 
    //on our hands
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    //for all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // if canddiate is active and they have the min total
        if (candidates[i].eliminated == false && candidates[i].votes == min)
        {
            //set their eliminated status to true
            candidates[i].eliminated = true;
        }
    }
}