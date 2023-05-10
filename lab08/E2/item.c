#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "item.h"



int ITEM_checkNull(Item A){
    if(A.name[0] == '\0') return 1;
    return 0;
}

Item ITEM_scan(FILE* fp){
    Item tmp;
    fscanf(fp, "%s %s", tmp.name, tmp.net);
    return tmp;
}

Item ITEM_scanKey(FILE* fp){
    Item tmp;
    fscanf(fp, "%s", tmp.name);
    return tmp;
}

void ITEM_show(Item A) {
    printf("name: %s, net: %s \n", A.name, A.net);
}

Item ITEM_setVoid(){
    return (Item){"\0", "\0", -1};
}

Item ITEM_setNull(){
    return ITEM_setVoid();
}

int ITEM_getValue(Item A){
    return A.value;
}


Key KEY_get(Item* A){
    return A->name;
}

int KEY_cmp (Key k1, Key k2){
    return strcmp(k1, k2);
}

void KEY_show(Key k){
    printf("%s\n", k);
}
