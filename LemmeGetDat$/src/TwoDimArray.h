#pragma once

#include "pch.h"

class TwoDimArray
{
private:
	char* elems;
	char* uniqueElems;
	char* arraySizes;
	char numUnique;
	char currUniqueIndex;

	char numRows;
	char numCols;
public:
	TwoDimArray(char numRows, char numCols);
	TwoDimArray();
	~TwoDimArray();

	TwoDimArray(const TwoDimArray&);
	TwoDimArray& operator=(const TwoDimArray&);

	char getRowSize(int rowNum);
	char getNumCols();
	char getNumRows();
	char getNextUnique();
	char getNumUnique();
	char get(int i, int j);
	void set(int i, int j, char newVal);
	void addRow();
	void resetArray();
};

inline char TwoDimArray::getRowSize(int rowNum) 
{
	return arraySizes[rowNum];
}

inline char TwoDimArray::getNumCols() 
{
	return numCols;
}

inline char TwoDimArray::getNumRows()
{
	return numRows;
}

inline char TwoDimArray::getNextUnique() 
{
	char nextElem = -2;
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

inline char TwoDimArray::getNumUnique() 
{
	return numUnique;
}

inline char TwoDimArray::get(int i, int j)
{
#ifdef _DEBUG
	assert(numCols * i + j < numRows * numCols);
#endif
	return elems[numCols * i + j];
}

inline void TwoDimArray::set(int i, int j, char newVal)
{
#ifdef _DEBUG
	assert(numCols * i + j < numRows * numCols);
	assert(newVal < numCols);
	assert(currUniqueIndex == -1);
	assert(j < arraySizes[i] + 1);
	assert(newVal < numCols);
#endif
	if (newVal > -1) {
		if (uniqueElems[newVal] == -1) {
			uniqueElems[newVal] = newVal;
			numUnique++;
		}
	}
	if (arraySizes[i] == j) {
		arraySizes[i]++;
	}
	elems[numCols * i + j] = newVal;
}

inline void TwoDimArray::addRow() 
{
#ifdef _DEBUG
	assert(numRows < numCols);
#endif
	numRows++;
}
