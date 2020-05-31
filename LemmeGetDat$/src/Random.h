#pragma once

static unsigned int g_seed;

inline void fast_srand(unsigned int seed)
{
	g_seed = seed;
}

inline unsigned int fast_rand()
{
	g_seed = 214013 * g_seed + 2531011;
	return g_seed;
}
