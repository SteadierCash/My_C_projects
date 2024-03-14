#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Max number of candidates
#define MAX 9
#define MAX_LENGTH 100

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
char candidates[MAX][MAX_LENGTH];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, char* name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, char* argv[])
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
        strcpy(candidates[i], argv[i + 1]);
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
    int voter_count;

    printf("Number of voters: ");
    scanf("%d", &voter_count);
    getchar();

    
    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char name[MAX_LENGTH];

            printf("Rank %d: ", j + 1);
            fgets(name, sizeof(name), stdin);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        for (int k = 0; k < candidate_count; k++)
        {
            for (int t = 0; t < candidate_count; t++)
            {
                printf("%i", preferences[k][t]);
            }
            printf("\n");
        }

        printf("\n");
    }

    add_pairs();
    for (int t = 0; t < candidate_count; t++)
    {
        printf("%i ", pairs[t].winner);
        printf("%i\n", pairs[t].loser);
    }
    printf("---------\n");
    sort_pairs();

    for (int t = 0; t < candidate_count; t++)
    {
        printf("%i ", pairs[t].winner);
        printf("%i\n", pairs[t].loser);
    }

    lock_pairs();
    for (int k = 0; k < candidate_count; k++)
    {
        for (int t = 0; t < candidate_count; t++)
        {
            printf("%i", locked[k][t]);
        }
        printf("\n");
    }
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, char* name, int ranks[])
{
    name[strcspn(name, "\n")] = '\0';
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    int pair_place = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i] && j != i)
            {
                pairs[pair_place].winner = i;
                pairs[pair_place].loser = j;
                pair_count++;
                pair_place++;
            }
            else if (preferences[i][j] < preferences[j][i] && j != i)
            {
                pairs[pair_place].winner = j;
                pairs[pair_place].loser = i;
                pair_count++;
                pair_place++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                pair prev = pairs[i];
                pair next = pairs[j];

                pairs[j] = prev;
                pairs[i] = next;
            }
        }
    }
    return;
}

bool is_cycle(pair visited[], int counter, pair selected, pair first, int n)
{
    for (int k = 0; k < counter; k++)
    {

        if (first.winner == visited[k].loser && n > 1)
        {
            return true;
            break;
        }

        else if (selected.loser == visited[k].winner)
        {
            n++;
            if (is_cycle(visited, counter, visited[k], first, n))
            {
                return true;
            }
        }
    }
    return false;
}

//[1, 0] [2, 0] [3, 1] [2, 3]  [1, 3]   [2,1]
//[0, 1] [0, 2]   [2, 1]
//[[2,0], [0,1]]    [1,2]
//[[0, 1], [2, 0]]  [2, 1]
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int counter = 0;
    pair visited[pair_count];
    int n = 0;
    for (int i = 0; i < pair_count; i++)
    {
        if (!is_cycle(visited, counter, pairs[i], pairs[i], n))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            visited[i] = pairs[i];
            counter++;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int j = 0; j < candidate_count; j++)
    {
        int row_count = 0;
        for (int i = 0; i < candidate_count; i++)
        {
            row_count += locked[i][j];
        }
        if (row_count == 0)
        {
            printf("%s\n", candidates[j]);
        }
    }
    return;
}
