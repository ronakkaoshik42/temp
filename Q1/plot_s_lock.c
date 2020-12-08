#define _S_LOCK_TRIE
#define _NO_HOH_LOCK_TRIE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "trie.h"
#include <time.h> 
#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define RED     "\033[31m"      /* Red */
#define ff fflush(stdout)

trie_t trie;

void test_file_diff(char* f1, char* f2){
    char command[200];
    sprintf(command,"cmp -s %s %s && echo \"\\033[32mPassed\" || echo \"\\033[31mFailure\"",f1, f2);
    system(command);
}

void* insert_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    int val;
    char ins_file_name[100];
    sprintf(ins_file_name, "./tests/multi_thread/initial/%d.txt",t);
    FILE* ins_file = fopen(ins_file_name,"r");
    while (1)
    {
        fscanf(ins_file,"%s",key);
        if (key[0]=='-') break;
        fscanf(ins_file,"%d",&val);
        insert(trie,key,val);
    }
    fclose(ins_file);
    return NULL;
}

void* find_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    char find_test_name[100];
    sprintf(find_test_name,"./tests/multi_thread/find/%d.txt",t);
    FILE* find_test = fopen(find_test_name,"r");
    char find_test_exp_name[100];
    sprintf(find_test_exp_name,"./.testout/exp_find_%d.txt",t);
    FILE* find_test_exp = fopen(find_test_exp_name,"w");
    while (1)
    {
        int f_ret = -101;
        fscanf(find_test,"%s",key);
        if (key[0]=='-') break;
        if(find(trie,key,&f_ret)==-1) fprintf(find_test_exp,"NO\n");
        else fprintf(find_test_exp,"YES\n");
    }
    fclose(find_test);
    fclose(find_test_exp);
    return NULL;
}


int main(int argc, char* argv[]){
    // Trie test begins
    trie = init_trie();
    char key[15];
    int val;
    int narr[1000];
    val = atoi(argv[1]);
    
    for(int i=1;i<101;i++)
    {
        narr[i-1]=i;
    }

    // Write Intensive
    clock_t t; 
    pthread_t ins_threads[val];
    int num_threads;
    num_threads = val;

    t = clock();        
    for(int j=0;j<num_threads;j++)
    {
        pthread_create(&ins_threads[j], NULL, insert_task,(void *)&narr[j]);
    } 
    int err;
    for(int j=0;j<num_threads;j++)
    {
        // pthread_join(ins_threads[j],NULL);
        err = pthread_detach(ins_threads[j]);
    }

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    FILE* f1 = fopen("./plot/s_lock_write.txt","a");
    fprintf(f1,"%d,%f\n",num_threads,time_taken);
    



    // Read Intensive
    clock_t t2; 
    pthread_t ins_threads2[val];
    int num_threads2;
    num_threads2 = val;

    t2 = clock();        
    for(int j=0;j<num_threads2;j++)
    {
        pthread_create(&ins_threads2[j], NULL, find_task,(void *)&narr[j]);
    } 
    int err2;
    for(int j=0;j<num_threads2;j++)
    {
        // pthread_join(ins_threads[j],NULL);
        err2 = pthread_detach(ins_threads2[j]);
    }

    t2 = clock() - t2; 
    double time_taken2 = ((double)t2)/CLOCKS_PER_SEC;
    FILE* f2 = fopen("./plot/s_lock_read.txt","a");
    fprintf(f2,"%d,%f\n",num_threads2,time_taken2);



    // Write-Read Intensive
    clock_t t3; 
    pthread_t ins_threads3[val];
    int num_threads3;
    num_threads3 = val;

    t3 = clock();        
    for(int j=0;j<(num_threads3)/2;j++)
    {
        pthread_create(&ins_threads3[j], NULL, insert_task,(void *)&narr[j]);
    } 
    for(int j=(num_threads3)/2;j<num_threads3;j++)
    {
        pthread_create(&ins_threads3[j], NULL, find_task,(void *)&narr[j]);
    } 
    int err3;
    for(int j=0;j<num_threads3;j++)
    {
        // pthread_join(ins_threads[j],NULL);
        err3 = pthread_detach(ins_threads3[j]);
    }

    t3 = clock() - t3; 
    double time_taken3 = ((double)t3)/CLOCKS_PER_SEC;
    FILE* f3 = fopen("./plot/s_lock_read_write.txt","a");
    fprintf(f3,"%d,%f\n",num_threads3,time_taken3);

    return 0;
}