//
// Created by ziv on 10/06/2020.
//

#include "Matrix.h"


#define CONSTRUCTOR_ERR_MSG "Error: invalid rows or columns number"
#define MULT_ERR_MSG "Error: first matrix columns dims and second matrix row dims are not the same"
#define ADD_ERR_MSG "Error: the shape of the matrices should be the same"
#define IDX_ERR_MSG "Error: bad indexes. indexes should be in the rage of the dims of the matrix"

#define DEFAULT_SHAPE 1

using std::cerr;
using std::cout;
using std::endl;


/**
 *
 */
Matrix::Matrix() : shape({DEFAULT_SHAPE, DEFAULT_SHAPE})
{
	values = new float[DEFAULT_SHAPE]();
}

/**
 *
 * @param rows
 * @param cols
 */
Matrix::Matrix(int rows, int cols)
{
	if (rows <= 0 || cols <= 0)
	{
		cerr << CONSTRUCTOR_ERR_MSG << endl;
		exit(EXIT_FAILURE);
	}
	values = new float[rows * cols]();
	shape.rows = rows;
	shape.cols = cols;
}

/**
 *
 * @param matrix
 */
Matrix::Matrix(const Matrix &matrix)
{
	shape.rows = matrix.shape.rows;
	shape.cols = matrix.shape.cols;
	values = new float[shape.rows * shape.cols]();
	for (int i = 0; i < shape.rows * shape.cols; i++)
	{
		values[i] = matrix.values[i];
	}
}

/**
 *
 */
Matrix::~Matrix()
{
	delete[] values;
}

/**
 *
 * @param rhs
 * @return
 */
Matrix &Matrix::operator=(const Matrix &rhs)
{
	if (this != &rhs)
	{
		shape = {rhs.getRows(), rhs.getCols()};
		delete[] values;
		values = new float[shape.rows * shape.cols]();
		for (int i = 0; i < shape.rows * shape.cols; i++)
		{
			values[i] = rhs.values[i];
		}
	}
	return *this;
}

/**
 *
 * @return
 */
int Matrix::getRows() const
{
	return shape.rows;
}

/**
 *
 * @return
 */
int Matrix::getCols() const
{
	return shape.cols;
}

/**
 *
 * @return
 */
Matrix &Matrix::vectorize()
{
	shape = {shape.rows * shape.cols, DEFAULT_SHAPE};
	return *this;
}

/**
 *
 */
void Matrix::plainPrint() const
{
	for (int i = 0; i < shape.rows; i++)
	{
		for (int j = 0; j < shape.cols; j++)
		{
			cout << values[i * shape.cols + j] << " ";
		}
		cout << endl;
	}
}

/**
 *
 * @param rhs
 * @return
 */
Matrix Matrix::operator*(const Matrix &rhs) const
{
	if (this->getCols() == rhs.getRows())
	{
		int rows = this->getRows();
		int cols = rhs.getCols();
		int range = rhs.getRows();

		Matrix newMatrix(rows, cols);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				for (int k = 0; k < range; k++)
				{
					newMatrix(i, j) += (values[i * range + k] * rhs.values[k * cols + j]);
				}
			}
		}
		return newMatrix;
	}
	cerr << MULT_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}

/**
 *
 * @param scalar
 * @return
 */
Matrix Matrix::operator*(const float scalar) const
{
	Matrix newMatrix = Matrix(shape.rows, shape.cols);
	for (int i = 0; i < shape.rows * shape.cols; i++)
	{
		newMatrix[i] = values[i] * scalar;
	}
	return newMatrix;
}

/**
 *
 * @param scalar
 * @param rhs
 * @return
 */
Matrix operator*(const float scalar, const Matrix &rhs)
{
	Matrix newMatrix = Matrix(rhs.shape.rows, rhs.shape.cols);
	for (int i = 0; i < rhs.shape.rows * rhs.shape.cols; i++)
	{
		newMatrix[i] = rhs.values[i] * scalar;
	}
	return newMatrix;
}

/**
 *
 * @param rhs
 * @return
 */
Matrix Matrix::operator+(const Matrix &rhs) const
{
	if (shape.rows == rhs.shape.rows && shape.cols == rhs.shape.cols)
	{
		Matrix newMatrix = Matrix(shape.rows, shape.cols);
		for (int i = 0; i < shape.rows * shape.cols; i++)
		{
			newMatrix[i] = values[i] + rhs.values[i];
		}
		return newMatrix;
	}
	cerr << ADD_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}

/**
 *
 * @param rhs
 * @return
 */
Matrix &Matrix::operator+=(const Matrix &rhs)
{
	if (shape.rows == rhs.shape.rows && shape.cols == rhs.shape.cols)
	{
		for (int i = 0; i < shape.rows * shape.cols; i++)
		{
			values[i] += rhs[i];
		}
		return *this;
	}
	cerr << ADD_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}

/**
 *
 * @param row
 * @param col
 * @return
 */
float Matrix::operator()(const int row, const int col) const
{
	if (0 <= row && row < shape.rows && 0 <= col && col < shape.cols)
	{
		return values[row * shape.cols + col];
	}
	cerr << IDX_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}

/**
 *
 * @param idx
 * @return
 */
float Matrix::operator[](const int idx) const
{
	if (0 <= idx && idx < shape.rows * shape.cols)
	{
		return values[idx];
	}
	cerr << IDX_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}

/**
 *
 * @param row
 * @param col
 * @return
 */
float &Matrix::operator()(const int row, const int col)
{
	if (0 <= row && row < shape.rows && 0 <= col && col < shape.cols)
	{
		return values[row * shape.cols + col];
	}
	cerr << IDX_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}

/**
 *
 * @param idx
 * @return
 */
float &Matrix::operator[](const int idx)
{
	if (0 <= idx && idx < shape.rows * shape.cols)
	{
		return values[idx];
	}
	cerr << IDX_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}

/**
 *
 * @param os
 * @param matrix
 * @return
 */
std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
	for (int i = 0; i < matrix.shape.rows; i++)
	{
		for (int j = 0; j < matrix.shape.cols; j++)
		{
			if (matrix(i, j) <= (float) 0.1)
			{
				os << "  ";
			}
			else
			{
				os << "**";
			}
		}
		os << endl;
	}
	return os;
}

/**
 *
 * @param is
 * @param matrix
 * @return
 */
std::istream &operator>>(std::istream &is, const Matrix &matrix)
{
	for (int i = 0; i < matrix.shape.rows * matrix.shape.cols; i++)
	{
		is.read((char *) &matrix.values[i], sizeof(float));
	}
	return is;
}


