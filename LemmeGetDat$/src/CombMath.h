#pragma once

#include <assert.h>

long long fact(int n);

int nCr(int n, int r);

int exp(int b, int n);

constexpr int sigma0(int end) 
{
#ifdef _DEBUG
	assert(end > -1);
#endif
	int sum = 0;
	for (int i = 1; i <= end; i++) 
	{
		sum += i;
	}
	return sum;
}
