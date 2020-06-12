#pragma once

#include "pch.h"

/*
Two Dimensional Array class with a specific "unique element quirk". 
That is, in addition to storing elements in a 2D array, an additional array is 
kept with all of the unique elements seen in the two dimensional array. 
This array can be used to obtain all of the unique elements in the TwoDimArray. 
However, as a constraint to this ability, the TwoDimArray is only able to accept
elements between 0 and numCols - 1. 

Additionally, elements must be added to a given row from left to right - 
it is not possible to add an element to an index in a row until all lower indices 
are set
*/
class TwoDimArray
{
private:
	int* elems;
	int* uniqueElems;
	int* arraySizes;
	int numUnique;
	int currUniqueIndex;

	int numRows;
	int numCols;

	void allocArray();
	void replaceArray(const TwoDimArray&);
	void deleteArray();
public:
	/*Constructors - allocate and set all elements in 2Darray to -1*/
	TwoDimArray(int numRows, int numCols);
	TwoDimArray();

	/*Copy and Assignement constructors: */
	TwoDimArray(const TwoDimArray&);
	TwoDimArray& operator=(const TwoDimArray&);

	~TwoDimArray();

	/*Return number of elements placed in a given row*/
	int getRowSize(int rowNum) const;

	/*Memeber variable Getters*/
	int getNumCols() const;
	int getNumRows() const;
	int getNumUnique() const;
	int getNextUnique();

	/*Get element of the 2D array at (i, j)*/
	int get(int i, int j) const;

	/*Set element of the 2D array at (i, j) to newVal. 
	It is required that newVal is in the range of 0 and 
	numCols-1 inclusive. Additionally, to set (i, j), 
	previous calls must have been made to everything in the set 
	{(i, j-1), (i,j-2), ..., (i, 0)}*/
	void set(int i, int j, int newVal);

	/*Increase the number rows in the array. 
	The maximum number of rows must be less than the number of columns however */
	void addRow();

	/*Set all elements in array to -1*/
	void resetArray();
	
	/*pack in elements of the passed row number such that there is no -1 between them and they are pushed leftward*/
	void pack(int row);
};

inline int TwoDimArray::getRowSize(int rowNum) const
{
	return arraySizes[rowNum];
}

inline int TwoDimArray::getNumCols() const
{
	return numCols;
}

inline int TwoDimArray::getNumRows() const
{
	return numRows;
}

inline int TwoDimArray::getNumUnique() const
{
	return numUnique;
}

inline int TwoDimArray::getNextUnique() 
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

inline int TwoDimArray::get(int i, int j) const
{
#ifdef _DEBUG
	assert(numCols * i + j < numRows * numCols);
#endif
	return elems[numCols * i + j];
}

inline void TwoDimArray::addRow() 
{
#ifdef _DEBUG
	assert(numRows < numCols);
#endif
	numRows++;
}
