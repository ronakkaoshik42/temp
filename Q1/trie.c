#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include <string.h>
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 

trie_t init_trie(void){
    // Write your code here
    trie_t trie = (trie_t)calloc(1,sizeof(_trie_t));
    trie->head = (trie_node_t)calloc(1,sizeof(_trie_node_t));
    #ifdef _NO_HOH_LOCK_TRIE
        // Aquire lock for single and read and write lock case.
        #ifdef _S_LOCK_TRIE
            pthread_mutex_init(&(trie->trie_lock), NULL);
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_init(&(trie->trie_lock), NULL);
        #endif
    #endif
    return trie;
} 

trie_node_t init_node(void){

    trie_node_t node = (trie_node_t)calloc(1,sizeof(_trie_node_t));
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_init(&(node->node_lock), NULL);
    #endif
    return node;

}

void insert(trie_t trie, char* key, int value){
    // Write your code here
        
    #ifdef _NO_HOH_LOCK_TRIE
        // Aquire lock for single and read and write lock case.
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_wrlock(&(trie->trie_lock));
        #endif
        // printf("got the lock for key %s\n", key);

        int level, index;
        int key_len = strlen(key);
        trie_node_t curr_node = trie->head;
        trie_node_t  prev_node=NULL;
        
        for(level=0;level<key_len;level++)
        {
        
            prev_node = curr_node;
            index = CHAR_TO_INDEX(key[level]);
            curr_node = curr_node->children[index];
            if(!curr_node)
            {
                curr_node = init_node();
                prev_node->children[index] = curr_node;
            }
            
        }
        curr_node->value = value;
        curr_node->is_end = true;

        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->trie_lock));
        #endif
        return;

    #endif

    #ifndef _NO_HOH_LOCK_TRIE

        int level, index;
        int key_len = strlen(key);
        trie_node_t curr_node = NULL;
        trie_node_t  prev_node=NULL;
        
        pthread_mutex_lock(&((trie->head)->node_lock));
        curr_node = trie->head;

        for(level=0;level<key_len;level++)
        {
        
            prev_node = curr_node;
            index = CHAR_TO_INDEX(key[level]);
            curr_node = curr_node->children[index];
            if(!curr_node)
            {
                curr_node = init_node();
                prev_node->children[index] = curr_node;
            }
            pthread_mutex_lock(&(curr_node->node_lock));
            pthread_mutex_unlock(&(prev_node->node_lock));
        }
        curr_node->value = value;
        curr_node->is_end = true;
        pthread_mutex_unlock(&(curr_node->node_lock));
        return;
    #endif
}

int find(trie_t trie,char* key, int* val_ptr)
{
    // Write your code here

    #ifdef _NO_HOH_LOCK_TRIE
        // Aquire lock for single and read and write lock case.
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_rdlock(&(trie->trie_lock));
        #endif

        int level, index;
        int key_len = strlen(key);
        trie_node_t curr_node = trie->head;

        if(!curr_node)
        {
            #ifdef _S_LOCK_TRIE
                pthread_mutex_unlock(&(trie->trie_lock));
            #endif
            #ifndef _S_LOCK_TRIE
                pthread_rwlock_unlock(&(trie->trie_lock));
            #endif
            return -1;
        }

        for(level=0;level<key_len;level++)
        {
            index = CHAR_TO_INDEX(key[level]);
            if(!curr_node->children[index])
            {
                #ifdef _S_LOCK_TRIE
                    pthread_mutex_unlock(&(trie->trie_lock));
                #endif
                #ifndef _S_LOCK_TRIE
                    pthread_rwlock_unlock(&(trie->trie_lock));
                #endif
                return -1;
            }        
            curr_node = curr_node->children[index];
        }

        if(curr_node->is_end)
        {
            *val_ptr = curr_node->value;
            #ifdef _S_LOCK_TRIE
                pthread_mutex_unlock(&(trie->trie_lock));
            #endif
            #ifndef _S_LOCK_TRIE
                pthread_rwlock_unlock(&(trie->trie_lock));
            #endif
            return 0;
        }

        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->trie_lock));
        #endif
        return -1;

    #endif

    #ifndef _NO_HOH_LOCK_TRIE


        int level, index;
        int key_len = strlen(key);

        pthread_mutex_lock(&((trie->head)->node_lock));
        trie_node_t curr_node = trie->head;
        trie_node_t prev_node = NULL;

        if(!curr_node)
        {

            return -1;
        }

        for(level=0;level<key_len;level++)
        {
            prev_node = curr_node;
            index = CHAR_TO_INDEX(key[level]);
            if(!curr_node->children[index])
            {
                pthread_mutex_unlock(&curr_node->node_lock);
                return -1;
            }        
            curr_node = curr_node->children[index];
            pthread_mutex_lock(&curr_node->node_lock);
            pthread_mutex_unlock(&prev_node->node_lock);
        }

        if(curr_node->is_end)
        {
            *val_ptr = curr_node->value;
            pthread_mutex_unlock(&curr_node->node_lock);
            return 0;
        }
        pthread_mutex_unlock(&curr_node->node_lock);
        return -1;
    #endif
} 



bool isLeafNode(trie_node_t p)
{
    return p->is_end == 1;
}

bool isFreeNode(trie_node_t curr)
{
    int i;
    for(i = 0; i < 26; ++i)
    {
        if(curr->children[i])
            return false;
    }
    return true;
}

bool deleteHelper(trie_node_t root, char* key, int level, int len)
{
    int ind;
    //check if the key is in the trie.
    if(root!=NULL)
    {
        if(level==len)
        {
            //check if the key is in the trie.
            if(root->is_end==1)
            {
                root->is_end = 0;

                //Is it an unique key?
                            if(isFreeNode(root))
                {
                    return true;
                }
                return false;
            }               
        }
        else
        {
            ind = *(key+level) - 'a';
            if(deleteHelper(root->children[ind], key, level+1, len))
            {
                free(root->children[ind]);
                root->children[ind] = NULL;

                //check if the node is an unique key node. Case (2) and
                            //and case (4) can both generates unique key node. 
                            if(!isLeafNode(root) && isFreeNode(root))
                {
                    return true;
                }
                return false;
            }
        }
    }   
    return false;
}

void delete_kv(trie_t trie, char* key){
    // Write your code here

    #ifdef _NO_HOH_LOCK_TRIE
        // Aquire lock for single and read and write lock case.
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_wrlock(&(trie->trie_lock));
        #endif
        int len = strlen(key);
        if(len>0)
        {
            //check if the key is the only one in the trie
            if(deleteHelper(trie->head, key, 0, len))
            {
                free(trie->head);
            }
        }
        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->trie_lock));
        #endif
    #endif

    #ifndef _NO_HOH_LOCK_TRIE
        // Aquire lock for single and read and write lock case.

        int len = strlen(key);
        if(len>0)
        {
            //check if the key is the only one in the trie
            if(deleteHelper(trie->head, key, 0, len))
            {
                free(trie->head);
            }
        }

    #endif
}


int count = 0;
void display(trie_node_t root, char *str, int level,char* prefix,char **l) 
{ int i;
    if (root->is_end)  
    { 
        str[level] = '\0'; 
        l[count] = malloc(sizeof(char));
        strcat(l[count],str);
        count++;
    } 
    
    for (i = 0; i < 26; i++)  
    { 
        if (root->children[i])  
        { 
            str[level] = i + 'a'; 
            display(root->children[i], str, level + 1,prefix,l);   
        }
    } 
}  

int findpre(trie_t root,char* key,int *val_ptr){
    // Write your code here
    int level; 
    int length = strlen(key); 
    int index; 
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_lock(&(root->head->node_lock));
    #endif
    trie_node_t pCrawl = root->head; 
    trie_node_t pCrawl2 = NULL;

    for (level = 0; level < length; level++) 
    { 
        pCrawl2 = pCrawl;
        index = CHAR_TO_INDEX(key[level]); 
        if (!pCrawl->children[index]) 
        {   
          #ifndef _NO_HOH_LOCK_TRIE   
          pthread_mutex_unlock(&(pCrawl->node_lock));
          #endif
          return -1; 
        }
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&(pCrawl->children[index]->node_lock));
        pthread_mutex_unlock(&(pCrawl2->node_lock));
        #endif
        pCrawl = pCrawl->children[index]; 
    } 
    *val_ptr = pCrawl->value ;  
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_unlock(&(pCrawl->node_lock));
    #endif
    return 0;
} 



char** keys_with_prefix(trie_t trie, char* prefix){
    // Write your code here

    #ifdef _NO_HOH_LOCK_TRIE
        // Aquire lock for single and read and write lock case.
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_rdlock(&(trie->trie_lock));
        #endif
        char **list = malloc(1000*sizeof(char*));
        list[0] = malloc(sizeof(char));
        if (trie->head == NULL)
        {
            #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->trie_lock));
            #endif
            #ifndef _S_LOCK_TRIE
                pthread_rwlock_unlock(&(trie->trie_lock));
            #endif
            return list;
        }
        int level = 0; 
        char *str = malloc(sizeof(char));  
        int *ptr = (int*)malloc(sizeof(int));
        int x = findpre(trie,prefix,ptr);
        if(x==-1)
        {
            #ifdef _S_LOCK_TRIE
                pthread_mutex_unlock(&(trie->trie_lock));
            #endif
            #ifndef _S_LOCK_TRIE
                pthread_rwlock_unlock(&(trie->trie_lock));
            #endif
            return list;
        
        }
        
        trie_node_t temp = trie->head;
        while(*prefix)
        {
            temp = temp->children[*prefix-'a'];
            str[level] += *prefix; 
            level = level+1;
            prefix++;
        }
        display(temp, str,level,prefix,list); 
        count = 0; 

        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->trie_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->trie_lock));
        #endif
        return list;
    #endif

    #ifndef _NO_HOH_LOCK_TRIE
        char **list = malloc(1000*sizeof(char*));
        list[0] = malloc(sizeof(char));
        if (trie->head == NULL)
        {
            return list;
        }
        int level = 0; 
        char *str = malloc(sizeof(char));  
        int *ptr = (int*)malloc(sizeof(int));
        
        int x = findpre(trie,prefix,ptr);
        if(x==-1)
        {
        return list;
        }
        
        trie_node_t temp = trie->head;
        while(*prefix)
        {
        temp = temp->children[*prefix-'a'];
        str[level] += *prefix; 
        level = level+1;
        prefix++;
        }
        display(temp, str,level,prefix,list); 
        count = 0; 
        return list;
        
    #endif

}

// void delete_trie(trie_t trie){
//     // Write your code here
//     for (int i = 0; i < 26; i++)
//         if (trie->head->children[i])
//     { trie->head = trie->head->children[i];
//             delete_trie(trie);
//     }
//     free(trie->head);
// }

void delete_trie_helper(trie_node_t node)
{
    trie_node_t temp = node;
    for (int i = 0; i < 26; i++)
        if (node->children[i])
        { 
            node = node->children[i];
            delete_trie_helper(node);
        }
    free(temp);
}

void delete_trie(trie_t trie){
    // Write your code here
    trie_node_t temp=trie->head;

        if (temp)
        { 
            delete_trie_helper(temp);
        }
    free(trie->head);
}