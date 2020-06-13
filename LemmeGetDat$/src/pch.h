#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <chrono>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <optional>
#include <thread>
#include <mutex>
#include <chrono>
#include <climits>
#include <stdint.h>

#define MULTITHREAD

//TODO: Optimizie TwoDimArray/bestHands for the many row, pairwise comparison case
//TODO: Figure out pots history
//TODO: End game when stack for traverser is zero
//TODO: Figure out how big blind can raise preflop
//TODO: Eliminate fold option when they can check (call)
//TODO: Figure out max raises, max potnums
