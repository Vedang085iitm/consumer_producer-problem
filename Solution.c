#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
long long numfull = 0;
long long max = 0; // Maximum buffer size
long long N = 0;   // Maximum number of items produced by a producer thread
long long *buffer; // The shared buffer (adjust the size as needed)

// Additional variables to control consumer threads
long long items_per_consumer;
long long consumers_waiting = 0;
bool all_producers_finished = false;

// Function to check if the program should exit due to being stuck
bool shouldExit() {
    // Define a timeout threshold (in seconds)
    time_t timeout = 10; // Adjust this value as needed

    time_t current_time;
    time(&current_time);

    pthread_mutex_lock(&mtx);
    bool stuck = (consumers_waiting == items_per_consumer && numfull == 0 && !all_producers_finished);
    pthread_mutex_unlock(&mtx);

    if (stuck) {
        printf("Program is stuck; consumers_waiting=%lld, numfull=%lld, all_producers_finished=%d\n", consumers_waiting, numfull, all_producers_finished);
    }

    if (stuck && difftime(time(NULL), current_time) > timeout) {
        printf("Program exited due to being stuck.\n");
        return true;
    }

    return false;
}

void *producer(void *arg) {
    long long thread_num = *((long long *)arg);

    for (long long i = 0; i < N; i++) {
        long long item = 1000 + rand() % 9000;
        pthread_mutex_lock(&mtx);
        while (numfull >= max) {
            pthread_cond_wait(&empty, &mtx);
        }
        buffer[numfull] = item;
        numfull++;
        printf("Producer Thread #%lld; Item #%lld; Added Number: %lld\n", thread_num, i, item);
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mtx);
        sleep(rand() % 4);

        // Check for program exit
        if (shouldExit()) {
            printf("Program stuck; exiting...\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    long long thread_num = *((long long *)arg);
    long long items_consumed = 0;

    while (items_consumed < items_per_consumer || !all_producers_finished) {
        pthread_mutex_lock(&mtx);
        while (numfull == 0) {
            if (all_producers_finished) {
                pthread_mutex_unlock(&mtx);
                break;
            }
            consumers_waiting++;
            pthread_cond_wait(&fill, &mtx);
            consumers_waiting--;
        }
        if (numfull > 0) {
            long long item = buffer[numfull - 1];
            numfull--;
            printf("Consumer Thread #%lld; Item #%lld; Read Number: %lld\n", thread_num, items_consumed, item);
            items_consumed++;
            pthread_cond_signal(&empty);
        }
        pthread_mutex_unlock(&mtx);
        sleep(rand() % 5);

        // Check for program exit
        if (shouldExit()) {
            printf("Program stuck; exiting...\n");
            exit(EXIT_FAILURE);
        }
    }

    // Check if this consumer thread should exit early due to consuming enough items
    if (items_consumed >= items_per_consumer) {
        pthread_exit(NULL);
    }
}

int main(int argc, char *argv[]) {
    printf("#############Start#############\n");
    long long P = 0; // Number of producer threads
    long long C = 0; // Number of consumer threads
    long long S = 0; // Size of the buffer

    // Parse command-line arguments using getopt
    int opt;
    while ((opt = getopt(argc, argv, "p:c:s:n:")) != -1) {
        switch (opt) {
            case 'p':
                P = atoll(optarg);
                break;
            case 'c':
                C = atoll(optarg);
                break;
            case 's':
                S = atoll(optarg);
                break;
            case 'n':
                N = atoll(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -p <int> -c <int> -s <int> -n <int>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (P <= 0 || C <= 0 || S <= 0 || N <= 0) {
        fprintf(stderr, "All arguments must be positive integers.\n");
        exit(EXIT_FAILURE);
    }

    max = S;
    buffer = (long long *)malloc(S * sizeof(long long));
    items_per_consumer = (long long)(0.7 * N * P / C);

    // Initialize variables and create producer and consumer threads
    pthread_t producer_threads[P];
    pthread_t consumer_threads[C];
    long long producer_args[P];
    long long consumer_args[C];

    for (long long i = 0; i < P; i++) {
        producer_args[i] = i + 1;
        pthread_create(&producer_threads[i], NULL, producer, &producer_args[i]);
    }

    for (long long i = 0; i < C; i++) {
        consumer_args[i] = i + 1;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_args[i]);
    }

    // Wait for producer threads to finish
    for (long long i = 0; i < P; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    // Signal consumer threads that all producers have finished
    pthread_mutex_lock(&mtx);
    all_producers_finished = true;
    pthread_mutex_unlock(&mtx);
    pthread_cond_broadcast(&fill);

    // Wait for consumer threads to finish
    for (long long i = 0; i < C; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    if (shouldExit()) {
        printf("Program exited due to the reason mentioned above.\n");
    } else {
        printf("Program over...\n");
    }

    free(buffer);
    return 0;
}
