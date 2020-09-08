#include "CombMath.h"

long long fact(int n) 
{
#ifdef _DEBUG
	long long oldFact = 1;
#endif
	long long currFact = 1;
	for (int i = n; i > 1; i--) 
	{
		currFact *= i;
#ifdef _DEBUG
		assert(oldFact < currFact);
		oldFact = currFact;
#endif
	}
	return currFact;
}

long long productSeries(int start, int end) 
{
#ifdef _DEBUG
	int oldRes = 1;
#endif
	int currRes = 1;
	for (int i = start; i <= end; i++) 
	{
	currRes *= i;
#ifdef _DEBUG
		assert(oldRes < currRes);
		oldRes = currRes;
#endif
	}
	return currRes;
}

int nCr(int n, int k) 
{
	if (n - k > k) 
	{
		return (int)(productSeries(n-k + 1, n) / fact(k));
	} else 
	{
		return (int)(productSeries(k + 1, n) / fact(n - k));
	}
}

int exp(int b, int n) 
{
	int currExp = 1;
	for (int i = 0; i < n; i++) 
	{
		currExp *= b;
	}
	return currExp;
}
