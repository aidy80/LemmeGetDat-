#include "TwoDimArray.h"

inline char TOTAL_NUM_ROWS(char numCols) 
{
	return numCols - 1;
}

inline char ARRAY_SIZE(char numCols) 
{
	return TOTAL_NUM_ROWS(numCols) * numCols;
}

TwoDimArray::TwoDimArray(char initNumRows, char initNumCols) 
	: elems(nullptr), uniqueElems(nullptr), numUnique(0), currUniqueIndex(-1), numRows(initNumRows), numCols(initNumCols)
{
#ifdef _DEBUG
	assert(numRows < numCols);
#endif
	uniqueElems = new char[numCols];
	for (int i = 0; i < numCols; i++) {
		uniqueElems[i] = -1;
	}

	elems = new char[ARRAY_SIZE(numCols)];
	for (int i = 0; i < ARRAY_SIZE(numCols); i++) 
	{
		elems[i] = (char)(-1);
	}

	arraySizes = new char[TOTAL_NUM_ROWS(numCols)];
	for (int i = 0; i < TOTAL_NUM_ROWS(numCols); i++) {
		arraySizes[i] = 0;
	}
}

TwoDimArray::TwoDimArray() : elems(nullptr), uniqueElems(nullptr), arraySizes(nullptr), numUnique(0), currUniqueIndex(-1), numRows(1), numCols(6)
{
#ifdef _DEBUG
	std::cerr << "Default constructor" << std::endl;
#endif
	uniqueElems = new char[numCols];
	elems = new char[ARRAY_SIZE(numCols)];
	arraySizes = new char[TOTAL_NUM_ROWS(numCols)];
}

TwoDimArray::TwoDimArray(const TwoDimArray& other)
{
	currUniqueIndex = other.currUniqueIndex;
	numUnique = other.numUnique;
	numRows = other.numRows;
	numCols = other.numCols;

	uniqueElems = new char[numCols];
	for (int i = 0; i < numCols; i++) {
		uniqueElems[i] = other.uniqueElems[i];
	}

	elems = new char[ARRAY_SIZE(numCols)];
	for (int i = 0; i < ARRAY_SIZE(numCols); i++) 
	{
		elems[i] = other.elems[i];
	}

	arraySizes = new char[TOTAL_NUM_ROWS(numCols)];
	for (int i = 0; i < TOTAL_NUM_ROWS(numCols); i++) {
		arraySizes[i] = other.arraySizes[i];
	}
}

TwoDimArray& TwoDimArray::operator=(const TwoDimArray& other) 
{
	if (this != &other) {
		currUniqueIndex = other.currUniqueIndex;
		numUnique = other.numUnique;
		numRows = other.numRows;
		numCols = other.numCols;

		delete[]uniqueElems;
		uniqueElems = new char[numCols];
		for (int i = 0; i < numCols; i++) {
			uniqueElems[i] = other.uniqueElems[i];
		}

		delete[]elems;
		elems = new char[ARRAY_SIZE(numCols)];
		for (int i = 0; i < ARRAY_SIZE(numCols); i++)
		{
			elems[i] = other.elems[i];
		}

		delete[]arraySizes;
		arraySizes = new char[TOTAL_NUM_ROWS(numCols)];
		for (int i = 0; i < TOTAL_NUM_ROWS(numCols); i++) {
			arraySizes[i] = other.arraySizes[i];
		}
	}

	return *this;
}

TwoDimArray::~TwoDimArray() 
{
	delete[]elems;
	delete[]uniqueElems;
	delete[]arraySizes;
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

	int currIndex;
	while ((currIndex = getNextUnique()) != -1) 
	{
		uniqueElems[currIndex] = -1;
	}

	numRows = 0;
	numUnique = 0;
}
