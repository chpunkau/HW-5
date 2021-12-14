#include<time.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>

#define SHARED 1

void* Ivanov();
void* Petrov();

// The number of full, empty and mutex value.
sem_t empty, sm, full;

void* NumIvanov();
int getRandomNumber();

int data;
int number;
int sizeNum;

void errMessage() {
	printf("incorrect qualifier value!\nWaited : command numberOfItems\n");
}

int main(int argc, char* argv[])
{
	if ((argc < 1 || argc > 2)) {
		errMessage();
		return 1;
	}

	printf("Start\n");

	pthread_t itid;
	pthread_t ptid;
	int x = 1;
	int* a = &x;

	if (argc == 1) {
		sem_init(&empty, SHARED, 1);
		sem_init(&full, SHARED, 0);
		sem_init(&sm, SHARED, 1);

		pthread_create(&itid, NULL, Ivanov, NULL);
		pthread_create(&ptid, NULL, Petrov, (void*)a);

		pthread_join(ptid, NULL);
	}

	if (argc != 1) {
		int size = atoi(argv[1]);
		if ((size < 1 || size>10000)) {
			errMessage();
			return 1;
		}

		sizeNum = size;
		sem_init(&empty, SHARED, 1);
		sem_init(&full, SHARED, 0);
		sem_init(&sm, SHARED, 1);

		pthread_create(&itid, NULL, NumIvanov, NULL);
		pthread_create(&ptid, NULL, Petrov, (void*)a);

		pthread_join(ptid, NULL);
	}

	printf("\nMain done\n");
}

void* Ivanov()
{
	srand((time(NULL)));
	int produced;
	printf("\nIvanov is ready to produce");
	int r = getRandomNumber(1, 1000);
	number = r;
	printf("\nNumber of item in storage is : %d", r);
	for (produced = 0; produced < r; produced++)
	{
		sem_wait(&empty);
		sem_wait(&sm);
		int item = getRandomNumber(1, 10000);
		data = item;
		sem_post(&sm);
		sem_post(&full);
		printf("\nIvanov found item on cost : %d", data);
	}
}

void* Petrov(void* no)
{
	srand((time(NULL)));
	int consumed, nechiporchuk = 0;
	int* thread = (int*)no;
	printf("\nPetrov is ready to consume");
	for (consumed = 0; consumed < number; consumed++)
	{
		sem_wait(&full);
		sem_wait(&sm);
		nechiporchuk = nechiporchuk + data;
		printf("\nPetrov consumed item on cost : %d", data);
		printf("\nNecheporchuk  counted items : %d", nechiporchuk);
		sem_post(&sm);
		sem_post(&empty);
	}
	printf("\nThe total cost of %d items in storage is %d\n", number, nechiporchuk);
}

void* NumIvanov() {
	srand((time(NULL)));
	int produced;
	printf("\nIvanov is ready");
	printf("\nNumber of item in storage is : %d", sizeNum);
	number = sizeNum;
	for (produced = 0; produced < sizeNum; produced++)
	{
		sem_wait(&empty);
		sem_wait(&sm);
		int item = getRandomNumber(1, 10000);
		data = item;
		sem_post(&sm);
		sem_post(&full);
		printf("\nIvanov found item on cost : %d", data);
	}
}

int getRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min;
}
