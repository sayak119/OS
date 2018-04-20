#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define totalcars  20
#define totalattenders  3
#define maxcapacity  7
#define totalpumps  4
#define totalmovement  3
//const int totalcars;
pthread_t threads[100];

sem_t max_capacity;int freepumps = totalpumps;sem_t mutex2;int freemovement=totalmovement;
sem_t mutexmovement;sem_t movement;sem_t attenderpump;int pump[totalpumps];sem_t cust_ready;

sem_t finished[totalcars];sem_t done[totalcars];
sem_t finishedcounter;sem_t leave_bpump[totalpumps];

//Queue Q1,Q2; /* Q1 pipe between car <->  attender
//                Q2 pipe between car <-> acceptPayment */
sem_t mutex3;sem_t mutex4;

//semaphore readywithpayment = 0; /* acceptPayment waits for car */
sem_t readywithpayment;
//semaphore receipt[50] /* car waits for acceptPayment,
//                         individualized for each car */
sem_t receipt[totalcars];

int pipe1[2],pipe2[2];

void write_pipe(writefd,value)
int writefd;
int value;
{
        if(write(writefd,&value,sizeof(int)) != sizeof(int)) {
                printf("pipe write error");exit(0);
        }
}

void read_pipe(readfd,value,status)
int readfd;
int *value;
int *status;
{
        int n;
        if( (n =  read(readfd,value,sizeof(int))) == -1) {
                printf ("pipe read error");
                exit(0);
        }
        *status = (char)n;
}

void car(int carId)
{
        int i;
        int k;
        sem_wait(&max_capacity);
        printf("car %d enters pump\n",carId);

        sem_wait(&mutex2);
        sem_wait(&mutexmovement);
        if ((freepumps == 0) || (freemovement < totalmovement)) {
                sem_post(&mutexmovement);
                sem_post(&mutex2);
                printf("car %d deciding what to read\n",carId);
                sem_wait(&mutexmovement);
                if (freemovement <= 0)
                {
                        printf("car %d reads S+G OS\n",carId);
                }
                freemovement--;
                sem_post(&mutexmovement);
                sem_wait(&movement);
                printf("car %d reads movement\n",carId);
                sem_wait(&attenderpump); /* read movement*/
                /*release movement*/
                sem_wait(&mutexmovement);
                freemovement++;
                sem_post(&mutexmovement);
                sem_post(&movement);
                printf("car %d released movement\n",carId);
        }
        else
        {
          sem_post(&mutexmovement);
          sem_post(&mutex2);
          sem_wait(&attenderpump);
        }

        /* attenderpumps, here we come to select */
        /*choose a free pump. pump[i] = -1 means it is unoccupied */
        sem_wait(&mutex2);
        i = 0;
        while ((pump[i] != -1) && (i < totalpumps)) i++;
        if (i == totalpumps)
        {
                printf("not possible because at least 1 spot empty");
                //printf("YOYOYOY");
                exit(0);
        }
        else
        {
                /* occupy pump i */
                pump[i] = carId;
                freepumps--;
                printf("car %d occupies pump %d. freepumps = %d\n",carId,i,freepumps);
        }
        sem_post(&mutex2);
        /*   and sem_post that we are ready */
        sem_wait(&mutex3);write_pipe(pipe1[1],carId);write_pipe(pipe1[1],i);sem_post(&mutex3);sem_post(&cust_ready);
        sem_wait(&finished[carId]);
        sem_wait(&mutex2);
        freepumps++;
        pump[i] = -1;
        sem_post(&leave_bpump[i]);sem_post(&mutex2);
        printf("car %d left pump %d. freepumps = %d\n",carId,i,freepumps);
        //payment
        sem_wait(&mutex4);write_pipe(pipe2[1],carId);sem_post(&mutex4);
        printf("car %d ready to pay\n",carId);

        sem_post(&readywithpayment);
        sem_wait(&receipt[carId]);

        //leavepump; /* animated procedure */
        printf("car %d leaving pump\n",carId);
        sem_post(&done[carId]);
        /* sem_post one less in pump, have a good day */
        sem_post(&max_capacity);
        if (carId == (totalcars-1)) {
                puts("served the last. wait for others to finish");
                for(k=0; k <= (totalcars-1); k++) sem_wait(&done[k]);
                puts("Done filling");
                exit(0);
        }
        else pthread_exit(0);
}

void attender(int number) {
        int mycar, mypump, status;
        while(1) {

                printf("attender %d: waiting for a call\n",number);
                /* we sleep till there is there a car reached on a pump
                   or there is a call from the cash counter */
                sem_wait(&cust_ready);sem_wait(&mutex3);
                read_pipe(pipe1[0],&mycar,&status);read_pipe(pipe1[0],&mypump,&status);
                sem_post(&mutex3);
                if (mycar != -1) {
                        //a regular car
                        //fillgas; may be a random delay here?
                        printf("attender %d fills gas of car %d on pump %d\n",number,mycar,mypump);
                        printf("Filling started\n");
                        sleep(1);
                        printf("gas filling finished of car %d by attender %d on pump %d\n",mycar,number,mypump);
                        sem_post(&finished[mycar]);sem_wait(&leave_bpump[mypump]);
                        printf("attender %d has seen  car %d leave pump %d\n",number,mycar,mypump);
                        /* now sem_post that a pump is free */
                        sem_post(&attenderpump);
                }
                else
                {
                        //acceptpayment;
                        printf("attender %d accepts payment\n",number);
                        sem_post(&finishedcounter);
                }
        }
}

void acceptPayment() {
        int car;
        int MyId = -1;
        int Mypumpno = 0;
        int status;
        while(1) {
                /* payment */
                sem_wait(&readywithpayment);
                sem_wait(&mutex4);
                read_pipe(pipe2[0],&car,&status);
                sem_post(&mutex4);
                printf("acceptPayment: car %d has arrived with payment. Calling a attender\n",car);
                /* first put our id on the demand Q to the attender*/
                sem_wait(&mutex3);
                write_pipe(pipe1[1],MyId);
                write_pipe(pipe1[1],Mypumpno);
                sem_post(&mutex3);
                sem_post(&cust_ready);
                printf("acceptPayment: Waiting to hear that car %d has paid to a attender\n",car);
                sem_wait(&finishedcounter);
                printf("acceptPayment: car %d has paid to a attender\n",car);
                sem_post(&receipt[car]); /* let go the right car */
        }
}
void carMaker()
{
        int i=0;
        printf("Started process\n\n");
        fflush(stdout);
        while (i < totalcars) {
                sleep(rand()%3);
                pthread_create(&threads[i+totalattenders],NULL,(void *)&car,(void *)i);
                i++;
                printf("car found %d\n",i);
        }
        pthread_exit(0);
}



int main()
{
        int iseed=time(NULL);
        srand(iseed);
        int i;
        pthread_t carM;
        sem_init(&max_capacity,0,maxcapacity);
        sem_init(&mutex2,0,1);
        sem_init(&mutex3,0,1);
        sem_init(&mutex4,0,1);
        sem_init(&mutexmovement,0,1);
        sem_init(&movement,0,totalmovement);
        sem_init(&attenderpump,0,totalpumps);
        sem_init(&cust_ready,0,0);
        sem_init(&finishedcounter,0,0);
        sem_init(&readywithpayment,0,0);

        for(i=0; i<totalcars; i++)
        {
                sem_init(&finished[i],0,0);
                sem_init(&receipt[i],0,0);
        }
        for(i=0; i<totalpumps; i++)
        {   sem_init(&leave_bpump[i],0,0); pump[i]=-1;}

        if (pipe(pipe1) < 0) printf("can't create pipe1\n");
        if (pipe(pipe2) < 0) printf("can't create pipe2\n");

        printf("creating attenders\n");
        i=1;
        while (i <= totalattenders) {
                pthread_create(&threads[i],NULL,(void *)&attender,(void *)i);
                i++;
        }
        printf("creating cars\n");
        i=0;
        printf("creating cars\n");
        i=0;
        pthread_create(&carM,NULL,(void *)&carMaker,NULL);
        acceptPayment();
        pthread_join(carM,NULL);

}
