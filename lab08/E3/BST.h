#ifndef __BST_H
#define __BST_H

#include "quotazione.h"

typedef struct binarysearchtree *BST;
enum {PREORDER, INORDER, POSTORDER};



BST BST_init();
void BST_free(BST bst);
int BST_count(BST bst);
int BST_empty(BST bst);
Quotazione BST_search(BST bst, KeyQ k);
Quotazione BST_min(BST bst);
Quotazione BST_max(BST bst);
Quotazione BST_succ(BST bst, KeyQ k);
Quotazione BST_pred(BST bst, KeyQ k);
void BST_insertLeafR(BST bst, Quotazione x);
void BST_insertLeafI(BST bst, Quotazione val);
void BST_insertRoot(BST bst, Quotazione val);
void BST_visit(BST bst, int strategy);
Quotazione BST_select(BST bst, int k);
void BST_part(BST bst, int k);
void BST_delete(BST bst, KeyQ k);
void BST_balance(BST bst);
void BST_update(BST bst, Quotazione val);
void BST_searchQuotazioni(BST bst, KeyQ k1, KeyQ k2, Quotazione* q1, Quotazione *q2);
float BST_LongestOverShortest(BST bst);

#endif