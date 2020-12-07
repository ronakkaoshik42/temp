/* 
INSTRUCTIONS:

All enum, structs and functions to be declared here.
If a definition is already declared, do not change it.
You may add extra definitions as required.

*/

#ifndef __DEF_H__
#define __DEF_H__

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL ENUMERATORS HERE */

// LOOP - Workload type for Loop
// RANDOM - Workload type for RANDOM
// LOCAL - 80-20 workload type

typedef enum enum_workloads{LOOP, RANDOM, LOCAL}WorkloadsT;

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL STRUCTS HERE */

struct workload
{
	// Defines type of workload
	WorkloadsT type;
	// The number of pages in workload
	int pages;
	// The total size of the workload
	int size;
	// The final work to run the policy
	int * work;
};

/* Additional Custom Data Structures */
typedef struct dl
{  
  struct dl *prev, *next;
  int page; 
}dl;

typedef struct dll
{         
  int count;                 
  int cache_size;        
  struct dl *front;
  struct dl *rear;
}dll;

typedef struct link
{ 
  struct link *next;
  int page; 
  int bitref;
}link;

typedef struct ll
{
  int count;
  int cache_size;
  struct link *front;
  struct link *rear;
}ll;


//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL FUNCTION DEFINITIONS HERE */

struct workload * generate_workload(WorkloadsT type, int pages, int size);
struct workload * generate_random(struct workload * w);
struct workload * generate_loop(struct workload * w);
struct workload * generate_local(struct workload * w);

float policy_FIFO(struct workload * w, int cache_size);
float policy_LRU(struct workload * w, int cache_size);
float policy_RANDOM(struct workload * w, int cache_size);
float policy_LRUapprox(struct workload * w, int cache_size);

/* Additional Custom Functions */
dll* init_dll(int cache_size);
dl* init_dl(int page);
int check_dll(int page,dll* dll);
dll* insert_dl(int page,dll* dll);
ll* init_ll(int cache_size);
link* init_l(int page);
ll * insert_l(int page, ll* ll);
int check_ll(int page,ll* ll);

#endif /* __DEF_H__ */
