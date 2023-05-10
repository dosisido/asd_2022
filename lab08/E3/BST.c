#include<stdio.h>
#include<stdlib.h>
#include "BST.h"

typedef struct BSTnode {
    Quotazione item;
    link p;
    link l;
    link r;
    int N;
}* link;
struct binarysearchtree {
    link root;
    link z;
};

static link NEW(Quotazione item, link p, link l, link r, int N);
static void treeFree(link h, link z);
static int treeCount(link h, link z);
static Quotazione treeSearchR(link h, KeyQ k, link z);
static Quotazione treeMin(link h, link z);
static Quotazione treeMax(link h, link z);
static Quotazione treeSucc(link h, KeyQ k, link z);
static Quotazione treePred(link h, KeyQ k, link z);
static link treeInsertLeafR(link h, Quotazione val, link z);
static void treePrint(link h, link z, int strategy);
static Quotazione treeSelect(link h, int k, link z);
static link rotR(link h);
static link rotL(link h);
static link treeInsertRoot(link h, Quotazione val, link z);
static link treePart(link h, int k);
static link treeDelete(link h, KeyQ k, link z);
static link treeJoin(link a, link b, link z);
static link treeBalance(link h, link z);
static void treeSearchQuotazioni(link h, KeyQ k1, KeyQ k2, Quotazione* min, Quotazione* max, link z);
static void treeSearchShortestLongest(link h, int* min, int* max, int actual, link z);




static link NEW(Quotazione item, link p, link l, link r, int N){
    link x = malloc(sizeof *x);
    x->item = item;
    x->p = p;
    x->l = l;
    x->r = r;
    x->N = N;
    return x;
}

BST BST_init(){
    BST bst = malloc(sizeof *bst) ;
    bst->root= ( bst->z = NEW(QUOTAZIONE_setNull(), NULL, NULL, NULL, 0));
    return bst;
}

void BST_free(BST bst){
    if(bst==NULL) return;
    treeFree(bst->root, bst->z);
    free(bst->z);
    free(bst);
}

static void treeFree(link h, link z){
    if(h==z) return;
    treeFree(h->l, z);
    treeFree(h->r, z);
    free(h);
}

int BST_count(BST bst){
    return treeCount(bst->root, bst->z);
}

static int treeCount(link h, link z){
    if(h==z) return 0;
    return 1 + treeCount(h->l, z) + treeCount(h->r, z);
}

int BST_empty(BST bst){
    return bst->root == bst->z;
}

Quotazione BST_search(BST bst, KeyQ k){
    return treeSearchR(bst->root, k, bst->z);
}

static Quotazione treeSearchR(link h, KeyQ k, link z){
    int cmp;
    if(h==z) return QUOTAZIONE_setNull();
    cmp = KEYQ_cmp(k, KEYQ_get(h->item));
    if(cmp==0) return h->item;
    if(cmp<0) return treeSearchR(h->l, k, z);
    return treeSearchR(h->r, k, z);
}

Quotazione BST_min(BST bst){
    return treeMin(bst->root, bst->z);
}

static Quotazione treeMin(link h, link z){
    if(h==z) return QUOTAZIONE_setNull();
    if(h->l==z) return h->item;
    return treeMin(h->l, z);
}

Quotazione BST_max(BST bst){
    return treeMax(bst->root, bst->z);
}

static Quotazione treeMax(link h, link z){
    if(h==z) return QUOTAZIONE_setNull();
    if(h->r==z) return h->item;
    return treeMax(h->r, z);
}

Quotazione BST_succ(BST bst, KeyQ k){
    return treeSucc(bst->root, k, bst->z);
}

static Quotazione treeSucc(link h, KeyQ k, link z){
    link p;
    if(h==z) return QUOTAZIONE_setNull();
    if(KEYQ_cmp(k, KEYQ_get(h->item)) == 0){
        if(h->r != z) return treeMin(h->r, z);
        p = h->p;
        while(p!=z && h==p->r){
            h = p;
            p = p->p;
        }
        return p->item;
    }
    if(KEYQ_cmp(k, KEYQ_get(h->item)) < 0)
        return treeSucc(h->l, k, z);
    return treeSucc(h->r, k, z);
}

Quotazione BST_pred(BST bst, KeyQ k){
    return treePred(bst->root, k, bst->z);
}

static Quotazione treePred(link h, KeyQ k, link z){
    link p;
    if(h==z) return QUOTAZIONE_setNull();
    if(KEYQ_cmp(k, KEYQ_get(h->item)) == 0){
        if(h->l != z) return treeMax(h->l, z);
        p = h->p;
        while(p!=z && h==p->l){
            h = p;
            p = p->p;
        }
        return p->item;
    }
    if(KEYQ_cmp(k, KEYQ_get(h->item)) < 0)
        return treePred(h->l, k, z);
    return treePred(h->r, k, z);
}

void BST_insertLeafR(BST bst, Quotazione x){
    bst->root = treeInsertLeafR(bst->root, x, bst->z);
}

static link treeInsertLeafR(link h, Quotazione val, link z){
    if(h==z) return NEW(val, z, z, z, 1);
    if(KEYQ_cmp(KEYQ_get(val), KEYQ_get(h->item)) < 0){
        h->l = treeInsertLeafR(h->l, val, z);
        h->l->p = h;
    }else{
        h->r = treeInsertLeafR(h->r, val, z);
        h->r->p = h;
    }
    (h->N)++;
    return h;
}

void BST_insertLeafI(BST bst, Quotazione val){
    link p = bst->root, h = p;
    if(bst->root == bst->z){
        bst->root = NEW(val, bst->z, bst->z, bst->z, 1);
        return;
    }

    while(h!=bst->z){
        p = h;
        if(KEYQ_cmp(KEYQ_get(val), KEYQ_get(h->item)) < 0)
            h = h->l;
        else
            h = h->r;
    }

    h = NEW(val, p, bst->z, bst->z, 1);
    if(KEYQ_cmp(KEYQ_get(val), KEYQ_get(p->item)) < 0)
        p->l = h;
    else
        p->r = h;
}

void BST_visit(BST bst, int strategy){
    if(BST_empty(bst)) return;
    treePrint(bst->root, bst->z, strategy);
}

static void treePrint(link h, link z, int strategy){
    if(h==z) return;
    if(strategy==PREORDER) QUOTAZIONE_show(h->item);
    treePrint(h->l, z, strategy);
    if(strategy==INORDER) QUOTAZIONE_show(h->item);
    treePrint(h->r, z, strategy);
    if(strategy==POSTORDER) QUOTAZIONE_show(h->item);
}

Quotazione BST_select(BST bst, int k){
    return treeSelect(bst->root, k, bst->z);
}

static Quotazione treeSelect(link h, int k, link z){
    int t;
    if(h==z) return QUOTAZIONE_setNull();
    t = h->l->N;
    if(t>k) return treeSelect(h->l, k, z);
    if(t<k) return treeSelect(h->r, k-t-1, z);
    return h->item;
}

static link rotR(link h){
    link x = h->l;
    h->l = x->r;
    x->r->p = h;
    x->r = h;
    x->p = h->p;
    h->p = x;

    x->N = h->N;
    h->N = 1;
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;
    return x;
}

static link rotL(link h){
    link x = h->r;
    h->r = x->l;
    x->l->p = h;
    x->l = h;
    x->p = h->p;
    h->p = x;

    x->N = h->N;
    h->N = 1;
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;
    return x;
}

void BST_insertRoot(BST bst, Quotazione val){
    bst->root = treeInsertRoot(bst->root, val, bst->z);
}

static link treeInsertRoot(link h, Quotazione val, link z){
    if(h==z) return NEW(val, z, z, z, 1);
    if(KEYQ_cmp(KEYQ_get(val), KEYQ_get(h->item))<0){
        h->l = treeInsertRoot(h->l, val, z);
        h = rotR(h);    
        h->N++;
    } else {
        h->r = treeInsertRoot(h->r, val, z);
        h = rotL(h);
        h->N++;
    }
    return h;
}

void BST_part(BST bst, int k){
    bst->root = treePart(bst->root, k);
}

static link treePart(link h, int k){
    int t = h->l->N;
    if(t>k){
        h->l = treePart(h->l, k);
        h = rotR(h);
    }
    if(t<k){
        h->r = treePart(h->r, k-t-1);
        h = rotL(h);
    }
    return h;
}

void BST_delete(BST bst, KeyQ k){
    bst->root = treeDelete(bst->root, k, bst->z);
}

static link treeDelete(link h, KeyQ k, link z){
    link y, p;
    if(h==z) return z;
    if(KEYQ_cmp(k, KEYQ_get(h->item)) < 0)
        h->l = treeDelete(h->l, k, z);
    if(KEYQ_cmp(k, KEYQ_get(h->item)) > 0)
        h->r = treeDelete(h->r, k, z);
    h->N--;
    if(KEYQ_cmp(k, KEYQ_get(h->item)) == 0){
        y = h;
        p = h->p;
        h = treeJoin(h->l, h->r, z);
        h->p = p;
        free(y);
    }
    return h;
}

static link treeJoin(link a, link b, link z){
    if(b==z) return a;
    b = treePart(b, 0);
    b->l = a;
    a->p = b;
    b->N = a->N + b->r->N + 1;
    return b;
}

void BST_balance(BST bst){
    bst->root = treeBalance(bst->root, bst->z);
}

static link treeBalance(link h, link z){
    int r;
    if(h==z) return z;
    r = (h->N+1)/2 -1;
    h = treePart(h, r);
    h->l = treeBalance(h->l, z);
    h->r = treeBalance(h->r, z);
    return h;
}

void BST_update(BST bst, Quotazione val){
    BST_delete(bst, KEYQ_get(val));
    BST_insertLeafR(bst, val);
}

void BST_searchQuotazioni(BST bst, KeyQ k1, KeyQ k2, Quotazione* q1, Quotazione *q2){
    // esplora tutto l'albero compreso tra i valori k1 e k2
    *q1 = QUOTAZIONE_setNull();
    *q2 = QUOTAZIONE_setNull();

    treeSearchQuotazioni(bst->root, k1, k2, q1, q2, bst->z);
}

static void treeSearchQuotazioni(link h, KeyQ k1, KeyQ k2, Quotazione* min, Quotazione* max, link z){
    if(h==z) return;
    if(KEYQ_cmp(KEYQ_get(h->item), k2) >= 0)                         // se la chiave è maggiore di k2 allora va a sinistra
        treeSearchQuotazioni(h->l, k1, k2, min, max, z);
    if(KEYQ_cmp(KEYQ_get(h->item), k1) <= 0)                         // se la chiave è minore di k1 allora va a destra
        treeSearchQuotazioni(h->r, k1, k2, min, max, z);

    if(QUOTAZIONE_isNull(*min) || QUOTAZIONE_value(h->item) < QUOTAZIONE_value(*min))
        *min = h->item;
    if(QUOTAZIONE_isNull(*max) || QUOTAZIONE_value(h->item) > QUOTAZIONE_value(*max))
        *max = h->item;
}

float BST_LongestOverShortest(BST bst){
    int min = 0;
    int max = 0;
    treeSearchShortestLongest(bst->root, &min, &max, 0, bst->z);
    return (float)max/(float)min;
}

static void treeSearchShortestLongest(link h, int* min, int* max, int actual, link z){
    if(h==z){
        if(*min== 0 || actual < *min)
            *min = actual;
        if(*max== 0 || actual > *max)
            *max = actual;
        return;
    }
    actual++;
    treeSearchShortestLongest(h->l, min, max, actual, z);
    treeSearchShortestLongest(h->r, min, max, actual, z);
}