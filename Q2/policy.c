/* INSTRUCTIONS:

This file will contain all functions related with various policies.
Each policy must return the hit rate

*/

#include "definitions.h"
#include <stdlib.h>

float policy_FIFO(struct workload *w, int cache_size)
{
	int hit_ctr = 0, cache_ctr = 0;
	int *cache = (int *)calloc(cache_size, sizeof(int));

	for (int i = 0; i < cache_size; i++) cache[i] = -1;
	for (int i = 0; i < w->size; i++)
	{
		int flag = 0;
		for (int j = 0; j < cache_size; j++)
		{
			if (w->work[i] == cache[j])
			{
				hit_ctr++;
				flag = 1;
				break;
			}
		}

		if (flag == 0)
		{
			cache[cache_ctr] = w->work[i];
			cache_ctr = (cache_ctr + 1) % cache_size;
		}
	}
	float hit_rate = (float)hit_ctr / w->size;
	free(cache);
	return 100*hit_rate;
}


float policy_RANDOM(struct workload * w, int cache_size)
{
    srand(0);
    int cache[cache_size], hits = 0, size = w->size, flag = 0, cache_ct = 0;
    float hit_rate = 0;

    for(int i=0; i<size; i++)
    {
        for(int j=0; j< cache_ct; j++)
        {
            if(*(w->work+i)==cache[j])
            {
                hits++;
                flag =1;
                break;
                }
            }
        if(flag==0)
        {
            if(cache_ct < cache_size)
            {
                cache[cache_ct] = *(w->work+i);
                cache_ct++;
            }
            else
            {
                int index = rand() % cache_size;
                cache[index] =  *(w->work+i);
            }
        }
        else
        {
            flag =0; 
        }
    }
    hit_rate = hits*100/(size);
    return hit_rate;
}

dll* init_dll(int cache_size)
{
    dll* tmp_ll = (dll*)malloc(sizeof(dll)); 
    tmp_ll->count = 0;
    tmp_ll->front = NULL;
    tmp_ll->rear = NULL;
    tmp_ll->cache_size = cache_size;
    return tmp_ll;
}

dl* init_dl(int page)
{
    dl* t = (dl*)malloc(sizeof(dl));
    t->page = page;
    t->prev = t->next = NULL;
    return t;
}

int check_dll(int page,dll* dll){
    int flag = 0;
    dl* temp;
    temp = dll->front;
    for(int i=0;i<dll->count;i++){
        if(temp->page == page){
            flag = 1;
            break;
        }
        if(temp->next == NULL){
            break;
        }
        temp = temp->next;
    }
    return flag;
}

dll* insert_dl(int page,dll* dll){
    int flag; 
    dl* temp;
    
    if(dll->count == 0)
    {
        dl* temp_dl = init_dl(page);
        temp_dl->page = page;
        dll->front = temp_dl;
        dll->rear = temp_dl;
        dll->count++;
    }
    else{
        temp = dll->front;
        for(int i=0;i<dll->count;i++){
            if(temp->page == page){
                flag = 1;
                break;
            }
            if(temp->next != NULL){
                temp = temp->next;
            }
            else break;
        }
        if(flag == 1){
            if(temp != dll->front){
                temp->prev->next = temp->next;
                if(temp != dll->rear)
                {
                    temp->next->prev = temp->prev;
                }
                else
                {
                    dll->rear = temp->prev;
                }
                temp->next = dll->front;
                dll->front->prev = temp;
                dll->front = temp;    
            }
        }
        else{
            dl* temp_dl = init_dl(page);
            temp_dl->page = page;
            temp_dl->next = dll->front;
            dll->front->prev = temp_dl;
            dll->front = temp_dl;
            
            if (dll->count < dll->cache_size){
                dll->count++;
            }

            else{
                dll->rear = dll->rear->prev;
            }
        }   
    }
    return dll;
}

float policy_LRU(struct workload * w, int cache_size)
{

    float size = w->size; 
    float hits = 0;       
    float hit_rate = 0;    
    dll* dll;

    dll = init_dll(cache_size); 
    for(int i=0; i<size; i++){
        if(check_dll(*(w->work+i),dll)){
            hits ++;
        }
        insert_dl(*(w->work+i),dll);

    }

    hit_rate = (hits/size) * 100 ;
	return hit_rate;
}

ll* init_ll(int cache_size){
    ll* link_l = (ll*)malloc(sizeof(ll)); 
    link_l->count = 0;
    link_l->front = NULL;
    link_l->rear = NULL;
    link_l->cache_size = cache_size;
    return link_l;
}

link* init_l(int page)
{
    link* t = (link*)malloc(sizeof(link));
    t->page = page;
    t->next = NULL;
    t->bitref = 1;
    return t;
}

int check_ll(int page,ll* ll){
    int flag = 0;
    link *temp = ll->front;

    for(int i=0;i<ll->count;i++){
        if(temp->page == page){
            flag = 1;
            break;
        }
        if(temp->next == NULL) break;
        temp = temp->next;
    }
    return flag;
}

ll * insert_l(int page, ll* ll){
    
    link* temp1; 

    int flag = 0;
    link *temp = ll->front;

    if (temp == NULL){
        link* new_link = init_l(page);
        ll->front = new_link;
        ll->rear = new_link;
        ll->count++;
    }

    else{
        for(int i=0;i<ll->count;i++){
            if(temp->page == page){
                flag = 1;
                break;
            }
            if(temp->next == NULL){
                break;
            }
            temp = temp->next;
        }

        if(flag == 0){
            
            link* new_link = init_l(page);
            
            if(ll->count < ll->cache_size){
                
                ll->rear->next = new_link;
                ll->rear = new_link;
                
                ll->count++;
            }
            else{
                if (ll->cache_size != 1){
                    while(ll->front->bitref == 1){
                        temp1 = ll->front;
                        ll->front = ll->front->next;
                        ll->rear->next = temp1;
                        ll->rear = temp1;
                        temp1->bitref = 0;
                    }
                    
                    ll->front = ll->front->next;
                    ll->rear->next = new_link;
                    ll->rear = new_link;
                }
                else{
                    ll->front = new_link;
                    ll->rear = new_link;
                }
            }
        }
        else if(flag == 1){
            temp->bitref = 1;
        }
    }
    return ll;
}

float policy_LRUapprox(struct workload * w, int cache_size)
{
	float size = w->size;   
    float hits = 0;         
    float hit_rate = 0;     
    ll* ll;
    ll = init_ll(cache_size);
    for(int i=0; i<size ;i++){
        if(check_ll(*(w->work+i),ll)){
            hits = hits+1;
        }
        insert_l(*(w->work+i),ll);
    }
    hit_rate = (hits/size) * 100;
	return hit_rate;
}


