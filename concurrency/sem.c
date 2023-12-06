#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> 
#include <assert.h>


sem_t *semaphore;

void *mythread(void *arg){

    sem_wait(semaphore);
    printf("[Thread %s] - just entered\n", (char *) arg);
    printf("[Thread %s] - Sleeping for 10 seconds...\n", (char *) arg);
    sleep(10);
    printf("Thread %s just left\n\n", (char *) arg);
    sem_post(semaphore);
    return NULL;
}

int main(int argc, char * argv[]){

    const char *semName = "/myUniqueSemaphoreName";
    int rc;

    printf("begin main\n");

    // Creating a named semaphore
    semaphore = sem_open(semName, O_CREAT, 0644, 1);

    if (semaphore == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    pthread_t thread1, thread2, thread3;

    rc = pthread_create(&thread1, NULL, mythread, "1"); assert(rc==0); 
    rc = pthread_create(&thread2, NULL, mythread, "2"); assert(rc==0); 
    rc = pthread_create(&thread3, NULL, mythread, "3"); assert(rc==0); 

    rc = pthread_join(thread1, NULL); assert(rc==0); 
    rc = pthread_join(thread2, NULL); assert(rc==0); 
    rc = pthread_join(thread3, NULL); assert(rc==0); 

    // Close and unlink the semaphore
    sem_close(semaphore);
    sem_unlink(semName);

    printf("main done.\n");

    return 0;
}