#include "myffmpeg.h"
#include "myfftw.h"
#include "return_codes.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
	{
		fputs("The cmd line's args or number of parameters (argv) is incorrect\n", stderr);
		return ERROR_ARGUMENTS_INVALID;
	}
	Cross_samp samp = cross(argc, argv);
	if (samp.err == SUCCESS)
	{
		printf("delta: %i samples\nsample rate: %i Hz\ndelta time: %i ms\n", samp.samples, samp.sample_rate, samp.time);
	}
	return samp.err;
}
