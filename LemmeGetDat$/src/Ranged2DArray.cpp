#include "Ranged2DArray.h"

void Ranged2DArray::allocArray()
{
	uniqueElems = new int[inputRange];
	elems = new int[numRows * numCols];
	rowSizes = new int[numRows];
}

Ranged2DArray::Ranged2DArray(int initNumRows, int initNumCols, int initInputRange) 
	: elems(nullptr), uniqueElems(nullptr), rowSizes(nullptr), numUnique(0), currUniqueIndex(-1), numRows(initNumRows), numCols(initNumCols), inputRange(initInputRange)
{
	allocArray();
	resetArray();
	numRows = initNumRows;
}

void Ranged2DArray::replaceArray(const Ranged2DArray& other) 
{
	allocArray();

	for (int i = 0; i < inputRange; i++) {
		uniqueElems[i] = other.uniqueElems[i];
	}

	for (int i = 0; i < numRows * numCols; i++) 
	{
		elems[i] = other.elems[i];
	}

	for (int i = 0; i < numRows;  i++) {
		rowSizes[i] = other.rowSizes[i];
	}
}

Ranged2DArray::Ranged2DArray(const Ranged2DArray& other)
{
	currUniqueIndex = other.currUniqueIndex;
	numUnique = other.numUnique;
	numRows = other.numRows;
	numCols = other.numCols;

	this->replaceArray(other);
}

void Ranged2DArray::deleteArray()
{
	delete[]uniqueElems;
	delete[]elems;
	delete[]rowSizes;
}

Ranged2DArray& Ranged2DArray::operator=(const Ranged2DArray& other) 
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

Ranged2DArray::~Ranged2DArray() 
{
	deleteArray();
}

void Ranged2DArray::resetArray() 
{
	for (int i = 0; i < numRows * numCols; i++) 
	{
		elems[i] = -1;
	}

	for (int i = 0; i < numRows; i++) 
	{
		rowSizes[i] = 0;
	}

	for (int i = 0; i < inputRange; i++) {
		uniqueElems[i] = -1;
	}

	numUnique = 0;
}

void Ranged2DArray::set(int i, int j, int newVal)
{
#ifdef _DEBUG
	assert(i < numRows);
	assert(j < numCols);
	assert(newVal < inputRange);
	assert(currUniqueIndex == -1);
	assert(j < rowSizes[i] + 1);
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

	if (rowSizes[i] == j) {
		rowSizes[i]++;
	}
	elems[numCols * i + j] = newVal;
}

void Ranged2DArray::appendToRow(int row, int newVal)
{
#ifdef _DEBUG
	assert(newVal > -1 && newVal < inputRange);
#endif
	set(row, rowSizes[row], newVal);
}

void Ranged2DArray::erase(int row, int col) 
{
#ifdef _DEBUG
	assert(col < rowSizes[row]);
#endif
	set(row, col, -1);
}

int Ranged2DArray::getNextUnique() 
{
	int nextElem = -2;
	do 
	{
		currUniqueIndex++;
		if (currUniqueIndex == numCols) {
			currUniqueIndex = -1;
			return -1;
		}
		nextElem = uniqueElems[currUniqueIndex];
	} while (nextElem == -1);

	return nextElem;
}

/*TODO: Test*/
void Ranged2DArray::pack(int row)
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
