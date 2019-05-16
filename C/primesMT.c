/*
 * Michael Winberry
 * 
 * Sources:
 * http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
 * https://www.geeksforgeeks.org/sieve-of-eratosthenes/
*/

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgen.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

typedef struct BitBlock_s 
{
    __uint128_t bits;
    pthread_mutex_t mutex;
} BitBlock_t;

#define COMMAND_ARGS "hvt:u:"
#define DEFAULT_THREAD_COUNT 1
#define DEFAULT_UPPER_BOUND 10240
#define BITS_SIZE (long) sizeof(__uint128_t)

int even_up = 0;
int verbose = 0;
int array_size = 0;
int upper_bound = DEFAULT_UPPER_BOUND;
long num_threads = DEFAULT_THREAD_COUNT;

BitBlock_t **bit_blocks;

int get_bit(int num);
void set_bit(int num);
void print_help(void);
void free_blocks(void);
void clear_bit(int num);
void print_primes(void);
void destroy_mutexes(void);
void initialize_blocks(void);
void *sieve_of_eratosthenes(void *arg);
void join_threads(int thread_count, pthread_t *threads);
void create_threads(int thread_count, pthread_t *threads);

int main(int argc, char *argv[])
{
    pthread_t *threads = NULL;

    {
        int c;

        while((c = getopt(argc, argv, COMMAND_ARGS)) != -1)
        {
            switch(c)
            {
                case 'v': 
                    verbose = 1;
                    break;
                case 'h':
                    print_help();
                    exit(EXIT_SUCCESS);
                    break;
                case 'u':
                    upper_bound = atoi(optarg);
                    break;
                case 't':
                    num_threads = atoi(optarg);
                    break;
                case '?':
                    fprintf(stderr, "Invalid argument: %s\n", argv[c]);
                    exit(EXIT_FAILURE);
                    break;
                default:
                    print_help();
                    exit(EXIT_SUCCESS);
                    break;
            }
        }
    }

    array_size = (upper_bound/BITS_SIZE);
    threads = calloc(num_threads, sizeof(pthread_t));
    bit_blocks = malloc((array_size + 1)  * sizeof(BitBlock_t *));
    even_up = num_threads;
    if (even_up % 2 == 0)
        even_up--;

    initialize_blocks();
    create_threads(num_threads, threads);
    join_threads(num_threads, threads);
    
    destroy_mutexes();
    print_primes();
    free_blocks();
    
    free(threads);
    if(verbose >= 1)
    {
        fprintf(stderr, "Verbose: Freed %ld threads\n", num_threads);
        fprintf(stderr, "Exiting Successfully\n");
    }
    pthread_exit(EXIT_SUCCESS);
}


void *sieve_of_eratosthenes(void *arg)
{
    int i;
    long tid = (long) arg;
    int p = 3 + (tid * 2);
    for(; p*p <= upper_bound; p+=even_up)
    {
        if(get_bit(p) == 0)
        {
            for(i = p*p; i < upper_bound; i+=p)
            {
                set_bit(i);
            }
        }
    }
    pthread_exit(EXIT_SUCCESS);
}

void set_bit(int prime)
{
    int index = prime/BITS_SIZE;
    pthread_mutex_lock(&bit_blocks[index]->mutex);
    bit_blocks[index]->bits |= 1 << (prime%BITS_SIZE);
    pthread_mutex_unlock(&bit_blocks[index]->mutex);
}

void clear_bit(int number)
{
    int pos = number%BITS_SIZE;
    int index = number/BITS_SIZE;
    unsigned int flag = 1;
    flag = flag << pos;
    flag = -flag;
    pthread_mutex_lock(&bit_blocks[index]->mutex);
    bit_blocks[index]->bits = bit_blocks[index]->bits & flag;
    pthread_mutex_unlock(&bit_blocks[index]->mutex);
}

int get_bit(int number)
{
    int test = 0;
    int pos = number%BITS_SIZE;
    int index = number/BITS_SIZE;
    unsigned int flag = 1;

    flag = flag << pos;
    if( bit_blocks[index]->bits & flag )
        test = 1;
    return test;
}


void print_help(void)
{
    printf("Args: %s\n", COMMAND_ARGS);
    printf("v: enables verbose mode\n");
    printf("u: used to set upper bound defaults to 10240\n");
    printf("t: sets thread count, defaults to 1\n");
}

void join_threads(int thread_count, pthread_t *threads)
{
    long thread_id;
    for(thread_id = 0; thread_id < thread_count; thread_id++)
    {
        pthread_join(threads[thread_id], NULL);
    }
    if(verbose >= 1)
        fprintf(stderr, "Verbose: Joined %d threads\n", thread_count);
}

void create_threads(int thread_count, pthread_t *threads)
{
    long thread_id;
    for(thread_id = 0; thread_id < thread_count; thread_id++)
    {
        pthread_create(&threads[thread_id], NULL, sieve_of_eratosthenes, (void *) thread_id);
    }
    
    if(verbose >= 1)
        fprintf(stderr, "Verbose: Created %d threads\n", thread_count);
}

void initialize_blocks(void)
{
    int i;
    for(i = 0; i <= array_size; i++) 
    {
        bit_blocks[i] = malloc(sizeof(BitBlock_t));
        bit_blocks[i]->bits = 0;
        pthread_mutex_init(&bit_blocks[i]->mutex, NULL);
    }
    if(verbose >= 1)
        fprintf(stderr, "Verbose: Initialized %d BitBlock_t's\n", i);
}

void free_blocks(void)
{
    int i;
    for(i = 0; i <= array_size; i++)
    {
        free(bit_blocks[i]);
    }
    free(bit_blocks);
    if(verbose >= 1)
    {
        fprintf(stderr, "Verbose: Freed %d BitBlock_t's\n", i);
        fprintf(stderr, "Verbose: Freed array of BitBlock_t\n");
    }
}

void destroy_mutexes(void)
{
    int i;
    for(i = 0; i <= array_size; i++)
    {
        pthread_mutex_destroy(&bit_blocks[i]->mutex);
    } 
    if(verbose >= 1)
        fprintf(stderr, "Verbose: Destroyed %d Mutexes\n", i);
}

void print_primes(void)
{
    int i;
    int prime_count = 0;
    fprintf(stdout, "2\n");
    for (i = 3; i < upper_bound; i+=2) 
    {
        if(get_bit(i) == 0)
        {
            fprintf(stdout, "%d\n", i);
            prime_count++;
        }
    }
    if(verbose >= 1)
        fprintf(stderr, "Verbose: Printed %d primes\n", prime_count);
}