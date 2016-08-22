#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 4
#define TEXT "Hello World!"
#define LEN strlen(TEXT)

pthread_mutex_t indexMutex;
int i = 0;

int r() {
	return rand();
}

void printChars(void *threadId) {
	long tid = (long)threadId;
	for (;;) {
		pthread_mutex_lock(&indexMutex);
		if (i >= LEN) {
			printf("close %ld\n", tid);
			pthread_mutex_unlock(&indexMutex);
			pthread_exit(NULL);
		} else {
			printf("%c [%ld]\n", TEXT[i++], tid);
		}
		pthread_mutex_unlock(&indexMutex);
		usleep(rand() / 10000);
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	pthread_t threads[NUM_THREADS];

	int returnCode;
	long threadId;

	for (threadId = 0; threadId < NUM_THREADS; threadId++) {
		returnCode = pthread_create(
				&threads[threadId],
				NULL,
				(void *)&printChars,
				(void *)threadId
		);

		if (returnCode) {
			printf("ERROR %d\n", returnCode);
			exit(1);
		}
	}


	pthread_exit(NULL);
	return 0;
}
