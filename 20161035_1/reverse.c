#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>//has permission bits S_IRUSR|S_IWUSR
#include <sys/types.h>
#include <fcntl.h>// contains open(), read(), close() and write() functions

int main(int argc, char *argv[])
{
        int fp1 = open(argv[1],O_RDONLY);
        int fp2;
        char ch;
        struct stat buff = {0};
        long long int cnt;
        if(fp1)
        {
                if (stat("./Assignment", &buff) == -1)
                {
                        mkdir("./Assignment", 0700);//directory creation and giving permissions for Assignment directory
                }
                creat("./Assignment/output.txt",S_IRUSR|S_IWUSR);
                fp2=open("./Assignment/output.txt",O_WRONLY);
                chmod("./Assignment/output.txt", 0600);//read and write permissions for user for output.txt
                //lseek(fp1,-1L,1);
                cnt = lseek(fp1,(off_t)0,SEEK_END);
                long long int k=1000000;
                long long int cur=0;
                long long int num=cnt/k;
                long long int left=cnt-num*k;
                char arr1[1000000];
                char arr2[1000000];
                long long int j;
                cnt--;
                for (j=cnt; j>=0; )
                {
                        if(cur < num && j>=0)
                        {
                                lseek(fp1,(off_t)j - k,SEEK_SET);
                                cur++;
                                read(fp1,arr1,k);
                        }
                        else if (j>=0)
                        {
                                lseek(fp1,(off_t)0,SEEK_SET);
                                k = left;
                                read(fp1,arr1,k);
                        }
                        int i=0;
                        while(i<k)
                        {
                                if(arr1[i]>=65 && arr1[i]<=90 && j>=0)
                                        arr1[i]+=32;

                                else if(arr1[i]>=97 && arr1[i]<=122 && j>=0)
                                        arr1[i]-=32;
                                arr2[k-1-i] = arr1[i];
                                i++;
                        }

                        write(fp2,arr2,k);
                        j-=k;
                }
        }
        else
        {
                write(1,"Error\n",6);//Interprets the value of errno as an error message, and prints it to stderr
        }
        close(fp1);
        close(fp2);
        //closing the opened files
        return 0;
}
