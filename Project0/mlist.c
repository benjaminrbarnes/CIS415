#include "mentry.h"
#include "mlist.h"

#define MAXSIZE 20

typedef struct mlistnode {
	struct mlistnode *next;
	MEntry *entry;
} Bucket;

struct mlist{
    unsigned long size;
    Bucket **map;
};


/* ml_create - created a new mailing list */
MList *ml_create(void){

    int i;
    MList *mailing_list;
    //Bucket *hashmap;

    if((mailing_list = (MList *)malloc(sizeof(MList))) == NULL) return mailing_list;

    mailing_list->size = 100;

    if((mailing_list->map = (Bucket **)malloc(mailing_list->size * sizeof(Bucket *))) == NULL) return mailing_list;

    for(i = 0; i < mailing_list->size; i++){
        mailing_list->map[i] = (Bucket *)malloc(sizeof(Bucket));
        mailing_list->map[i]->next = NULL;
    }
    return mailing_list;

}

/* ml_add - adds a new MEntry to the list;
 * returns 1 if successful, 0 if error (malloc)
 * returns 1 if it is a duplicate */
int ml_add(MList **ml, MEntry *me){
    MList *mailing_list;
    Bucket *q, *dest;
    unsigned long value;
    int i;

    mailing_list = *ml;

    if (ml_lookup(mailing_list, me) != NULL)
        return 1;

    // if we get to here, we have determinied it is not in the hash map so
    // now we need to get a hash value and find its spot. we also need to
    // confirm that it is not exceeding the length of 20 - if so, call resize
    // function
    if ((q = (Bucket *)malloc(sizeof(Bucket))) == NULL)
        return 0;
    q->next = NULL;
    q->entry = me;

    // get hash value
    value = me_hash(me, mailing_list->size);

    // get correct bucket
    dest = mailing_list->map[value];

    i = 0;
    while(dest->next != NULL){
        dest = dest->next;
        i++
    }
    dest->next = q;

    if(i >= MAXSIZE){
        if((mailing_list = resize(mailing_list, 2)) == NULL)
            return 0;
        return 1
    }
    return 1;
}

MList *resize(MList *ml, int scale){
    int i
    unsigned long new_size;
    Bucket *temp;
    MList *new_mailing_list;

    new_size = ml->size * scale;

    if((new_mailing_list = (MList *)malloc(sizeof(MList))) == NULL) return new_mailing_list;

    mailing_list->size = new_size;

    if((new_mailing_list->map = (Bucket **)malloc(new_mailing_list->size * sizeof(Bucket *))) == NULL) return new_mailing_list;

    for(i = 0; i < new_mailing_list->size; i++){
        new_mailing_list->map[i] = (Bucket *)malloc(sizeof(Bucket));
        new_mailing_list->map[i]->next = NULL;
    }

    // moving old entries to new mailing list
    for(i = 0; i < ml->size; i++){
        temp = ml->map[i];
        while(temp->next != NULL){
            me_add(new_mailing_list, temp->entry);
            temp = temp->next;
        }
    }

    ml_destroy(ml);

    return new_mailing_list;

}

/* ml_lookup - looks for MEntry in the list, returns matching entry or NULL */
MEntry *ml_lookup(MList *ml, MEntry *me){
    int i;
    unsigned long value;
    MList *mailing_list;
    Bucket *buck;

    mailing_list = ml;
    value = me_hash(me, mailing_list->size);

    buck = mailing_list->map[value];

    while(buck != NULL){
        if(me_compare(me, buck->entry) == 0)
            return buck->entry;
        buck = buck->next;
    }
}

/* ml_destroy - destroy the mailing list */
void ml_destroy(MList *ml){
    int i;
    Bucket *temp;

    for(i = 0; i < ml->size; i++){
        temp = ml->map[i];
        while(temp != NULL){
            Bucket *r = temp->next;
            me_destroy(temp->entry);
            free(temp);
            temp = r;
        }
        free(temp);
    }
    free(ml->map);
    free(ml->size);
    free(ml);

}