#ifndef __ITEM_H
#define __ITEM_H

#define LEN (30+1)

typedef struct {
    char name[LEN];
    char net[LEN];
    int value;
} Item;

typedef char* Key;
typedef int Name;

int ITEM_checkNull(Item A);
Item ITEM_scan(FILE* fp);
Item ITEM_scanKey(FILE* fp);
void ITEM_show(Item A);
Item ITEM_setVoid();
Item ITEM_setNull();
int ITEM_getValue(Item A);


Key KEY_get (Item* A);
int KEY_cmp (Key k1, Key k2);
void KEY_show(Key k);



#endif