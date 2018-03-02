#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/time.h>

FILE *key, *seek;
int *S,*K,*hit,seekSize,keySize,i=0,j=0,k1;
long keyFileSize,seekFileSize;
 struct timeval tm1, tm2, exec_tm;
long computeFileSize(FILE *file){
    int fileSize;
    fseek(file, 0, SEEK_END); // seek to end of file
    fileSize = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET);
    return fileSize;
}

void mem_lin (){
    gettimeofday( &tm1, NULL );
    key=fopen("key.db","rb");
    keyFileSize = computeFileSize(key);
    K=(int*)malloc(sizeof(int)*keyFileSize);
    seek=fopen("seek.db","rb");
    seekFileSize = computeFileSize(seek);
    S=(int*)malloc(sizeof(int)*seekFileSize);
    hit=(int*)malloc(sizeof(int)*seekFileSize);
    while(fread( &k1, sizeof( int ), 1, seek )){
       S[i] = k1;
       i++;
    }
    i=0;
    while(fread( &k1, sizeof( int ), 1, key )){
       K[i] = k1;
       i++;
    }
    seekSize=seekFileSize/sizeof(int);
    keySize=keyFileSize/sizeof(int);

    for (i=0;i<seekSize;i++){
        for (j=0;j<keySize;j++){
            if(S[i]==K[j]){
                hit[i]=1;
            }
        }
    }
    gettimeofday( &tm2, NULL );
    for(i=0;i<seekSize;i++)
    {
        if(hit[i]==1)
        {
            printf("%12d: Yes\n", S[i]);
        }
        else
        {
            printf("%12d: No\n",S[i]);
        }
    }
    timersub(&tm2, &tm1, &exec_tm);
    printf( "Time: %ld.%06ld", exec_tm.tv_sec, exec_tm.tv_usec );
}
int BSearch_mem(int arr[], int low, int high, int s){
    if (low<=high){
        int mid = low + (high-low)/2;
        if(arr[mid] == s)
            return 1;
        if(arr[mid]<s)
            return BSearch_mem(arr, mid + 1, high, s);
        else
            return BSearch_mem(arr, low, mid - 1, s);
    }
    return -1;
}
void mem_bin(){
    gettimeofday( &tm1, NULL );
    key=fopen("key.db","rb");
    keyFileSize = computeFileSize(key);
    K=(int*)malloc(sizeof(int)*keyFileSize);
    seek=fopen("seek.db","rb");
    seekFileSize = computeFileSize(seek);
    S=(int*)malloc(sizeof(int)*seekFileSize);
    hit=(int*)malloc(sizeof(int)*seekFileSize);
    i=0;
    while(fread( &k1, sizeof( int ), 1, seek )){
       S[i] = k1;
       i++;
    }
    i=0;
    while(fread( &k1, sizeof( int ), 1, key )){
       K[i] = k1;
       i++;
    }
    seekSize=seekFileSize/sizeof(int);
    keySize=keyFileSize/sizeof(int);
    for (i=0;i<seekSize;i++){
        if((BSearch_mem(K, 0, keySize-1, S[i]))==1)
            hit[i] = 1;
    }
    gettimeofday( &tm2, NULL );
    for(i=0;i<seekSize;i++)
    {
        if(hit[i]==1)
        {
            printf("%12d: Yes\n", S[i]);
        }
        else
        {
            printf("%12d: No\n",S[i]);
        }
    }
    timersub(&tm2, &tm1, &exec_tm);
    printf( "Time: %ld.%06ld", exec_tm.tv_sec, exec_tm.tv_usec );
}
void disk_lin(){
    gettimeofday( &tm1, NULL );
    key = fopen( "key.db", "rb" );
    seek = fopen( "seek.db", "rb" );
    seekFileSize = computeFileSize(seek);
    S=(int*)malloc(sizeof(int)*seekFileSize);
    hit=(int*)malloc(sizeof(int)*seekFileSize);
    while(fread( &k1, sizeof( int ), 1, seek )){
       S[i] = k1;
       i++;
    }
    seekSize=seekFileSize/sizeof(int);
    for (i=0;i<seekSize;i++){
       while(fread( &k1, sizeof( int ), 1, key )){
       if(S[i] == k1)
           hit[i] = 1;
       }
       fseek(key,0,SEEK_SET);
    }
    gettimeofday( &tm2, NULL );
    for(i=0;i<seekSize;i++)
    {
        if(hit[i]==1)
        {
            printf("%12d: Yes\n", S[i]);
        }
        else
        {
            printf("%12d: No\n",S[i]);
        }
    }
    timersub(&tm2, &tm1, &exec_tm);
    printf( "Time: %ld.%06ld", exec_tm.tv_sec, exec_tm.tv_usec );
}
int BSearch(FILE *key, int low, int high, int s){
    if (high>=low){
        int mid,k;
        mid = low + (high-low)/2;
        fseek(key,mid*sizeof(int),SEEK_SET);
        fread(&k,sizeof(int),1,key);
        if(k == s){
            return 1;
        }
        else if(k<s)
             return BSearch(key, mid + 1, high, s);
        else
            return BSearch(key, low, mid - 1, s);
    }
    return -1;
}
void disk_bin(){
    gettimeofday( &tm1, NULL );
    key = fopen( "key.db", "rb" );
    keyFileSize = computeFileSize(key);
    seek = fopen( "seek.db", "rb" );
    seekFileSize = computeFileSize(seek);
    S=(int*)malloc(sizeof(int)*seekFileSize);
    hit=(int*)malloc(sizeof(int)*seekFileSize);
    i =0;
    while(fread( &k1, sizeof( int ), 1, seek )){
       S[i] = k1;
       i++;
    }
    seekSize=seekFileSize/sizeof(int);
    keySize=keyFileSize/sizeof(int);
    for (i=0;i<seekSize;i++){
        if((BSearch(key, 0, keySize-1, S[i]))==1){
            hit[i] =1;
        }
    }
    gettimeofday( &tm2, NULL );
    for(i=0;i<seekSize;i++)
    {
        if(hit[i]==1)
        {
            printf("%12d: Yes\n", S[i]);
        }
        else
        {
            printf("%12d: No\n",S[i]);
        }
    }
    timersub(&tm2, &tm1, &exec_tm);
    printf( "Time: %ld.%06ld", exec_tm.tv_sec, exec_tm.tv_usec );

}
void main(int argc , char *argv[])
{
    if(strcmp("--mem-lin",argv[1])==0)
        mem_lin();
    else if(strcmp("--mem-bin",argv[1])==0)
        mem_bin();
    else if(strcmp("--disk-lin",argv[1])==0)
        disk_lin();
    else if(strcmp("--disk-bin",argv[1])==0)
        disk_bin();
    else
        printf("Invalid Argument");

}
