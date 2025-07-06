#ifndef MYFFPEG_H
#define MYFFPEG_H

#include <stdint.h>
#include <stdio.h>
typedef struct
{
	size_t err;
	char *str_error;
} AudioFile;

typedef struct
{
	size_t size;
	double *array;
	size_t err;
} Buffer;

typedef struct
{
	int32_t samples;
	int32_t sample_rate;
	int32_t time;
	size_t err;
} Cross_samp;

typedef struct
{
	size_t err;
	double *arr;
} Over;

typedef struct
{
	size_t err;
	size_t index;
} Fast;

Cross_samp cross(int argc, char *argv[]);

#endif	  // MYFFMPEG_H
