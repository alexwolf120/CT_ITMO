#include "RBTree.h"

#include <stdio.h>
#include <stdlib.h>

RBTree *createRBTree(void)
{
	RBTree *tree = malloc(sizeof(RBTree));
	if (!tree)
	{
		fputs("Failed to allocate memory for RBTree\n", stderr);
		return NULL;
	}
	tree->base = NULL;
	return tree;
}

static void leftSwap(RBTree *tree, Node *x)
{
	Node *y = x->right;
	x->right = y->left;

	if (y->left)
		y->left->parent = x;

	y->parent = x->parent;

	if (!x->parent)
		tree->base = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

static void rightSwap(RBTree *tree, Node *x)
{
	Node *y = x->left;
	x->left = y->right;

	if (y->right)
		y->right->parent = x;

	y->parent = x->parent;

	if (!x->parent)
		tree->base = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;

	y->right = x;
	x->parent = y;
}

static void fixRBTree(RBTree *tree, Node *node)
{
	while (node != tree->base && node->parent && node->parent->color == RED)
	{
		if (node->parent == node->parent->parent->left)
		{
			Node *uncle = node->parent->parent->right;
			if (uncle && uncle->color == RED)
			{
				node->parent->color = BLACK;
				uncle->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->right)
				{
					node = node->parent;
					leftSwap(tree, node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rightSwap(tree, node->parent->parent);
			}
		}
		else
		{
			Node *uncle = node->parent->parent->left;
			if (uncle && uncle->color == RED)
			{
				node->parent->color = BLACK;
				uncle->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->left)
				{
					node = node->parent;
					rightSwap(tree, node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				leftSwap(tree, node->parent->parent);
			}
		}
	}

	if (tree->base)
		tree->base->color = BLACK;
}
int insertRBTree(RBTree *tree, int num)
{
	if (!tree)
		return EXIT_FAILURE;

	Node *newNode = malloc(sizeof(Node));
	if (!newNode)
	{
		fputs("Failed to allocate memory for newNode\n", stderr);
		return EXIT_FAILURE;
	}

	newNode->num = num;
	newNode->color = RED;
	newNode->left = newNode->right = newNode->parent = NULL;

	if (!tree->base)
	{
		tree->base = newNode;
		newNode->color = BLACK;
	}
	else
	{
		Node *current = tree->base;
		Node *parent = NULL;

		while (current)
		{
			parent = current;
			if (num < current->num)
				current = current->left;
			else
				current = current->right;
		}
		if (!parent)
		{
			free(newNode);
			return EXIT_FAILURE;
		}
		newNode->parent = parent;
		if (num < parent->num)
			parent->left = newNode;
		else
			parent->right = newNode;

		fixRBTree(tree, newNode);
	}

	return EXIT_SUCCESS;
}

static void freeNodes(Node *node)
{
	if (!node)
		return;

	if (node->left)
	{
		freeNodes(node->left);
		node->left = NULL;
	}
	if (node->right)
	{
		freeNodes(node->right);
		node->right = NULL;
	}

	free(node);
}
void freeRBTree(RBTree *tree)
{
	if (!tree)
		return;

	if (tree->base)
	{
		freeNodes(tree->base);
		tree->base = NULL;
	}

	free(tree);
}
