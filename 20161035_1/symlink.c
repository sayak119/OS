#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
struct stat st={0},fileStat;

int main(int argc, char *argv[])
{
								char a[]="./Assignment/output.txt",b[]="./symlink";
								chmod("./Assignment", 0700);
								symlink(a,b);//make symlink
								write(1,"Checking whether the directory has been created:",49);
								if (stat("./Assignment", &st) == -1)
								{
																write(1,"No\n",3);
								}
								else
								{
																write(1,"Yes\n",4);
								}
								//check that file and directory are present or not
								write(1,"Checking whether the file has been created:",43);
								if (stat("./Assignment/output.txt", &st) == -1)
								{
																write(1,"No\n",3);
								}
								else
								{
																write(1,"Yes\n",4);
								}
								write(1,"Checking whether the symlink has been created:",46);
								if (stat("./symlink", &st) == -1)
								{
																write(1,"No\n",3);
								}
								else
								{
																write(1,"Yes\n",4);
								}
								write(1,"\n\n",2);
								int j;
								long long int fp1;
								long long int fp2;
								long long int count;
								long long int flag=0;
								count=lseek(fp1,(off_t)0,SEEK_END);
								int k=1000000;
								int num=count/k;
								int rem=count%k;
								long long int left=count%k;
								long long int current=0;
								char ar1[1000000];
								char ar2[1000000];
								long long int temp=count;
								for(j=count; j>0; )
								{
																if(current<num && j>0)
																{
																								lseek(fp1,(off_t)(count-k),SEEK_SET);
																								lseek(fp2,(off_t)(temp-count),SEEK_SET);
																								read(fp1,ar1,k);
																								read(fp2,ar2,k);
																								current++;
																}
																else if (j>0)
																{
																								k=left;
																								lseek(fp1,(off_t)0,SEEK_SET);
																								lseek(fp2,(off_t)(temp-k),SEEK_SET);
																								read(fp1,ar1,k);
																								read(fp2,ar2,k);
																}
																int i=0;
																while(i<k)
																{
																								if(ar1[i]>=65 && ar1[i]<=90 && i<k)
																																ar1[i]+=32;

																								else if(ar1[i]>=97 && ar1[i]<=122 && i<k) //convert fp1 character to fp2 form
																																ar1[i]-=32;
																								if(ar1[i]!=ar2[k-i-1] && i<k)
																								{
																																flag = 1;
																								}
																								i++;
																}
																j-=k;
								}
								close(fp1);
								close(fp2);//closing the files
								write(1,"Checking whether file contents have been reversed and case-inverted :",69);
								if(flag)
								{
																write(1,"No\n\n",4);
								}
								else
								{
																write(1,"Yes\n\n",5);
								}
								chmod("./Assignment", 0600);
								char file[]="./Assignment/output.txt";
								if(stat(file,&fileStat) < 0)
								{
																return 1;
								}
								write(1,"User has read pemission on file :",33);
								if((fileStat.st_mode & S_IRUSR))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"User has write pemission on file :",34);
								if((fileStat.st_mode & S_IWUSR))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"User has execute pemission on file :",36);
								if((fileStat.st_mode & S_IXUSR))
								{
																write(1,"Yes",3);
								}
								else
								{
																write(1,"No",2);
								}
								write(1,"\n\n",2);

								write(1,"Group has read pemission on file :",34);
								if((fileStat.st_mode & S_IRGRP))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Group has write pemission on file :",35);
								if((fileStat.st_mode & S_IWGRP))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Group has execute pemission on file :",37);
								if((fileStat.st_mode & S_IXGRP))
								{
																write(1,"Yes",3);
								}
								else
								{
																write(1,"No",2);
								}
								write(1,"\n\n",2);
								write(1,"Others has read permission on file :",36);
								if((fileStat.st_mode & S_IROTH))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Others has write permission on file :",37);
								if((fileStat.st_mode & S_IWOTH))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Others has execute permission on file :",38);
								if((fileStat.st_mode & S_IXOTH))
								{
																write(1,"Yes",3);
								}
								else
								{
																write(1,"No",2);
								}
								write(1,"\n\n",2);


								char dir[]="./Assignment";
								if(stat(dir,&fileStat) < 0) return 1;
								write(1,"User has read pemission on directory :",38);
								if((fileStat.st_mode & S_IRUSR))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"User has write pemission on directory :",39);
								if((fileStat.st_mode & S_IWUSR))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"User has execute pemission on directory :",41);
								if((fileStat.st_mode & S_IXUSR))
								{
																write(1,"Yes",3);
								}
								else
								{
																write(1,"No",2);
								}
								write(1,"\n\n",2);
								write(1,"Group has read pemission on directory :",39);
								if((fileStat.st_mode & S_IRGRP))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Group has write pemission on directory :",40);
								if((fileStat.st_mode & S_IWGRP))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Group has execute pemission on directory :",42);
								if((fileStat.st_mode & S_IXGRP))
								{
																write(1,"Yes",3);
								}
								else
								{
																write(1,"No",2);
								}

								write(1,"\n\n",2);
								write(1,"Others has read permission on directory :",41);
								if((fileStat.st_mode & S_IROTH))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Others has write permission on directory :",42);
								if((fileStat.st_mode & S_IWOTH))
								{
																write(1,"Yes\n",4);
								}
								else
								{
																write(1,"No\n",3);
								}
								write(1,"Others has execute permission on directory :",44);
								if((fileStat.st_mode & S_IXOTH))
								{
																write(1,"Yes",3);
								}
								else
								{
																write(1,"No",2);
								}
								write(1,"\n\n",2);
								return 0;
}
