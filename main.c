#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "wordle.h"
#include "player.h"

#define TEST_ROUNDS 1000 
#define MAX_GUESSES 10   

void loadTestWords(char testWords[MAX_WORDS][WORD_LENGTH + 1], int *count)
{
    FILE *file = fopen("testList.txt", "r");
    if (!file)
    {
        perror("Failed to open testList file");
        exit(EXIT_FAILURE);
    }

    *count = 0;
    while (fscanf(file, "%s", testWords[*count]) != EOF)
    {
        (*count)++; 
    }
    fclose(file);
}

int main()
{
    char testWords[MAX_WORDS][WORD_LENGTH + 1];
    int testCount = 0;

    loadTestWords(testWords, &testCount);
    loadWordList("wordList.txt");

    srand(time(NULL));

    int totalGuesses = 0;
    int unsolvedRounds = 0; 

    for (int round = 0; round < TEST_ROUNDS; round++)
    {
        char solution[WORD_LENGTH + 1];
        strcpy(solution, testWords[rand() % testCount]);

        int guesses = 0;
        char lastResult[WORD_LENGTH + 1] = ""; 
        int solved = 0; 

        while (guesses < MAX_GUESSES)
        { 
            char *guess = player_AI(lastResult);
            guesses++;

            if (strcmp(guess, solution) == 0)
            {
                if (round % 10 == 0)
                    printf("Round %d solved in %d guesses! Solution: %s\n", round + 1, guesses, solution);
                solved = 1;
                break;
            }

            reduceword(guess, solution, lastResult);

        }

        if (!solved)
        {
            unsolvedRounds++;
        }

        totalGuesses += guesses;
    }

    printf("\nTest completed over %d rounds.\n", TEST_ROUNDS);
    printf("Average guesses: %.4f\n", (double)totalGuesses / TEST_ROUNDS);
    printf("Unsolved rounds: %d\n", unsolvedRounds);

    return 0;
}