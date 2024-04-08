#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATES 5
#define HASH_TABLE_SIZE 101

struct Candidate
{
    char name[50];
    int votes;
    struct Candidate *next;
};

struct Voter
{
    char username[50];
    unsigned long hashedPassword; 
    int voted;
    struct Voter *next;
};

struct Candidate *candidates = NULL;
struct Voter *voters = NULL;

unsigned long djb2_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; 
    }

    return hash;
}

void addCandidate(char name[])
{
    struct Candidate *newCandidate = (struct Candidate *)malloc(sizeof(struct Candidate));
    strcpy(newCandidate->name, name);
    newCandidate->votes = 0;
    newCandidate->next = candidates;
    candidates = newCandidate;
}

void addVoter(char username[], char password[])
{
    struct Voter *newVoter = (struct Voter *)malloc(sizeof(struct Voter));
    strcpy(newVoter->username, username);
    newVoter->hashedPassword = djb2_hash(password);
    newVoter->voted = 0;
    newVoter->next = voters;
    voters = newVoter;
}

void displayCandidates()
{
    for (int i = 1; i <= 5; i++)
        printf("Candidate: %d\n", i);
}

void displayCandidatesVotes()
{
    printf("Candidates:\n");
    struct Candidate *currentCandidate = candidates;
    int count = 1;
    while (currentCandidate != NULL)
    {
        printf("%d. %s - Votes: %d\n", count++, currentCandidate->name, currentCandidate->votes);
        currentCandidate = currentCandidate->next;
    }
}

void displayMenu()
{
    printf("\n=== Menu ===\n");
    printf("1. Register Voter\n");
    printf("2. Vote\n");
    printf("3. Display Candidate Votes\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void handleRegistration()
{
    char username[50], password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    struct Voter *currentVoter = voters;
    while (currentVoter != NULL)
    {
        if (strcmp(currentVoter->username, username) == 0)
        {
            printf("This username is already registered.\n");
            return;
        }
        currentVoter = currentVoter->next;
    }

    addVoter(username, password);
    printf("Registration successful!\n");
}

void handleVoting()
{
    char username[50], password[50];
    printf("Enter username: ");
    scanf("%s", username);

    getchar();

    printf("Enter password: ");
    scanf("%s", password);

    struct Voter *currentVoter = voters;
    int found = 0;

    while (currentVoter != NULL)
    {
        if (strcmp(currentVoter->username, username) == 0 && currentVoter->hashedPassword == djb2_hash(password))
        {
            found = 1;
            if (currentVoter->voted == 1)
            {
                printf("You have already voted.\n");
                return;
            }
            break;
        }
        currentVoter = currentVoter->next;
    }

    if (!found)
    {
        printf("Invalid username or password.\n");
        return;
    }

    int choice;
    printf("Vote for a candidate:\n");
    displayCandidates();
    printf("Enter candidate number to vote: ");
    scanf("%d", &choice);

    struct Candidate *currentCandidate = candidates;
    int voted = 0;
    while (currentCandidate != NULL)
    {
        if (choice == 1)
        {
            currentCandidate->votes++;
            currentVoter->voted = 1;
            voted = 1;
            break;
        }
        choice--;
        currentCandidate = currentCandidate->next;
    }

    if (!voted)
    {
        printf("Invalid candidate number.\n");
        return;
    }

    printf("Vote cast successfully!\n");
}

int main()
{
    addCandidate("Candidate 5");
    addCandidate("Candidate 4");
    addCandidate("Candidate 3");
    addCandidate("Candidate 2");
    addCandidate("Candidate 1");

    int choice;
    do
    {
        displayMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\n**If you have any disabilities or have any language barriers, Our team will help you in all possible ways\n");
            handleRegistration();
            break;
        case 2:
            handleVoting();
            break;
        case 3:
            displayCandidatesVotes();
            break;
        case 4:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Please select a valid option.\n");
            break;
        }
    } while (choice != 4);

    return 0;
}

