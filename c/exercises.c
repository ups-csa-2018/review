#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

void exercise1()
{
    printf("meh\n");
}

void exercise2()
{
    int dices[6];
    int stop = 0;

    while (!stop) {
        for (int i = 0; i < 6; i++) {
            dices[i] = rand() % 6;
        }

        printf("Dices rolled: %d %d %d %d %d %d\n",
                dices[0],
                dices[1],
                dices[2],
                dices[3],
                dices[4],
                dices[5]);

        int sorts[6] = {0,0,0,0,0,0};
        for (int i = 0; i < 6; i++) {
            if (++sorts[dices[i]] >= 4) {
                stop = 1;
                break;
            }
        }
    }
}

void exercise3()
{
    int x, y;
    int **matrix;

    printf("Matrix dimension x: \n");
    if (1 != scanf("%d", &x)) {
        fprintf(stderr, "Unable to read the x dimension.\n");
        exit(EXIT_FAILURE);
    }
    printf("Matrix dimension y: \n");
    if (1 != scanf("%d", &y)) {
        fprintf(stderr, "Unable to read the y dimension.\n");
        exit(EXIT_FAILURE);
    }

    if (x <= 0 || x >= 6 || y <= 0 || y >= 6) {
        fprintf(stderr, "Matrix dimesions should not exceed 5x5.\n");
        exit(EXIT_FAILURE);
    }

    // I was not sure if the goal of this exercise was to write an algorithm that
    // manipulate a multi dimentionnal array, or not. So I used one anyway.
    matrix = malloc(sizeof(int *) * x);
    for (int i = 0; i < x; i++) {
        matrix[i] = malloc(sizeof(int) * y);
        for (int j = 0; j < y; j++) {
            matrix[i][j] = 0x61 + j + i * x;
        }
    }

    printf("Original matrix: \n");

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("Transposed matrix: \n");

    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; i++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < x; i++) {
        free(matrix[i]);
    }

    free(matrix);
}

void exercise4()
{
    char *word = NULL;
    int wordlen;
    int palindrome = 1;

    printf("Write a word to check if it's a palindrome:\n");

    if (1 != scanf("%ms", &word)) {
        fprintf(stderr, "Unable to read the word.\n");
        exit(EXIT_FAILURE);
    }

    wordlen = strlen(word);

    for (int i = 0; i < wordlen / 2; i++) {
        if (word[i] != word[wordlen - i - 1]) {
            palindrome = 0;
            break;
        }
    }

    if (palindrome) {
        printf("%s is a palindrome!\n", word);
    } else {
        printf("%s is not a palindrome!\n", word);
    }

    free(word);
}

void exercise5()
{
    char *message;
    int key;

    printf("Write the key:\n");
    if (1 != scanf("%d", &key)) {
        fprintf(stderr, "Unable to read the key.\n");
        exit(EXIT_FAILURE);
    }

    if (key < 0 || key > 25) {
        fprintf(stderr, "The key must be between 0 and 25.\n");
        exit(EXIT_FAILURE);
    }

    // throw the \n char to garbage (yes this is a bad hack)
    getc(stdin);

    printf("Write the message:\n");
    if (1 != scanf("%m[^\n]s", &message)) {
        fprintf(stderr, "Unable to read the message.\n");
        exit(EXIT_FAILURE);
    }

    printf("Plaintext: %s\n", message);

    for (int i = 0; i < strlen(message); i++) {
        int offset;
        if (message[i] >= 'a' && message[i] <= 'z') {
            offset = 'a';
        } else if (message[i] >= 'A' && message[i] <= 'Z') {
            offset = 'A';
        } else {
            continue;
        }

        message[i] = (message[i] + key - offset) % 25 + offset;
    }

    printf("Ciphertext: %s\n", message);

    free(message);
}

void _exercise6_cutting_minutes(int *hours, int *minutes)
{
    *hours += *minutes / 60;
    *minutes = *minutes % 60;
}

void exercise6()
{
    int heures = 0, minutes = 120;
    _exercise6_cutting_minutes(&heures, &minutes);
    printf("%d heures et %d minutes.\n", heures, minutes);
}

void _exercise7(int *array, size_t size, int max)
{
    for (int i = 0; i < size; i++) {
        if (array[i] > max) {
            array[i] = 0;
        }
    }
}

void exercise7()
{
    int array[3] = {2, 4, 6};
    _exercise7(array, 3, 4);

    printf("%d, %d, %d\n", array[0], array[1], array[2]);
}

void exercise8()
{
    int array[10];

    printf("Variable array[10] is an array of integers.\n");
    printf("Integer size is: %zu bytes.\n", sizeof(int));

    for (int i = 0; i < 10; i++) {
        printf("array[%d] is at 0x%p\n", i, &array[i]);
    }
}

void exercise9()
{
    char *file;
    FILE *fp;
    char s[255];
    char *r;

    printf("Write the file to read:\n");

    if (1 != scanf("%m[^\n]s", &file)) {
        fprintf(stderr, "Unable to read the file to read.\n");
        exit(EXIT_FAILURE);
    }

    if (NULL == (fp = fopen(file, "r"))) {
        fprintf(stderr, "Unable to open file %s: %s.\n", file, strerror(errno));
        free(file);
        exit(EXIT_FAILURE);
    }

    while (NULL != (r = fgets(s, 255, fp))) {
        printf("%s", s);
    }

    if (0 != errno) {
        printf("Unable to read the whole file: %s.\n", strerror(errno));
        fclose(fp);
        free(file);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    free(file);
}

struct _exercise11_thread_job
{
    unsigned short rows_nb;
    int offset;
    int (*matrix)[5];
    int result;
};

void *_exercise11_thread(void *args)
{
    struct _exercise11_thread_job *job = args;

    for (int i = 0; i < job->rows_nb; i++) {
        int sum = 0;

        for (int j = 0; j < 5; j++) {
            sum += job->matrix[job->offset + i][j];
        }

        printf("The total in row %d is %d\n", job->offset + i, sum);

        job->result += sum;
    }

    return NULL;
}

void exercise11()
{
    // This is not exactly what was asked in the subject, but as I didn't want
    // to create a global variable (which is bad!) for only one exercise, I
    // created an _exercise11_thread_job structure which holds the thread
    // context.
    int matrix[10][5];
    int threads_nb;
    pthread_t *threads;
    struct _exercise11_thread_job *thread_jobs;
    int result;
    int remaining_rows = 10;

    printf("How many threads:\n");
    if (1 != scanf("%d", &threads_nb)) {
        fprintf(stderr, "Unable to read the number of threads.\n");
        exit(EXIT_FAILURE);
    }

    if (threads_nb <= 0 || threads_nb > 10) {
        fprintf(stderr, "The number of threads must be between 1 and 10.\n");
        exit(EXIT_FAILURE);
    }

    threads = malloc(sizeof(pthread_t) * threads_nb);
    thread_jobs = malloc(sizeof(struct _exercise11_thread_job) * threads_nb);

    printf("Matrix:\n");

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            matrix[i][j] = j + i * 5;
            printf("%4d", matrix[i][j]);
        }

        printf("\n");
    }

    for (int i = 0; i < threads_nb; i++) {
        thread_jobs[i].rows_nb = remaining_rows / (threads_nb - i);
        thread_jobs[i].matrix = matrix;
        thread_jobs[i].offset = 10 - remaining_rows;
        remaining_rows -= thread_jobs[i].rows_nb;

        int e = pthread_create(
                &threads[i],
                NULL,
                _exercise11_thread,
                (void *)&thread_jobs[i]);

        if (0 != e) {
            fprintf(stderr, "Unable to start thread #%d: %s\n", i, strerror(e));
            free(threads);
            free(thread_jobs);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < threads_nb; i++) {
        int e = pthread_join(threads[i], NULL);
        if (0 != e) {
            fprintf(stderr, "Unable to wait for thread #%d: %s\n", i, strerror(e));
            free(threads);
            free(thread_jobs);
            exit(EXIT_FAILURE);
        }

        result += thread_jobs[i].result;
    }

    printf("The total values in the matrix is %d\n", result);

    free(threads);
    free(thread_jobs);
}

void init()
{
    unsigned int seed = time(NULL);
    srand(seed);
}

int main(int argc, char **argv)
{
    int exercise_number;

    if (argc != 2) {
        fprintf(stderr, "Usage: exercises <exercise-number>\n");
        exit(EXIT_FAILURE);
    }

    exercise_number = atoi(argv[1]);

    init();

    switch (exercise_number) {
        case 1: exercise1(); break;
        case 2: exercise2(); break;
        case 3: exercise3(); break;
        case 4: exercise4(); break;
        case 5: exercise5(); break;
        case 6: exercise6(); break;
        case 7: exercise7(); break;
        case 8: exercise8(); break;
        case 9: exercise9(); break;
        case 11: exercise11(); break;
        default:
            fprintf(
                stderr,
                "exercise %d does not exist or is not implemented yet.\n",
                exercise_number);
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
