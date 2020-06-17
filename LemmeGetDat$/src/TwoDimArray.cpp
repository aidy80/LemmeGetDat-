#include "TwoDimArray.h"

inline int TOTAL_NUM_ROWS(int numRows, int numCols) 
{
	return numRows < numCols - 1 ? numCols - 1 : numRows;
}

inline int ARRAY_SIZE(int numRows, int numCols) 
{
	return TOTAL_NUM_ROWS(numRows, numCols) * numCols;
}

void TwoDimArray::allocArray()
{
	uniqueElems = new int[numCols];
	elems = new int[ARRAY_SIZE(numRows, numCols)];
	arraySizes = new int[TOTAL_NUM_ROWS(numRows, numCols)];
}

TwoDimArray::TwoDimArray(int initNumRows, int initNumCols) 
	: elems(nullptr), uniqueElems(nullptr), numUnique(0), currUniqueIndex(-1), numRows(initNumRows), numCols(initNumCols)
{
	allocArray();
	resetArray();
	numRows = initNumRows;
}

TwoDimArray::TwoDimArray() : elems(nullptr), uniqueElems(nullptr), arraySizes(nullptr), numUnique(0), currUniqueIndex(-1), numRows(1), numCols(6)
{
#ifdef _DEBUG
	std::cerr << "Default constructor Running! (Shouldnt happen)" << std::endl;
#endif
	allocArray();
	resetArray();
	numRows = 1;
}

void TwoDimArray::replaceArray(const TwoDimArray& other) 
{
	allocArray();

	for (int i = 0; i < numCols; i++) {
		uniqueElems[i] = other.uniqueElems[i];
	}

	for (int i = 0; i < ARRAY_SIZE(numRows, numCols); i++) 
	{
		elems[i] = other.elems[i];
	}

	for (int i = 0; i < TOTAL_NUM_ROWS(numRows, numCols); i++) {
		arraySizes[i] = other.arraySizes[i];
	}
}

TwoDimArray::TwoDimArray(const TwoDimArray& other)
{
	currUniqueIndex = other.currUniqueIndex;
	numUnique = other.numUnique;
	numRows = other.numRows;
	numCols = other.numCols;

	this->replaceArray(other);
}

void TwoDimArray::deleteArray()
{
	delete[]uniqueElems;
	delete[]elems;
	delete[]arraySizes;
}

TwoDimArray& TwoDimArray::operator=(const TwoDimArray& other) 
{
	if (this != &other) {
		currUniqueIndex = other.currUniqueIndex;
		numUnique = other.numUnique;
		numRows = other.numRows;
		numCols = other.numCols;

		deleteArray();
		this->replaceArray(other);
	}

	return *this;
}

TwoDimArray::~TwoDimArray() 
{
	deleteArray();
}

void TwoDimArray::resetArray() 
{
	for (int i = 0; i < numRows * numCols; i++) 
	{
		elems[i] = -1;
	}

	for (int i = 0; i < TOTAL_NUM_ROWS(numRows, numCols); i++) 
	{
		arraySizes[i] = 0;
	}

	for (int i = 0; i < numCols; i++) {
		uniqueElems[i] = -1;
	}

	numRows = 1;
	numUnique = 0;
}

void TwoDimArray::set(int i, int j, int newVal)
{
#ifdef _DEBUG
	assert(numCols * i + j < (numRows+1) * numCols);
	assert(newVal < numCols);
	assert(currUniqueIndex == -1);
	assert(j < arraySizes[i] + 1);
	assert(i < numRows + 1);
	assert(newVal < numCols);
#endif
	if (newVal > -1) {
		if (uniqueElems[newVal] == -1) {
			uniqueElems[newVal] = newVal;
			numUnique++;
		}
	} else if (newVal == -1) 
	{
		uniqueElems[elems[numCols * i + j]] = newVal;
		numUnique--;
	}

	if (i == numRows && arraySizes[i] == 0)   
	{
		numRows++;
	}

	if (arraySizes[i] == j) {
		arraySizes[i]++;
	}
	elems[numCols * i + j] = newVal;
}



/*TODO: Test*/
void TwoDimArray::pack(int row)
{
	for (int i = 0; i < numCols; i++) 
	{
		if (elems[i] != -1) 
		{
			int j = i - 1;
			while(elems[j] == -1) 
			{
				j--;
			}
			if (elems[i - 1] == -1)
			{
				elems[j + 1] = elems[i];
				elems[i] = -1;
			}
		}
	}
}
