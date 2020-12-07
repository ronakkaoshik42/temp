/*
INSTRUCTIONS:

This file will contain all the functions related to the generation of workload

*/
#include <stdlib.h>
#include <time.h>
#include "definitions.h"

struct workload * generate_workload(WorkloadsT type, int pages, int size)
{
	struct workload *w;
	w = (struct workload *) malloc(sizeof(struct workload));
    w-> type = type;
	w->pages = pages;
	w->size = size;
    w->work = (int*)malloc(size*sizeof(int));

	if(type == 	RANDOM){
		w = generate_random(w);  //Generate Random Workload
	}
	else if(type == LOOP){
		w =  generate_loop(w);   //Generate Loop/Sequential Workload
	}
	else{
		w =  generate_local(w);  //Generate 80-20 Workload
	}
	return w;
}

struct workload * generate_random(struct workload * w)
{
    srand(time(0));
    int i;
    int temp;
    for(i=0 ; i< w->size ; i++){
        temp = (rand()% (w->pages));
        *(w->work+i) =  temp;
    }
	return w;
}

struct workload * generate_loop(struct workload * w)
{
    int i;
    int temp;
    for(i=0 ; i< w->size ; i++){
        temp = i%(w->pages);
        *(w->work+i) = temp;
    }
	return w;

}

struct workload * generate_local(struct workload * w)
{
	srand(time(0));
    int i;
    int temp;
    int p;

    for(i=0 ; i< w->size ; i++){
        p = rand()%100;
        if (p >= 80){
            temp = (rand() % ((w->pages)*4/5)) + ((w->pages)/5);
            *(w->work+i) = temp;
        }
        else{
            temp = (rand() % ((w->pages)/5));
            *(w->work+i) = temp;
        }
    }
	return w;
}