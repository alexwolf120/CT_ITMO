#include "utils.h"

#include <graphviz/cgraph.h>

#include <stdio.h>
#include <stdlib.h>

static int fail(const char *message, Agraph_t *graph)
{
	fputs(message, stderr);
	if (graph)
		agclose(graph);
	return EXIT_FAILURE;
}
int createDotRBTree(RBTree *tree, const char *filename)
{
	if (!tree || !filename)
	{
		fputs("Invalid arguments\n", stderr);
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		fputs("Cannot open input file\n", stderr);
		return EXIT_FAILURE;
	}

	Agraph_t *graph = agread(fp, NULL);
	fclose(fp);

	if (!graph)
	{
		agerr(AGERR, "Cannot read graph\n");
		return EXIT_FAILURE;
	}

	for (Agnode_t *node = agfstnode(graph); node; node = agnxtnode(graph, node))
	{
		const char *name = agnameof(node);
		if (!name)
		{
			return fail("Node name is NULL\n", graph);
		}

		int num = atoi(name);
		if (num == 0 && name[0] != '0')
		{
			return fail("Invalid node name\n", graph);
		}

		if (insertRBTree(tree, num))
		{
			return fail("Failed insert num in tree\n", graph);
		}
	}

	agclose(graph);
	return EXIT_SUCCESS;
}
static void printEdgeToDot(Node *edge_node, FILE *file, int parent_num)
{
	if (edge_node)
	{
		fprintf(file, "    %d -- %d;\n", parent_num, edge_node->num);
		printTreeToDot(edge_node, file);
	}
}

static void printTreeToDot(Node *node, FILE *file)
{
	if (!node)
		return;

	const char *colorStr = (node->color == BLACK) ? "black" : "red";
	fprintf(file, "    %d\t[color=%s];\n", node->num, colorStr);

	printEdgeToDot(node->left, file, node->num);
	printEdgeToDot(node->right, file, node->num);
}
int printDot(RBTree *tree, FILE *file)
{
	if (!tree || !file)
		return EXIT_FAILURE;

	fprintf(file, "graph g {\n");
	if (tree->base)
	{
		printTreeToDot(tree->base, file);
	}
	fprintf(file, "}\n");

	return EXIT_SUCCESS;
}
