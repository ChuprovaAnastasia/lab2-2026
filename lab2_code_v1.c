#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_WORDS 10
#define MAX_LEN 20

char words[MAX_WORDS][MAX_LEN];
int word_count = 0;
int letter_val[26];
int used_digits[10];

int is_valid_assign(int word_idx, int char_idx) {
    if (char_idx == 0 && strlen(words[word_idx]) > 1) {
        if (letter_val[words[word_idx][char_idx] - 'A'] == 0) return 0;
    }
    return 1;
}

long long get_word_value(int word_idx) {
    long long val = 0;
    for (int i = 0; words[word_idx][i] != '\0'; i++) {
        val = val * 10 + letter_val[words[word_idx][i] - 'A'];
    }
    return val;
}

int solve(int word_idx, int char_idx) {
    if (word_idx == word_count) {
        long long sum = 0;
        for (int i = 0; i < word_count - 1; i++) {
            sum += get_word_value(i);
        }
        if (sum == get_word_value(word_count - 1)) {
            return 1;
        }
        return 0;
    }

    if (words[word_idx][char_idx] == '\0') {
        return solve(word_idx + 1, 0);
    }

    char current_letter = words[word_idx][char_idx];
    int letter_idx = current_letter - 'A';

    if (letter_val[letter_idx] != -1) {
        if (is_valid_assign(word_idx, char_idx)) {
            return solve(word_idx, char_idx + 1);
        }
        return 0;
    }

    for (int digit = 0; digit <= 9; digit++) {
        if (!used_digits[digit]) {
            letter_val[letter_idx] = digit;
            used_digits[digit] = 1;

            if (is_valid_assign(word_idx, char_idx)) {
                if (solve(word_idx, char_idx + 1)) {
                    return 1;
                }
            }

            letter_val[letter_idx] = -1;
            used_digits[digit] = 0;
        }
    }
    return 0;
}

void parse_rebus(char* str) {
    char* token = strtok(str, " =+");
    while (token != NULL) {
        strcpy(words[word_count++], token);
        token = strtok(NULL, " =+");
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001); 
#endif

    const char* puzzles[] = {
        "SEND + MORE = MONEY",
        "BE + BE = MOO",
        "BIG + CAT = LION",
        "TWO + TWO = FOUR",
        "FORTY + TEN + TEN = SIXTY",
        "ELEVEN + NINE + FIVE + FIVE = THIRTY"
    };

    int num_puzzles = sizeof(puzzles) / sizeof(puzzles[0]);
    double total_time = 0.0;

    printf("== base backtracking ==\n");

    for (int p = 0; p < num_puzzles; p++) {
        word_count = 0;
        memset(letter_val, -1, sizeof(letter_val));
        memset(used_digits, 0, sizeof(used_digits));

        char buffer[100];
        strcpy(buffer, puzzles[p]);

        parse_rebus(buffer);

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        int found = solve(0, 0);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
        total_time += time_ms;

        if (found) {
            printf("%s\n", puzzles[p]);

            for (int i = 0; i < word_count - 1; i++) {
                if (i > 0) printf(" + ");
                printf("%lld", get_word_value(i));
            }
            printf(" = %lld\n", get_word_value(word_count - 1));

            printf("%.3f ms\n\n", time_ms);
        }
        else {
            printf("%s\nNo solution found\n%.3f ms\n\n", puzzles[p], time_ms);
        }
    }
    printf("Total: %.3f ms\n", total_time);
    return 0;
}