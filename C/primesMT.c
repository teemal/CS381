#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define DEFAULT_THREAD_COUNT 1
#define DEFAULT_UBOUND 10240

long numThreads = DEFAULT_THREAD_COUNT;
long uppperBound = DEFAULT_UBOUND;

typedef struct BitBlock_s {
	uint32_t bits;
	pthread_mutex_t mutex;
}BitBlock_t;

BitBlock_t *bitBlock;

void helper(void);
void *sieve(void *arg);
void setBit(BitBlock_t A[], long k);
int testBit(BitBlock_t A[], long k);
void bitFalse(BitBlock_t A[], long k);
void printStuff(BitBlock_t A[], long k);

int main(int argc, char *argv[]) {
	pthread_t *wthreads = NULL;
        long tid = 0;
        uint32_t i;
	long start = 3;

	{
		int c;
		while((c = getopt(argc, argv, "t:u:hv")) != -1) {
			switch(c) {
				case 't':
					numThreads = atoi(optarg);
					break;
				case 'u':
					uppperBound = atoi(optarg);
					break;
				case 'h':
					helper();
					break;
				case 'v':
					break;
				default:
					//oops
					break;
			}
		}
	}

	bitBlock = malloc(((uppperBound/32) + 1) * sizeof(BitBlock_t));
	wthreads = calloc(numThreads, sizeof(pthread_t));

	for(i = 0; i < (1+(uppperBound/32)); i++) {
		pthread_mutex_init(&bitBlock[i].mutex, NULL);
	}

	for(tid = 0; tid < numThreads; tid++) {
		pthread_create(&wthreads[tid], NULL, sieve, (void*) start);
		start += 2;
	}

	for(tid = 0; tid < numThreads; tid++) {
		pthread_join(wthreads[tid], NULL);
	}

	for(i = 0; i < (1+(uppperBound/32)); i++) {
		pthread_mutex_destroy(&bitBlock[i].mutex);
	}

	printStuff(bitBlock, uppperBound);

	free(bitBlock);
	free(wthreads);
	pthread_exit(EXIT_SUCCESS);
	//return 0;

}

void setBit(BitBlock_t A[], long k) {
	int i = k/32;
	int pos = k%32;
	unsigned int flag = 1;
	flag = flag << pos;
	A[i].bits = A[i].bits | flag;
}


int testBit(BitBlock_t A[], long k) {
	int i = k/32;
	int pos = k%32;
	unsigned int flag = 1;
	flag = flag << pos;
	if(A[i].bits & flag) {
		return 1;
	}else {
		return 0;
	}
}

void *sieve(void *arg) {
	long start = (long) arg;
	int i, j;

	for (i = start; i <= uppperBound; i += numThreads) {
		if(!testBit(bitBlock, i)) {
			for(j = i+i; j <= uppperBound; j+=i) {
				pthread_mutex_lock(&bitBlock[j/32].mutex);
				setBit(bitBlock, j);
				pthread_mutex_unlock(&bitBlock[j/32].mutex);
			}
		}
	}

	pthread_exit(EXIT_SUCCESS);

}

void printStuff(BitBlock_t A[], long k) {
	int i;
	printf("2\n");
	for(i = 3; i < k; i += 2) {
		if(!testBit(A, i)) {
			printf("%d\n", i);
		}
	}
}

void helper(void) {
	printf("-t : to add threads\n");
	printf("-u : to add an upper bound\n");
	printf("If no flags where added then the defualt thread is 1 and upper bound is 10240\n");
}
