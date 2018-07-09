#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

void *test_thread(void *arg)
{
    printf("Signal main thread...\n");
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t thread;

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread, NULL, test_thread, NULL);

    sleep(1);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    printf("Main thread signaled, quit...\n");
    return 0;
}
