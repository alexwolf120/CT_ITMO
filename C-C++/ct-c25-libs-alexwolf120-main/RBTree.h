#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
typedef enum
{
	BLACK,
	RED
} Color;

typedef struct Node
{
	int num;
	Color color;
	struct Node *left, *right, *parent;
} Node;

typedef struct RBTree
{
	Node *base;
} RBTree;

RBTree *createRBTree(void);
int insertRBTree(RBTree *tree, int num);
void freeRBTree(RBTree *tree);

#endif
