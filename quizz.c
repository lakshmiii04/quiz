#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUESTIONS 10
#define OPTIONS 4

// Structure for question
typedef struct {
    char question[256];
    char options[OPTIONS][64];
    int correct_option;
} Question;

// Function to display rules
void showRules() {
    printf("====================================\n");
    printf("            QUIZ GAME RULES\n");
    printf("====================================\n");
    printf("1. Each question has 4 options\n");
    printf("2. Enter your answer between 1 and 4\n");
    printf("3. Each correct answer gives 1 mark\n");
    printf("4. No negative marking\n\n");
}

// Function to display a question
void displayQuestion(Question q) {
    printf("%s\n", q.question);
    for (int i = 0; i < OPTIONS; i++) {
        printf("%d. %s\n", i + 1, q.options[i]);
    }
}

// Function to check answer
int checkAnswer(Question q, int user_answer) {
    return user_answer == q.correct_option;
}

int main() {
    Question questions[MAX_QUESTIONS];
    int total_questions = 0;
    int score = 0;

FILE *fp = fopen("questions.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open questions file.\n");
        return 0;
    }

    // Read questions from file (real-time loading)
    while (fgets(questions[total_questions].question, 256, fp)) {
        questions[total_questions].question[
            strcspn(questions[total_questions].question, "\n")] = '\0';

        for (int i = 0; i < OPTIONS; i++) {
            fgets(questions[total_questions].options[i], 64, fp);
            questions[total_questions].options[i][
                strcspn(questions[total_questions].options[i], "\n")] = '\0';
        }

        fscanf(fp, "%d\n", &questions[total_questions].correct_option);
        fgets((char[2]){0}, 2, fp); // read blank line

        total_questions++;
        if (total_questions == MAX_QUESTIONS)
            break;
    }

    fclose(fp);

    srand(time(NULL));

    printf("====================================\n");
    printf("      WELCOME TO REAL-TIME QUIZ\n");
    

    showRules();

    // Shuffle questions
    for (int i = 0; i < total_questions; i++) {
        int j = rand() % total_questions;
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }

    for (int i = 0; i < total_questions; i++) {
        printf("\nQuestion %d/%d\n", i + 1, total_questions);
        displayQuestion(questions[i]);

        int user_answer;
        printf("Enter your answer (1-4): ");

        if (scanf("%d", &user_answer) != 1) {
            printf("Invalid input! Numbers only.\n");
            while (getchar() != '\n');
            i--;
            continue;
        }

        if (user_answer >= 1 && user_answer <= 4) {
            if (checkAnswer(questions[i], user_answer)) {
                printf("Correct!\n");
                score++;
            } else {
                printf("Wrong! Correct answer: %d. %s\n",
                       questions[i].correct_option,
                       questions[i].options[
                           questions[i].correct_option - 1]);
            }
        } else {
            printf("Invalid choice! Try again.\n");
            i--;
        }
    }

    printf("\n====================================\n");
    printf("Quiz Completed!\n");
    printf("Your Score: %d / %d\n", score, total_questions);

    if (score == total_questions)
        printf("Excellent Performance!\n");
    else if (score >= total_questions / 2)
        printf("Good Job!\n");
    else
        printf("Keep Practicing!\n");

    printf("====================================\n");

    return 0;
}
