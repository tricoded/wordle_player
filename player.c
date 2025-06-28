#include "player.h"
#include "wordle.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern char wordList[MAX_WORDS][WORD_LENGTH + 1];

static char validwords[MAX_WORDS][WORD_LENGTH + 1];
static char answerList[MAX_WORDS][WORD_LENGTH + 1]; 
static char lastGuess[WORD_LENGTH + 1] = ""; 
static int answerCount = 0;                         

void initialize_validwords()
{
    int i = 0;
    while (i < answerCount)
    {
        strncpy(validwords[i], answerList[i], WORD_LENGTH);
        i++;
    }
    validwords[i][0] = '\0'; 
}

void loadAnswerList(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open testList file");
        exit(EXIT_FAILURE);
    }
    answerCount = 0;
    while (fscanf(file, "%s", answerList[answerCount]) != EOF)
    {
        answerCount++;
    }
    fclose(file);
}

// void initialize_wordList(const char *filename)
// {
//     FILE *file = fopen(filename, "r");
//     if (!file)
//     {
//         perror("Failed to open wordList file");
//         exit(EXIT_FAILURE);
//     }
//     int idx = 0;
//     while (fscanf(file, "%s", wordList[idx]) != EOF && idx < MAX_WORDS)
//     {
//         idx++;
//     }
//     wordList[idx][0] = '\0'; 
//     fclose(file);
// }

void reduceword(const char guess[WORD_LENGTH], const char result[WORD_LENGTH + 1], char validwords[MAX_WORDS][WORD_LENGTH + 1])
{
    int idx1 = 0, idx2 = 0;
    while (validwords[idx1][0] != '\0')
    {
        char *tmp;
        tmp = checkWord(validwords[idx1], guess);
        if (strcmp(tmp, result) == 0)
        {
            strncpy(validwords[idx2], validwords[idx1], WORD_LENGTH);
            idx2++;
        }
        free(tmp);
        idx1++;
    }
    validwords[idx2][0] = '\0';
}


double entropy(const char guess[WORD_LENGTH], const char validwords[MAX_WORDS][WORD_LENGTH + 1])
{
    double distribution[243] = {0.0};
    int idx = 0, index = 0;

    while (validwords[idx][0] != '\0')
    {
        index = 0;
        char *feedback;
        feedback = checkWord(validwords[idx], guess);
        for (int i = 0; i < WORD_LENGTH; i++)
        {
            int value = (feedback[i] == 'G') ? 2 : (feedback[i] == 'Y') ? 1 : 0;
            index = index * 3 + value;
        }
        distribution[index] += 1.0;
        free(feedback);
        idx++;
    }
    if (idx == 0)
        return 0.0;
    double total = idx;
    double ent = 0.0;
    for (int i = 0; i < 243; i++)
    {
        if (distribution[i] > 0)
        {
            double probability = distribution[i] / total;
            ent -= probability * log2(probability);
        }
    }
    return ent;
}

char *player_AI(const char lastResult[WORD_LENGTH + 1])
{
    static char *dynamic_guess = NULL; 
    dynamic_guess = (char *)malloc((WORD_LENGTH + 1) * sizeof(char));

    if (lastResult[0] == ' ')
    {
        // initialize_wordList("wordList.txt");  
        loadAnswerList("testList.txt"); 
        initialize_validwords(); 
        strcpy(lastGuess, "crane");  
        strcpy(dynamic_guess, "crane");
        return dynamic_guess;
    }

    if (lastGuess[0] != '\0')
    {
        reduceword(lastGuess, lastResult, validwords);
        reduceword(lastGuess, lastResult, answerList);
    }

    int valid_count = 0;
    for (int i = 0; validwords[i][0] != '\0'; i++)
    {
        valid_count++;
    }

    // printf("validcount:%d", valid_count);

    if (valid_count == 1)
    {
        strncpy(lastGuess, validwords[0], WORD_LENGTH);
        return validwords[0];
    }

    double max_entropy_answers = -1.0;
    char *best_guess_answers = NULL;
    for (int i = 0; answerList[i][0] != '\0'; i++)
    {
        double current_entropy = entropy(answerList[i], validwords);
        if (current_entropy > max_entropy_answers)
        {
            max_entropy_answers = current_entropy;
            best_guess_answers = answerList[i];
        }
    }

    strncpy(dynamic_guess, best_guess_answers, WORD_LENGTH);
    dynamic_guess[WORD_LENGTH] = '\0';
    strcpy(lastGuess, best_guess_answers);
    return dynamic_guess;
}