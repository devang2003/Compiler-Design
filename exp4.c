
+139
Original file line number	Diff line number	Diff line change
@@ -0,0 +1,139 @@
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
int main() {
    char input[SIZE][SIZE];
    char non_terminal;
    char alpha[SIZE][SIZE], beta[SIZE][SIZE];
    int numProductions = 0;
    printf("Enter productions (type 'END' to finish):\n");
    // Input multiple grammar rules
    while (1) {
        printf("Production %d: ", numProductions + 1);
        fgets(input[numProductions], SIZE, stdin);
        input[numProductions][strcspn(input[numProductions], "\n")] = '\0'; // Remove newline
        if (strcmp(input[numProductions], "END") == 0) break;
        numProductions++;
    }
    for (int rule = 0; rule < numProductions; rule++) {
        int alphaCount = 0, betaCount = 0, hasEpsilon = 0;
        non_terminal = input[rule][0];
        // Ensure valid format (A->...)
        char *productions = strchr(input[rule], '>');
        if (!productions || productions[1] == '\0') {
            printf("\nInvalid production format: %s\n", input[rule]);
            continue;
        }
        productions += 1; // Move past '>'
        // Tokenize the right-hand side
        char *token = strtok(productions, "|");
        while (token) {
            if (strcmp(token, "ε") == 0) {
                hasEpsilon = 1;
            } else if (token[0] == non_terminal) {
                strncpy(alpha[alphaCount++], token + 1, SIZE - 1);
            } else {
                strncpy(beta[betaCount++], token, SIZE - 1);
            }
            token = strtok(NULL, "|");
        }
        // Print modified grammar
        if (alphaCount > 0) {
            printf("\nGrammar Without Left Recursion for %c:\n", non_terminal);
            printf(" %c -> ", non_terminal);
            for (int i = 0; i < betaCount; i++) {
                printf("%s%c'%s", beta[i], non_terminal, (i < betaCount - 1) ? " | " : "");
            }
            if (hasEpsilon) printf(" | ε");
            printf("\n");
            printf(" %c' -> ", non_terminal);
            for (int i = 0; i < alphaCount; i++) {
                printf("%s%c'%s", alpha[i], non_terminal, (i < alphaCount - 1) ? " | " : "");
            }
            printf(" | ε\n");
        } else {
            printf("\nNo left recursion detected for %c.\n", non_terminal);
        }
    }
    return 0;
}
#include <stdio.h>
#include <string.h>
#define SIZE 100
#define MAX_PRODUCTIONS 10
void eliminateLeftFactoring(char non_terminal, char *production) {
    char part[MAX_PRODUCTIONS][SIZE], common[SIZE] = "", newGram[MAX_PRODUCTIONS][SIZE];
    int count = 0, pos = 0, hasEpsilon = 0;
    // Split production at '|'
    char *token = strtok(production, "|");
    while (token) {
        if (!strcmp(token, "ε")) hasEpsilon = 1;
        else strcpy(part[count++], token);
        token = strtok(NULL, "|");
    }
    if (count < 2) { // No left factoring needed
        printf("%c -> %s\n", non_terminal, production);
        return;
    }
    // Find common prefix
    while (pos < strlen(part[0])) {
        char c = part[0][pos];
        for (int i = 1; i < count; i++)
            if (part[i][pos] != c) goto stop;
        common[pos++] = c;
    }
stop:
    common[pos] = '\0';
    // Print factored grammar
    printf("%c -> %s%c'\n%c' -> ", non_terminal, common, non_terminal, non_terminal);
    int newCount = 0;
    for (int i = 0; i < count; i++)
        if (strlen(part[i]) > pos) printf("%s%s", newCount++ ? " | " : "", part[i] + pos);
   
    if (hasEpsilon || newCount == 0) printf(" | ε");
    printf("\n");
}
int main() {
    char input[MAX_PRODUCTIONS][SIZE];
    int numProductions = 0;
    printf("Enter productions (type 'END' to finish):\n");
    while (1) {
        printf("Production %d: ", numProductions + 1);
        fgets(input[numProductions], SIZE, stdin);
        input[numProductions][strcspn(input[numProductions], "\n")] = '\0';
        if (!strcmp(input[numProductions], "END")) break;
        numProductions++;
    }
    printf("\nGrammar Without Left Factoring:\n");
    for (int i = 0; i < numProductions; i++)
        eliminateLeftFactoring(input[i][0], strchr(input[i], '>') + 1);
    return 0;
}
