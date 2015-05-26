/* Rickie Kerndt <rkerndt@uoregon.edu>
 * CIS 415 Spring 2014
 * Assignment 2, Problem 1 (6.40)
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 5
#define SEM_MODE S_IRWXU
#define MAX_THREADS 20
#define MAX_SLEEP 1000

/*
 * the buffer
 */

typedef int buffer_item_t;

typedef struct buffer {
    buffer_item_t   data[BUFFER_SIZE];
    pthread_mutex_t mutex;
    sem_t*          full;
    sem_t*          empty;
    int             in;
    int             out;
} buffer_t;

/*  Global Buffer */
buffer_t buffer;

/**
 * @brief buffer_init initializes mutex and semaphores on buffer
 * @return
 */
int buffer_init()
{
    memset(buffer.data, '\0', sizeof(buffer_item_t)*BUFFER_SIZE);
    buffer.in  = 0;
    buffer.out = 0;

    if((pthread_mutex_init(&buffer.mutex, NULL) == -1) ||
       ((buffer.full = sem_open("/FULL", O_CREAT, SEM_MODE, BUFFER_SIZE)) == SEM_FAILED) ||
       ((buffer.empty = sem_open("/EMPTY", O_CREAT, SEM_MODE, 0)) == SEM_FAILED))
    {
        return -1;
    }
    return 0;
}

/**
 * Close the semaphore to free resources
 */
void buffer_destroy()
{
   sem_close(buffer.full);
   sem_unlink("/FULL");
   sem_close(buffer.empty);
   sem_unlink("/EMPTY");
}

/**
 * @brief insert_item stores item is first open buffer slot
 * @param item pointer to buffer_item being placed in buffer
 * @return 0 on success, -1 on failure
 */
int insert_item(buffer_item_t* item)
{
    sem_wait(buffer.full);
    pthread_mutex_lock(&buffer.mutex);

    buffer.data[buffer.in] = *item;
    buffer.in = (buffer.in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&buffer.mutex);
    sem_post(buffer.empty);

    return 0;
}

/**
 * @brief remove_item returns item from buffer in FIFO
 * @param item pointer to buffer_item location to place value
 * @return 0 on success, -1 on failure
 */
int remove_item(buffer_item_t* item)
{
    sem_wait(buffer.empty);
    pthread_mutex_lock(&buffer.mutex);

    *item = buffer.data[buffer.out];
    buffer.out = (buffer.out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&buffer.mutex);
    sem_post(buffer.full);

    return 0;
}


/*
 * the producer/consumer
 */

/**1
 * @brief producer randomly sleeps and inserts random items into
 *        the bufer.
 */
void* producer()
{
    pid_t tid = syscall(SYS_gettid);
    pthread_t ptid = pthread_self();

    while(1)
    {
        /* sleep for a random 10-100 usec interval */
        int sleepy = (rand() % 90000) + 10000;
        usleep(sleepy);

        /* generate a random integer between 0-100 and insert into the buffer */
        buffer_item_t item = rand() % 100;
        if (insert_item(&item) == -1)
        {
            fprintf(stdout, "Producer [%d]: Failed to insert %d\n", tid, item);
            pthread_detach(ptid);
            pthread_exit(NULL);
        }
        else
        {
            fprintf(stdout, "Producer [%d]: produced %d\n", tid, item);
            fflush(stdout);
        }

    }
}

void* consumer()
{
    pid_t tid = syscall(SYS_gettid);
    pthread_t ptid = pthread_self();

    while(1)
    {
        /* sleep for a random 10-100 usec interval */
        int sleepy = (rand() % 90) + 10;
        usleep(sleepy);

        /* consume next item from the buffer */
        buffer_item_t item;
        if (remove_item(&item) == -1)
        {
            fprintf(stdout, "Consumer [%d]: Failed to remove item from the buffer\n", tid);
            pthread_detach(ptid);
            pthread_exit(NULL);
        }
        else
        {
            fprintf(stdout, "Consumer [%d]: Consumed %d\n", tid, item);
            fflush(stdout);
        }
    }
}

/**
 * @brief main
 * @return
 */
int main(int argc, char** argv)
{
     char usage[] = "USAGE: prob6_40 <seconds run time> <num producers> <num consumers>";
    if (argc != 4)
    {
        fprintf(stdout, "%s\n", usage);
        exit(EXIT_FAILURE);
    }

    int run_time      = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);
    int num_threads = num_producers + num_consumers;
    int tid_idx = 0;

    if (num_threads > MAX_THREADS)
    {
      fprintf(stderr, "%s\n", usage);
      exit(EXIT_FAILURE);
    }
    pthread_t tid[MAX_THREADS];
    pthread_t *p_tid = tid;
    pthread_attr_t tattr;
    pthread_attr_init(&tattr);

    if (buffer_init() == -1)    {
        exit(EXIT_FAILURE);
    }

    /* Create Producer Threads */
    for (int i = 0; i < num_producers; ++i)
    {
        int err = pthread_create(p_tid + tid_idx, &tattr, producer, NULL);
        if (err == 0) { ++tid_idx; }
    }

    /* Create Consumer Threads */
    for (int i = 0; i < num_consumers; ++i)
    {
        int err = pthread_create(p_tid + tid_idx, &tattr, consumer, NULL);
        if (err == 0) { ++tid_idx; }
    }

    /* Sleep */
    sleep(run_time);

   /* cancel threads */
   for (int i = 0; i < tid_idx; ++i)
   {
      pthread_cancel(p_tid[i]);
   }
   buffer_destroy();

   return EXIT_SUCCESS;
}

