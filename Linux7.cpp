#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int NUM_PROCESSES = 10;
pthread_mutex_t resourceMutex;
sem_t queueSemaphore;

void* accessResource(void* arg) {
    const char* color = static_cast<const char*>(arg);

    sem_wait(&queueSemaphore);
    pthread_mutex_lock(&resourceMutex);
    cout << "Thread " << pthread_self() << " (" << color << ") is accessing the resource.\n";
    sleep(1); // Simulating resource usage
    cout << "Thread " << pthread_self() << " (" << color << ") has finished using the resource.\n";
    pthread_mutex_unlock(&resourceMutex);
    sem_post(&queueSemaphore);

    return NULL;
}

int main() {
    srand(time(0));
    pthread_mutex_init(&resourceMutex, NULL);
    sem_init(&queueSemaphore, 0, 1);

    pthread_t processHandles[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        const char* color = (rand() % 2 == 0) ? "white" : "black";
        pthread_create(&processHandles[i], NULL, accessResource, (void*)color);
    }

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        pthread_join(processHandles[i], NULL);
    }

    pthread_mutex_destroy(&resourceMutex);
    sem_destroy(&queueSemaphore);

    return 0;
}
