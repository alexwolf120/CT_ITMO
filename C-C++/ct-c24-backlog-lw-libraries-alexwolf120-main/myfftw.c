#include "myfftw.h"

#include "return_codes.h"

#include <fftw3.h>
#include <stdlib.h>
Fast fast_cross_correlation(double *buf1, double *buf2, size_t n)
{
	Fast fast_sam;
	double *result = malloc(n * sizeof(double));
	fftw_complex *in1 = fftw_malloc(sizeof(fftw_complex) * 3 * n);
	fftw_complex *in2 = in1 + n;
	fftw_complex *result_complex = in2 + n;
	fftw_plan plan_f1 = NULL;
	fftw_plan plan_f2 = NULL;
	fftw_plan plan_b = NULL;
	if (result == NULL || in1 == NULL || in2 == NULL || result_complex == NULL)
	{
		fputs("Memory not allocated.\n", stderr);
		fast_sam.err = ERROR_NOTENOUGH_MEMORY;
		goto free_fast;
	}
	plan_f1 = fftw_plan_dft_r2c_1d(n, buf1, in1, FFTW_ESTIMATE);
	plan_f2 = fftw_plan_dft_r2c_1d(n, buf2, in2, FFTW_ESTIMATE);
	if (plan_f1 == NULL || plan_f2 == NULL)
	{
		fputs("The plan could not be created\n", stderr);
		fast_sam.err = ERROR_NOTENOUGH_MEMORY;
		goto free_fast;
	}
	fftw_execute(plan_f1);
	fftw_execute(plan_f2);
	for (size_t i = 0; i < n; i++)
	{
		result_complex[i][0] = in1[i][0] * in2[i][0] + in1[i][1] * in2[i][1];
		result_complex[i][1] = in1[i][1] * in2[i][0] - in1[i][0] * in2[i][1];
	}
	plan_b = fftw_plan_dft_c2r_1d(n, result_complex, result, FFTW_ESTIMATE);
	if (plan_b == NULL)
	{
		fputs("The plan could not be created\n", stderr);
		fast_sam.err = ERROR_NOTENOUGH_MEMORY;
		goto free_fast;
	}
	fftw_execute(plan_b);

	size_t max_index = 0;
	double max_value = 0;
	for (size_t i = 0; i < n; i++)
	{
		if (result[i] > max_value)
		{
			max_value = result[i];
			max_index = i;
		}
	}
	if (max_index < n / 2)
	{
		fast_sam.index = max_index;
	}
	else
	{
		fast_sam.index = max_index - n;
	}
	fast_sam.err = SUCCESS;
free_fast:
	if (result != NULL)
	{
		free(result);
	}
	if (in1 != NULL)
	{
		fftw_free(in1);
	}
	if (plan_b != NULL)
	{
		fftw_destroy_plan(plan_b);
	}
	if (plan_f1 != NULL)
	{
		fftw_destroy_plan(plan_f1);
	}
	if (plan_f2 != NULL)
	{
		fftw_destroy_plan(plan_f2);
	}
	fftw_cleanup();
	return fast_sam;
}
Over oversampling(size_t n, size_t m, double *buf)
{
	Over over_samp;
	double *result = malloc(m * sizeof(double));
	fftw_complex *in = fftw_malloc(sizeof(fftw_complex) * (n + m));
	fftw_complex *buf_new = in + n;
	fftw_plan plan_f1 = NULL;
	fftw_plan plan_b = NULL;
	if (buf_new == NULL || in == NULL || result == NULL)
	{
		fputs("Memory not allocated.\n", stderr);
		over_samp.err = ERROR_NOTENOUGH_MEMORY;
		goto free_over;
	}
	plan_f1 = fftw_plan_dft_r2c_1d(n, buf, in, FFTW_ESTIMATE);
	if (plan_f1 == NULL)
	{
		fputs("The plan could not be created\n", stderr);
		over_samp.err = ERROR_NOTENOUGH_MEMORY;
		goto free_over;
	}
	fftw_execute(plan_f1);
	for (size_t i = 0; i < n / 2; i++)
	{
		buf_new[i][0] = in[i][0];
		buf_new[i][1] = in[i][1];
	}
	for (size_t i = (n / 2 + m - n + 1); i < m; i++)
	{
		buf_new[i][0] = in[i - m + n][0];
		buf_new[i][1] = in[i - m + n][1];
	}
	if (n % 2 == 0)
	{
		buf_new[n / 2][0] = in[n / 2][0] / 2;
		buf_new[n / 2][1] = in[n / 2][1] / 2;
		for (size_t i = (n / 2 + 1); i < (n / 2 + m - n); i++)
		{
			buf_new[i][0] = 0;
			buf_new[i][1] = 0;
		}
		buf_new[n / 2 + m - n][0] = in[n / 2][0] / 2;
		buf_new[n / 2 + m - n][1] = in[n / 2][1] / 2;
	}
	else
	{
		buf_new[(n - 1) / 2][0] = in[(n - 1) / 2][0];
		buf_new[(n - 1) / 2][1] = in[(n - 1) / 2][1];
		for (size_t i = (n + 1) / 2; i < ((n + 1) / 2 + m - n); i++)
		{
			buf_new[i][0] = 0;
			buf_new[i][1] = 0;
		}
		buf_new[(n + 1) / 2 + m - n][0] = in[(n + 1) / 2 - m + n][0];
		buf_new[(n + 1) / 2 + m - n][1] = in[(n + 1) / 2 - m + n][1];
	}
	plan_b = fftw_plan_dft_c2r_1d(m, buf_new, result, FFTW_ESTIMATE);
	if (plan_b == NULL)
	{
		fputs("The plan could not be created\n", stderr);
		over_samp.err = ERROR_NOTENOUGH_MEMORY;
		goto free_over;
	}
	fftw_execute(plan_b);
	for (size_t i = 0; i < m; i++)
		result[i] /= m;
	over_samp.arr = result;
free_over:
	if (result != NULL)
	{
		free(result);
	}
	if (in != NULL)
	{
		free(in);
	}
	if (plan_b != NULL)
	{
		fftw_destroy_plan(plan_b);
	}
	if (plan_f1 != NULL)
	{
		fftw_destroy_plan(plan_f1);
	}
	fftw_cleanup();
	return over_samp;
}
