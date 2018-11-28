
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

using namespace std;

template <typename Cell = double>
class Matrix
{
private:
	int rows, colmns;
	// cells матрица элементов
	// индекс с 0!!!
	Cell **cells;                               // элементы матрицы
	void AllocateCells(int row, int column);
	void FreeCells();
public:
	Matrix() : rows(0), colmns(0), cells(nullptr) {}	// Конструктор по умолчанию
	Matrix(const Matrix&);					    // Конструктор копирования
	Matrix(int N, int M);						// Конструктор нулевой матрицы
	Matrix(int N, int M, Cell*);				// Конструктор матрицы из списка
	~Matrix();

	Cell& operator()(int i, int j) { return cells[i - 1][j - 1]; }

	Matrix& operator = (const Matrix&);		//  Перегрузка оператора присваивания
	Matrix  operator + (const Matrix&);		//  Сложение матриц
	Matrix  operator - (const Matrix&);		//  Вычитание матриц
	Matrix  operator * (const Matrix&);		//  Умножение матриц

	friend istream& operator >> <Cell> (istream &, Matrix&);		// Перегрузка оператора >> для ввода матрицы
	friend ostream& operator << <Cell> (ostream &, const Matrix&);	// Перегрузка оператора << для вывода матрицы

	double getElement(int i, int j) {
		return cells[i][j];
	}
	int getRowSize() { return rows; }
	int getColmnSize() { return colmns; }
};

template <typename Cell>
Matrix<Cell>::Matrix(const Matrix<Cell>& m)
{
	AllocateCells(m.rows, m.colmns);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < colmns; j++)
			cells[i][j] = m.cells[i][j];
}

template <typename Cell>
Matrix<Cell>::Matrix(int N, int M)
{
	AllocateCells(N, M);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int N, int M, Cell* list)
{
	AllocateCells(N, M);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			cells[i][j] = list[colmns*i + j];
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=(const Matrix& m)
{
	if (colmns != m.rows)
	{
		FreeCells();
		AllocateCells(m.rows, m.colmns);
	}
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < colmns; j++)
			cells[i][j] = m.cells[i][j];
	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+(const Matrix& m)
{
	

	Matrix<Cell> res(*this);

	if (rows == m.rows)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < m.colmns; j++)
				res.cells[i][j] += m.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-(const Matrix& m)
{
	Matrix<Cell> res(*this);

	if (rows == m.rows)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < m.colmns; j++)
				res.cells[i][j] -= m.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*(const Matrix& m)
{
	Matrix<Cell> res(rows, m.colmns);

	// число столбцов первой матрицы = число строк во воторой матрице
	if (colmns == m.rows)
	{
		for (int i = 0; i < res.rows; i++)
			for (int j = 0; j < res.colmns; j++)
				for (int k = 0; k < m.rows; k++)
					res.cells[i][j] += cells[i][k] * m.cells[k][j];
	}
	return res;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells(int row, int column)
{
	cells = new Cell*[row];
	for (int i = 0; i < row; i++)
		cells[i] = new Cell[column];
	rows = row;
	colmns = column;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for (int i = 0; i < rows; i++)
		delete cells[i];
	delete cells;
	rows = 0;
	colmns = 0;
}

template <typename Cell>
ostream& operator << (ostream & fo, const Matrix<Cell>& matrix)
{
	for (int i = 0; i < matrix.rows; i++)
	{
		fo << "  ";
		for (int j = 0; j < matrix.colmns; j++)
			fo << matrix.cells[i][j] << " \t";
		fo << endl;
	}
	return fo;
}

template <typename Cell>
istream& operator >> (istream& fi, Matrix<Cell>& matrix)
{
	for (int i = 0; i < matrix.rows; i++)
		for (int j = 0; j < matrix.colmns; j++)
			fi >> matrix.cells[i][j];
	return fi;
}

#endif MATRIX_H
