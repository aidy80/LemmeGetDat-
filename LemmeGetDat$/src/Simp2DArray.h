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

	Simp2DArray(const Simp2DArray&) = delete;
	Simp2DArray& operator=(const Simp2DArray&) = delete;

	~Simp2DArray();

	Simp2DArray(Simp2DArray&&) noexcept;
	Simp2DArray& operator=(Simp2DArray&&) = delete;

	/*Getter for row/column pair in the 2D array*/
	T get(int i, int j);
	
	/*Setter for row/column pair in the 2D array*/
	void set(int i, int j, T newVal);
};


template <typename T>
inline int Simp2DArray<T>::twoDto1D(int row, int col)
{
	return row * numCols + col;
}

template <typename T>
Simp2DArray<T>::Simp2DArray(int numRows, int numCols)
	: numRows(numRows), numCols(numCols), elems(nullptr)
{
	elems = new T[numRows * numCols];
}

template <typename T>
Simp2DArray<T>::Simp2DArray(Simp2DArray&& other) noexcept
{
	numRows = other.numRows;
	numCols = other.numCols;
	elems = other.elems;
}

template <typename T>
Simp2DArray<T>::~Simp2DArray() 
{
	delete[] elems;
}

template <typename T>
inline T Simp2DArray<T>::get(int i, int j) 
{
#ifdef _DEBUG
	assert(i < numRows);
	assert(j < numCols);
#endif
	return elems[twoDto1D(i, j)];
}

template <typename T>
inline void Simp2DArray<T>::set(int i, int j, T newVal)
{
#ifdef _DEBUG
	assert(i < numRows);
	assert(j < numCols);
#endif
	elems[twoDto1D(i, j)] = newVal;
}

//#include "Simp2DArray.t"
