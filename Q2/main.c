/*
INSTRUCTIONS:
This file must include only the main function
The main function should print out the various cases
and finally print out tables showing the performance
of various policies with the different workloads.
Vary the cache size.

You may include a function or 2 to ease the printing of tables.

*/

#include "definitions.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int max_cache_size =100;


int main()
{
  float FIFO[3][max_cache_size], RANDOM[3][max_cache_size], LRU[3][max_cache_size], LRU_APPX[3][max_cache_size];
  // LOOP = 0, RANDOM = 1, LOCAL = 2 Workload
  for(int i=0; i<3; i++) 
  {
    for(int c=1; c<=100; c++)
    {
      struct workload* load = generate_workload(i,100,10000); 
      FIFO[i][c] = policy_FIFO(load,c);
      RANDOM[i][c] = policy_RANDOM(load,c);
      LRU[i][c] = policy_LRU(load,c);
      LRU_APPX[i][c] = policy_LRUapprox(load,c);
    }
  }

    FILE* f1 = fopen("output.csv","w");
    for(int j=0;j<4;j++)
    {
      for(int k=0;k<3;k++)
      {
        for(int i=1;i<=100;i++)
        {
          if(j==0)
          {
            fprintf(f1,"%f,",FIFO[k][i]); 
          }
          else if(j==1)
          { 
            fprintf(f1,"%f,",RANDOM[k][i]);
          }

          else if(j==2)
          {
            fprintf(f1,"%f,",LRU[k][i]);
          }

          else if(j==3)
          {
            fprintf(f1,"%f,",LRU_APPX[k][i]);
          }
        }
        fprintf(f1,"\n");
      }
    }
    fclose(f1);

    return 0;
} 
