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
Simp2DArray<T>::~Simp2DArray() 
{
	delete[] elems;
}

template <typename T>
T Simp2DArray<T>::get(int i, int j) 
{
#ifdef _DEBUG
	assert(i < numRows);
	assert(j < numCols);
#endif
	return elems[i * numCols + j];
}

template <typename T>
void Simp2DArray<T>::set(int i, int j, T newVal)
{
#ifdef _DEBUG
	assert(i < numRows);
	assert(j < numCols);
#endif
	elems[i * numCols + j] = newVal;
}
