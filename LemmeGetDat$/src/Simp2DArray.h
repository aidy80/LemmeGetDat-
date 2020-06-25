#pragma once

#include "pch.h"

template <typename T>
class Simp2DArray
{
private:
	T* elems;
	int numRows;
	int numCols;

	int twoDto1D(int row, int col);
public:
	/*Basic Constructor/Desructor*/
	Simp2DArray(int numRows, int numCols);
	Simp2DArray() = delete;
	~Simp2DArray();

	/*Getter for row/column pair in the 2D array*/
	T get(int i, int j);
	
	/*Setter for row/column pair in the 2D array*/
	void set(int i, int j, T newVal);
};

#include "Simp2DArray.t"
