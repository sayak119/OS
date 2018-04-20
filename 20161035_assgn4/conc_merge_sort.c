#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

void swap(long long int *xp, long long int *yp)
{
    long long int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(long long int arr[], long long int n)
{
    long long int i, j, min_index;
    i = 0;
    while(i < n-1)
    {
        min_index = i;
        j = i+1;
        while(j < n)
        {
          if (arr[j] < arr[min_index])
          {
            min_index = j;
          }
          j++;
        }
        swap(&arr[min_index], &arr[i]);
        i++;
    }
    printf("selectionSort used\n");
}

void merge(long long int a[], long long int l1, long long int h1, long long int h2)
{
    long long int count=h2-l1+1,sorted[count],i=l1, k=h1+1, m=0,arr_count = l1;
    while (i<=h1 && k<=h2)
    {
        if (a[i]<a[k])
            sorted[m++]=a[i++];
        else if (a[k]<a[i])
            sorted[m++]=a[k++];
        else if (a[i]==a[k])
        {
            sorted[m++]=a[i++];
            sorted[m++]=a[k++];
        }
    }

    while (i<=h1)
        sorted[m++]=a[i++];
    while (k<=h2)
        sorted[m++]=a[k++];

    i=0;
    while(i<count)
    {
        a[l1] = sorted[i];
        i++;l1++;
    }
}

void mergeSort(long long int a[], long long int l, long long int h)
{
    long long int i, len=(h-l+1);
    int status;
    if (len<=5)
    {
        selectionSort(a+l, len);
        return;
    }
    pid_t left_pid,right_pid;
    left_pid = fork();
    if (left_pid<0)
    {
        perror("left child process forking error\n");
        _exit(-1);//used _exit so that child process does't delete temporary or external files of parent process when forking fails
    }
    else if (left_pid==0)
    {
        mergeSort(a,l,l+len/2-1);
        _exit(0);
    }
    else
    {
        right_pid = fork();
        if (right_pid<0)
        {
            perror("right child process forking error\n");
            _exit(-1);
        }
        else if(right_pid==0)
        {
            mergeSort(a,l+len/2,h);
            _exit(0);
        }
    }
    waitpid(left_pid, &status, 0);
    waitpid(right_pid, &status, 0);
    merge(a, l, l+len/2-1, h);
}

void sortCheck(long long int arr[], long long int len)
{
    if (len==1)
    {
        printf("Sorted\n");
        return;
    }

    long long int i=1;
    while(i<len)
    {
        if (arr[i]<arr[i-1])
        {
            printf("Sorting Not Done\n");
            return;
        }
        i++;
    }
    printf("Sorted\n");
    for(i=0; i<len; i++)
    {
      printf("%lld ",arr[i]);
    }
    printf("\n");
    return;
}

void consArray(long long int a[], long long int len) //creating random array
{
    long long int i;
    for (i=0; i<len; i++)
    {
        scanf("%lld",&a[i]);
    }
    return;
}

int main()
{
    long long int shmid,*shmArr,length;
    key_t key = IPC_PRIVATE;
    scanf("%lld",&length);
    size_t SHM_SIZE = sizeof(long long int)*length; //segment length
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) //segment creation
    {
        perror("shmget");
        _exit(1);
    }
    if ((shmArr = shmat(shmid, NULL, 0)) == (long long int *) -1)//segment attachment
    {
        perror("shmat");
        _exit(1);
    }
    srand(time(NULL));
    consArray(shmArr, length);
    mergeSort(shmArr, 0, length-1);
    sortCheck(shmArr, length);
    if (shmdt(shmArr) == -1)
    {
        perror("shmdt");
        _exit(1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1)//delete shared memory
    {
        perror("shmctl");
        _exit(1);
    }
    return 0;
}
