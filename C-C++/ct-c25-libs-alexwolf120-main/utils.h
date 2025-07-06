#ifndef UTILS_H
#define UTILS_H

#include "RBTree.h"

int createDotRBTree(RBTree *tree, const char *filename);
int printDot(RBTree *tree, FILE *file);
void printTreeToDot(Node *node, FILE *file);

#endif
