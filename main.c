#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>

#define SEMNIMI "/testi"
sem_t *sem;

static int P(void) {
 /* implementation of down semaphore */
    fprintf(stderr, "Setting down semaphone \n");
        sem_wait(sem);

}
static int V(void) {
 /* implementation of up semaphore */
    fprintf(stderr, "Raising up semaphone\n");
        sem_post(sem);

}
int main (int argc, char* argv[]) {
/* if there is invalid number of arguments */
if ( argc != 3 ) {
 fprintf(stderr, "Usage: %s [increments] [sleeptime]\n", argv[0]);
 return 0;
}

int time, count;
count = atoi(argv[1]);
time = atoi(argv[2]);

int i;
/* create and initialize the semaphore */
sem = sem_open(SEMNIMI, O_RDWR, 0, 1);

if (sem == SEM_FAILED)
{
    if (errno == ENOENT)
        sem = sem_open(SEMNIMI, O_RDWR | O_CREAT | O_EXCL, 0777, 1);
    else
    {
    perror("Could not create or open semaphore");
    exit(0);
    }
}

// säikeiden välinen, alkuarvo 1

/* wait */
sleep(10);

/* argv[1] times */
for(i=0 ; i < count; i++ )
{
FILE *file;
int number;
/* Set semaphore down */
P();

/* open file for reading */
if ( !(file = fopen("file.txt","r")) ) {
fprintf(stderr,"Cannot open file: %s\n", strerror (errno));
return 0;
}

/* read int */
fscanf(file, "%i", &number);
fclose(file);
sleep(time);
number++;
/* open file for writing */
if ( !(file = fopen("file.txt","w")) ) {
 fprintf(stderr,"Cannot open file: %s\n", strerror (errno));
 return 0;
}

 /* write int */
 fprintf(file, "%i", number);
 fclose(file);
 printf("%d wrote %i to file\n", getpid(), number);

 /* Set semaphore up */
 V();

 number = 0;
 sleep(1);
 }
 sem_destroy(sem);
 return 1;
}
