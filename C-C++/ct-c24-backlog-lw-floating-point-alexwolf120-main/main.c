#include "return_codes.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	uint32_t mantissa;
	int64_t exponent;
	int8_t sign;
	int8_t len_man;
	int16_t f_exp;
	char types_f;
} single_float;

uint64_t len_of_number(uint64_t num)
{
	int bin_len = 0;
	while (num > 0)
	{
		num /= 2;
		bin_len++;
	}
	return bin_len;
}
uint32_t num_is_NaN(single_float f)
{
	if (f.exponent == (f.f_exp * 2 + 1))
	{
		if (f.mantissa != 0)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	return 0;
}
single_float denorm_to_norm(single_float f)
{
	int exp_denorm = 0;
	while (!(f.mantissa & (1 << f.len_man)))
	{
		f.mantissa <<= 1;
		exp_denorm++;
	}
	f.mantissa = f.mantissa & ((1 << f.len_man) - 1);
	f.exponent = f.exponent + 2 - exp_denorm;
	f.mantissa >>= 1;
	return f;
}
void print_single_float(single_float f)
{
	int sign = f.sign;
	int exp = f.exponent;
	int32_t mantissa = f.mantissa;
	(f.types_f == 'f') ? (f.f_exp = 127) : (f.f_exp = 15);
	if (num_is_NaN(f) == 1)
	{
		printf("nan");
		return;
	}
	if (mantissa == 0)
	{
		if (exp == (f.f_exp * 2 + 1))
		{
			(sign) ? printf("-inf") : printf("inf");
			return;
		}
		else if ((exp == 0) && (f.types_f == 'f'))
		{
			printf("0x0.000000p+0");
			return;
		}
		else if ((exp >= 0) && (f.types_f == 'h'))
		{
			printf("0x0.000p+0");
			return;
		}
	}
	if (f.types_f == 'f')
	{
		printf("0x1.%06xp%+d\n", mantissa << 1, exp - f.f_exp);
		return;
	}
	else
	{
		printf("0x1.%03xp%+d\n", mantissa << 2, exp - f.f_exp);
		return;
	}
}

void single_float_round(int rounding, uint64_t frac_res, int32_t exponent, uint32_t sign, int len_mantiss, char format)
{
	uint32_t round_mantissa;
	round_mantissa =
		(frac_res & ((1 << (len_of_number(frac_res) - len_mantiss)) - 1)) | (1 << (len_of_number(frac_res) - len_mantiss));
	frac_res = frac_res >> (len_of_number(frac_res) - len_mantiss);
	frac_res &= ~(1ULL << (len_of_number(frac_res) - 1));
	uint32_t mantissa = frac_res;
	unsigned int round_bit = (round_mantissa >> (len_of_number(round_mantissa) - 2)) - 2;
	round_mantissa &= ~(1ULL << (len_of_number(round_mantissa) - 2));
	round_mantissa &= ~(1ULL << (len_of_number(round_mantissa) - 1));
	switch (rounding)
	{
	case 0:
		break;
	case 1:
		if (round_bit == 0x1)
		{
			if (round_mantissa == 0 && mantissa % 2 != 0)
			{
				if (sign == 0)
				{
					mantissa += 1;
				}
				else
				{
					mantissa -= 1;
				}
			}
			else if (round_mantissa != 0)
			{
				if (sign == 0)
				{
					mantissa += 1;
				}
				else
				{
					mantissa -= 1;
				}
			}
		}
		break;
	case 2:
		if (round_mantissa > 0)
		{
			if (sign == 0)
			{
				mantissa += 0x1;
			}
		}
		break;
	case 3:
		if (round_mantissa > 0)
		{
			if (sign == 1)
			{
				mantissa += 0x1;
			}
		}
		break;
	default:
		printf("Invalid rounding type\n");
	}
	exponent++;
	single_float res;
	res.sign = sign;
	res.exponent = exponent;
	res.mantissa = mantissa;
	res.types_f = format;
	print_single_float(res);
}

void single_float_add_substract(single_float a, single_float b, int rounding, char op)
{
	int sign = a.sign ^ b.sign;
	uint16_t exp_a = a.exponent;
	uint16_t exp_b = b.exponent;
	if (num_is_NaN(a) == 1 || num_is_NaN(b) == 1)
	{
		printf("nan");
		return;
	}
	else if (num_is_NaN(b) == 2)
	{
		if (op == '-')
		{
			(b.sign) ? printf("inf") : printf("-inf");
			return;
		}
		else
		{
			if (num_is_NaN(a) == 2 && a.sign == 0)
			{
				(b.sign) ? printf("nan") : printf("inf");
				return;
			}
			else
			{
				(b.sign) ? printf("-inf") : printf("inf");
				return;
			}
		}
	}
	else if (num_is_NaN(a) == 2)
	{
		(a.sign) ? printf("-inf") : printf("inf");
		return;
	}
	uint32_t frac_a = a.mantissa;
	uint32_t frac_b = b.mantissa;
	if (exp_a != 0)
	{
		frac_a = a.mantissa | (1 << (a.len_man - 1));
	}
	if (exp_b != 0)
	{
		frac_b = b.mantissa | (1 << (a.len_man - 1));
	}
	if (exp_a < exp_b)
	{
		int diff = exp_b - exp_a;
		frac_b <<= diff;
		exp_a = exp_b;
	}
	else if (exp_a > exp_b)
	{
		int diff = exp_a - exp_b;
		frac_a <<= diff;
		exp_b = exp_a;
	}

	uint64_t frac_res;
	bool isadd;
	isadd = (op == '+') && (sign == 0);
	isadd |= (op == '-') && (sign == 1);
	if (isadd)
	{
		frac_res = frac_a + frac_b;
		sign = a.sign;
	}
	else
	{
		(frac_a >= frac_b) ? (frac_res = frac_a - frac_b, sign = a.sign) : (frac_res = frac_b - frac_a, sign = !a.sign);
	}
	single_float_round(rounding, frac_res, exp_a, sign, a.len_man, a.types_f);
}
void single_float_multiply(single_float a, single_float b, int rounding)
{
	int sign = a.sign ^ b.sign;
	if (num_is_NaN(a) == 1 || num_is_NaN(b) == 1 || (num_is_NaN(a) == 2 && b.mantissa == 0 && b.exponent == 0) ||
		(num_is_NaN(b) == 2 && a.mantissa == 0 && a.exponent == 0))
	{
		printf("nan");
		return;
	}
	if (num_is_NaN(a) == 2 || num_is_NaN(b) == 2)
	{
		(sign) ? printf("-inf") : printf("inf");
		return;
	}

	int exp_a = a.exponent;
	int exp_b = b.exponent;
	uint64_t frac_a = a.mantissa;
	uint64_t frac_b = b.mantissa;

	if (exp_a != 0)
	{
		frac_a = a.mantissa | (1 << (a.len_man - 1));
	}
	if (exp_b != 0)
	{
		frac_b = b.mantissa | (1 << (a.len_man - 1));
	}
	uint64_t frac_res = frac_a * frac_b;
	int exp_res = exp_a + exp_b - a.f_exp;
	single_float_round(rounding, frac_res, exp_res, sign, a.len_man, a.types_f);
}
void single_float_divide(single_float a, single_float b, int rounding)
{
	int8_t sign = a.sign ^ b.sign;
	if (num_is_NaN(a) == 1 || num_is_NaN(b) == 1 || (a.mantissa == 0 && a.exponent == 0 && b.mantissa == 0 && b.exponent == 0) ||
		(num_is_NaN(a) == 2 && num_is_NaN(b) == 2))
	{
		printf("nan");
		return;
	}
	if (num_is_NaN(a) == 2 || num_is_NaN(b) == 2 || (b.mantissa == 0 && b.exponent == 0))
	{
		(sign) ? printf("-inf") : printf("inf");
		return;
	}
	int8_t exp_a = a.exponent;
	int8_t exp_b = b.exponent;
	uint32_t frac_a = a.mantissa;
	uint32_t frac_b = b.mantissa;
	if (exp_a != 0)
	{
		frac_a = a.mantissa | (1 << (a.len_man - 1));
	}
	if (exp_b != 0)
	{
		frac_b = b.mantissa | (1 << (a.len_man - 1));
	}
	uint64_t frac_res = (frac_a << a.len_man) / frac_b;

	uint32_t exp_res = exp_a - exp_b + a.f_exp;
	single_float_round(rounding, frac_res, exp_res, sign, a.len_man, a.types_f);
}

single_float single_float_from_hex(uint32_t value, char format)
{
	single_float f;
	if (format == 'f')
	{
		f.sign = (value >> 31) & 0x1;
		f.exponent = (value >> 23) & 0xff;
		f.mantissa = value & 0x7fffff;
		f.len_man = 24;
		f.f_exp = 127;
		f.types_f = 'f';
	}
	else
	{
		f.sign = (value >> 15) & 0x1;
		f.exponent = (value >> 10) & 0x1f;
		f.mantissa = value & 0x3ff;
		f.len_man = 11;
		f.f_exp = 15;
		f.types_f = 'h';
	}
	if (f.exponent == 0 && f.mantissa != 0)
	{
		f = denorm_to_norm(f);
	}

	return f;
}

int main(int argc, char *argv[])
{
	if (argc != 4 && argc != 6)
	{
		fprintf(stderr, "The cmd line's args or number of parameters (argv) is incorrect\n");
		return ERROR_ARGUMENTS_INVALID;
	}

	char format = argv[1][0];
	int8_t rounding = atoi(argv[2]);
	if (format != 'h' && format != 'f')
	{
		fprintf(stderr, "Incorrect format\n");
		return ERROR_ARGUMENTS_INVALID;
	}
	if (rounding < 0 || rounding > 3)
	{
		fprintf(stderr, "Incorrect args: round\n");
		return ERROR_ARGUMENTS_INVALID;
	}
	uint32_t value_f;
	if (sscanf(argv[3], "0x%x", &value_f) != 1)
	{
		fprintf(stderr, "Incorrect first arg\n");
		return ERROR_ARGUMENTS_INVALID;
	}
	single_float a, b;
	char op;

	if (argc == 4)
	{
		a = single_float_from_hex(value_f, format);
		print_single_float(a);
	}
	else if (argc == 6)
	{
		if (sscanf(argv[4], "%c", &op) != 1)
		{
			fprintf(stderr, "Incorrect first arg\n");
			return ERROR_ARGUMENTS_INVALID;
		}
		uint32_t value_s;
		if (sscanf(argv[5], "0x%x", &value_s) != 1)
		{
			fprintf(stderr, "Incorrect second arg\n");
			return ERROR_ARGUMENTS_INVALID;
		}
		a = single_float_from_hex(value_f, format);
		b = single_float_from_hex(value_s, format);
		switch (op)
		{
		case '+':
			single_float_add_substract(a, b, rounding, op);
			break;
		case '-':
			single_float_add_substract(a, b, rounding, op);
			break;
		case '*':
			single_float_multiply(a, b, rounding);
			break;
		case '/':
			single_float_divide(a, b, rounding);
			break;
		default:
			fprintf(stderr, "Incorrect operation\n");
			return ERROR_ARGUMENTS_INVALID;
		}
	}
	return SUCCESS;
}
