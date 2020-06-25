#pragma once

#include "pch.h"

/*
Two Dimensional Array class with a specific "unique element quirk". 
That is, in addition to storing elements in a 2D array, an additional array is 
kept with all of the unique elements seen in the two dimensional array. 
This array can be used to obtain all of the unique elements in the Ranged2DArray. 
However, as a constraint to this ability, the Ranged2DArray is only able to accept
elements between 0 and numCols - 1. 

Additionally, elements must be added to a given row from left to right - 
it is not possible to add an element to an index in a row until all lower indices 
are set
*/
class Ranged2DArray
{
private:
	int* elems;
	int* uniqueElems;
	int* rowSizes;
	int numUnique;
	int currUniqueIndex;
	int inputRange;

	int numRows;
	int numCols;

	void allocArray();
	void replaceArray(const Ranged2DArray&);
	void deleteArray();
public:
	/*Constructors - allocate and set all elements in 2Darray to -1. 
	inputRange will be one larger than the largest allowed element. So if you want to accept a range of values from 0-9, inputRange = 10*/
	Ranged2DArray(int numRows, int numCols, int inputRange);
	Ranged2DArray() = delete;

	/*Copy and Assignement constructors: */
	Ranged2DArray(const Ranged2DArray&);
	Ranged2DArray& operator=(const Ranged2DArray&);

	~Ranged2DArray();

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
	inputRange. Additionally, if (i, j) is being set and
	previous calls must have been made to everything in the set 
	{(i, j-1), (i,j-2), ..., (i, 0)}, then it will be recorded 
	that the "rowSize" of that row will be slightly larger.
	
	It is recommended to use "appendToRow" and "erase" if you are 
	confused about the "set" functionality*/
	void set(int i, int j, int newVal);

	/*Append an additional value to a given row*/
	void appendToRow(int row, int newVal);

	/*Erase a given value in the array. 
	This will not move any other elements in the array as a "delete"
	command would*/
	void erase(int row, int col);

	/*Increase the number rows in the array. 
	The maximum number of rows must be less than the number of columns however */
	void addRow();

	/*Set all elements in array to -1*/
	void resetArray();
	
	/*pack in elements of the passed row number such that there is no -1 between them and they are pushed leftward*/
	void pack(int row);
};

inline int Ranged2DArray::getRowSize(int rowNum) const
{
	return rowSizes[rowNum];
}

inline int Ranged2DArray::getNumCols() const
{
	return numCols;
}

inline int Ranged2DArray::getNumRows() const
{
	return numRows;
}

inline int Ranged2DArray::getNumUnique() const
{
	return numUnique;
}

inline int Ranged2DArray::get(int i, int j) const
{
#ifdef _DEBUG
	assert(numCols * i + j < numRows * numCols);
#endif
	return elems[numCols * i + j];
}

inline void Ranged2DArray::addRow() 
{
	numRows++;
}
