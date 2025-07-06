#include "RBTree.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fputs("Usage <input.dot> <output.dot>\n", stderr);
		return EXIT_FAILURE;
	}

	RBTree *tree = createRBTree();
	if (!tree)
	{
		fputs("Failed when create RBTree\n", stderr);
		return EXIT_FAILURE;
	}

	int err = EXIT_SUCCESS;
	FILE *output = NULL;
	if (createDotRBTree(tree, argv[1]))
	{
		err = EXIT_FAILURE;
		goto close_f;
	}
	output = fopen(argv[2], "w");
	if (!output)
	{
		fputs("Cannot open output file\n", stderr);
		err = EXIT_FAILURE;
		goto close_f;
	}
	if (printDot(tree, output))
	{
		fputs("Failed to write to output file\n", stderr);
		err = EXIT_FAILURE;
		goto close_f;
	}

close_f:
	if (output)
	{
		fclose(output);
	}
	if (tree)
	{
		freeRBTree(tree);
	}
	return err;
}
