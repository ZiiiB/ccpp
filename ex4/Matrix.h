// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstdlib>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
	int rows, cols;
} MatrixDims;


/**
 * class which represent the
 * mathematical object matrix
 * and some of its utilities
 */
class Matrix
{
private:
	MatrixDims shape;
	float *values;
public:
	/**
	 * regular constructor.
	 * create a Matrix instance initialized with zeros
	 * @param rows row dim
	 * @param cols col dim
	 */
	Matrix(int rows, int cols);

	/**
	 * default constructor. create a 1x1 matrix
	 * using the regular constructor
	 */
	Matrix();

	/**
	 * from Matrix constructor
	 * @param matrix a matrix object to copy (deep copy)
	 */
	Matrix(const Matrix &matrix);

	/**
	 * de-constructor of a Matrix instance
	 */
	~Matrix();

	/**
	 * getter for rows dim
	 * @return rows dim
	 */
	int getRows() const;

	/**
	 * getter for cols dim
	 * @return cols dim
	 */
	int getCols() const;

	/**
	 * matrix vectorized representation
	 * (as reshape to nx1 shape)
	 * @return vectorized matrix
	 */
	Matrix &vectorize();

	/**
	 * print the matrix in shape
	 */
	void plainPrint() const;

	/**
	 * = operator. assigns one matrix to another
	 * @param rhs the matrix to assign to the new matrix
	 * @return Matrix assigned to
	 */
	Matrix &operator=(const Matrix &rhs);

	/**
	 * matrix multiplication
	 * @param rhs the right hand matrix
	 * @return Matrix which is the value
	 * of the multiplication
	 */
	Matrix operator*(const Matrix &rhs) const;

	/**
	 * right hand by scalar multiplication
	 * @param scalar the scalar to multiply with
	 * @return new matrix after the multiplication
	 */
	Matrix operator*(const float scalar) const;

	/**
	 * left hand by scalar multiplication
	 * @param scalar the scalar to multiply with
	 * @param rhs the matrix to multiply with
	 * @return the value of the multiplication
	 */
	friend Matrix operator*(const float scalar, const Matrix &rhs);

	/**
	 * matrix addition
	 * @param rhs
	 * @return new matrix which is
	 * the sum of the two matrices
	 */
	Matrix operator+(const Matrix &rhs) const;

	/**
	 * matrix addition
	 * @param rhs matrix to add to this
	 * @return this after addition
	 */
	Matrix &operator+=(const Matrix &rhs);

	/**
	 * value in the matrix
	 * @param row row idx
	 * @param col col idx
	 * @return the value of a[row*cols+col]
	 */
	float operator()(const int row, const int col) const;

	/**
	 * value in the matrix
	 * @param idx the index in the a_rowcol element
	 * when row*cols + col = idx
	 * @return the value of values[idx]
	 */
	float operator[](const int idx) const;

	/**
	 * value in the matrix
	 * @param row row idx
	 * @param col col idx
	 * @return the value of a[row*cols+col] by ref
	 */
	float &operator()(const int row, const int col);

	/**
	 * value in the matrix
	 * @param idx the index in the a_rowcol element
	 * when row*cols + col = idx
	 * @return the value of values[idx] by ref
	 */
	float &operator[](const int idx);

	/**
	 * write values to the matrix threw a stream
	 * @param os the out-stream to write to
	 * @param matrix the matrix to stream to the stream
	 * @return the stream
	 */
	friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

	/**
	 * read values from a stream threw matrix
	 * @param is the in-stream to read from
	 * @param matrix the matrix to stream in from the stream
	 * @return the stream
	 */
	friend std::istream &operator>>(std::istream &is, const Matrix &matrix);
};


#endif //MATRIX_H
