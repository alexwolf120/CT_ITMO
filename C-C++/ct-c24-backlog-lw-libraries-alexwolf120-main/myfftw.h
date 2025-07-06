#ifndef MYFFTW_H
#define MYFFTW_H
#include "myffmpeg.h"

#include <stdint.h>
#include <stdio.h>

Fast fast_cross_correlation(double *buf1, double *buf2, size_t n);

Over oversampling(size_t n, size_t m, double *buf);

#endif	  // MYFFTW_H
