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
    //cycle through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // check if the name that's the input of this function
        //and a candidate's name in a struct are the same
        if (strcmp(candidates[i].name, name) == 0)
        {
            //if so, add one to the vote total of that candidates
            //and return true
            candidates[i].votes++;
            return true;
        }
    }
    //otherwise, false
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // assign a variable to represent what will be the winning vote total
    int winning_total = 0;

    //for all candidates
    for (int j = 0; j < candidate_count; j++)
    {
        //if a candidate's vote total is greater than the winning total
        //update the value of winning_total to be vote total of that candidates
        if (candidates[j].votes > winning_total)
        {
            winning_total = candidates[j].votes;
        }
    }
    //now, winning_total is equal to the highest votal found among all candidates
    for (int k = 0; k < candidate_count; k++)
    {
        //if a candidate's total is equal to that total, print the name
        if (candidates[k].votes == winning_total)
        {
            printf("%s\n", candidates[k].name);
        }
    }
}