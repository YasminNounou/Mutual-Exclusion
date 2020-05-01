#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include<math.h>
#include<semaphore.h>
#define MAX 5
#define SIZE 5


int counter=0;
int i;
pthread_mutex_t mutex;      //mutex thread
sem_t s;            //semaphore for the buffer full
sem_t n;            //semaphore for the buffer empty

//int *temp[MAX];
int buffer[10]; //buffer to hold value
int valin;      //variable to hold values for mmonitor
int valout;         //variable to hold for collector
int out = 0;        //index to consumer
int in = 0;         //index to producer
//int m;             //variable to hold in-out
int bcount;         //count number of elements in buffer

void thread(){
   while(1){
    int num = rand() % 10 +15;
   sleep(num);       //each thread Sleep for random number of seconds..

printf("Counter Thread : message received \n");
//printf("waiting to write..\n");

    pthread_mutex_lock( &mutex);        //lock mutex to access counter
    counter++;                      //increment counter per message
    printf("Counter value: %d\n",counter);  //print value of counter after this message

    pthread_mutex_unlock( &mutex);          //release mutex
}

}
//function responsible for looping the thread creation
void nmcounter()
{
    int number_of_threads = MAX;
    pthread_t thread_id[number_of_threads];
    for(i=0;i<number_of_threads;i++){
           //sleep each creation as to let time for interruption from other threads
            sleep(rand()%10);
        if(pthread_create( &thread_id[i], NULL, thread, NULL )){
      printf("Thread creation failed: \n");

    }
      // printf("cretaed thread number %d\n",i+1);
    }
    for(int j=0; j < number_of_threads; j++)
   {
        pthread_join( thread_id[j], NULL);
   }
}

//function of monitor to produce elements in buffer
void monitor(){

    while(1){           //infinite loop
int s2= rand() %10 +5;          //sleeps for random period then checks counter value
 sleep(s2);
   if (sem_trywait(&s)){            //try to wait and access buffer
    printf("\nMonitor Thread: waiting to write count \n");
   }
if(in==SIZE-1 && out==0) {       //if mintor is adding in the last place in buffer and consumer is still
                        //reading first element
            printf("\nbuffer is FUL!!\n");
            //return;
            }
else{
       // m=in-out;
    pthread_mutex_lock( &mutex);
    valin=counter;
    if(valin==0){       //checkes if counter value is 0 then no updates from messages
            printf("\nMonitor Thread: No messages received..\n\n");
    pthread_mutex_unlock( &mutex);
       // return 1;
    }
    else{
    counter=0;
    buffer[in]=valin;

    printf("\n********Monitor Thread: added value %d to position %d*******\n\n",valin,in);
    bcount++; // increment # of elements in buffer
    in++;
    in%=SIZE;       //for circular loop
    sem_post(&n);       //post semaphore to let the consumer in
    pthread_mutex_unlock( &mutex);
    }
}
    }
}

//function responsible for reading elemsnts out of buffer
void collector(){


while(1){

        int s3= rand() %10 +10;
        sleep(s3);

    sem_trywait(&n);
   // m=in-out;
    if(bcount==0){ //Buffer is empty
        printf("\n Buffer is empty!!\n\n");
        //return;
    }
else if(bcount>0){
        //start reading
        //printf("\n WASALL????????!!\n\n");
     valout=buffer[out]; //consumer element
    printf("\n********Collector Thread: Read value %d from position %d ***********\n\n",valout,out);

    out++;
    out%=SIZE; //for circular buffer
    bcount--; // decrements # of elements in buffer
   // pthread_mutex_unlock(&mutex);
    sem_post(&s);
}
else
//checking if consumer is faster than producer
    printf("\n*************************\n\nFATAL ERROR CONSUMER IS FASTER!!!!\n ************************");
   }
}

int main()
{
//    n=in-out;
//******   initializling the three threads **********
    pthread_t mmonitor;   //responsible for reading counter value and adding it to buffer
    pthread_t mcollector;   // read values from buffer
    pthread_t mcounter;     // threads for incrementing counter
    //pthread_t thread_id[number_of_threads];
    sem_init(&s,0,SIZE);          //initializing semaphores
	sem_init(&n,0,0);
    pthread_mutex_init(&mutex, NULL);
//while(1){

            //Creating threads

    int mer=pthread_create( &mmonitor, NULL, monitor, NULL );
    if(mer){
      printf("Thread mmonitor creation failed: %d\n", mer);
}

 int cer=pthread_create( &mcollector, NULL, collector, NULL );
    if(cer){
      printf("Thread mcollector creation failed: %d\n", cer);
}
    int er=pthread_create( &mcounter, NULL, nmcounter, NULL );
    if(er){
      printf("Thread mcounter creation failed: %d\n", er);
}

  // while(1){
  printf("\n\nHNAAAAAAAA\n\n");

  //                Joining threads
    pthread_join( mmonitor, NULL);
    pthread_join(mcollector,NULL);
//                 destroy all semaphores and mutex
    sem_destroy(&s);
	sem_destroy(&n);
    pthread_mutex_destroy(&mutex);

  //   printf("Hello world!\n");
    return 0;
}
