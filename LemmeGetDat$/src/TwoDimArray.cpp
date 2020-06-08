#include "TwoDimArray.h"

inline char TOTAL_NUM_ROWS(char numCols) 
{
	return numCols - 1;
}

inline char ARRAY_SIZE(char numCols) 
{
	return TOTAL_NUM_ROWS(numCols) * numCols;
}

void TwoDimArray::allocArray()
{
	uniqueElems = new char[numCols];
	elems = new char[ARRAY_SIZE(numCols)];
	arraySizes = new char[TOTAL_NUM_ROWS(numCols)];
}

TwoDimArray::TwoDimArray(char initNumRows, char initNumCols) 
	: elems(nullptr), uniqueElems(nullptr), numUnique(0), currUniqueIndex(-1), numRows(initNumRows), numCols(initNumCols)
{
#ifdef _DEBUG
	assert(numRows < numCols);
#endif
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

	for (int i = 0; i < ARRAY_SIZE(numCols); i++) 
	{
		elems[i] = other.elems[i];
	}

	for (int i = 0; i < TOTAL_NUM_ROWS(numCols); i++) {
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

	for (int i = 0; i < TOTAL_NUM_ROWS(numCols); i++) 
	{
		arraySizes[i] = 0;
	}

	for (int i = 0; i < numCols; i++) {
		uniqueElems[i] = -1;
	}

	numRows = 0;
	numUnique = 0;
}
