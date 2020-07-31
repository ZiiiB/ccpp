//
// Created by ziv on 11/06/2020.
//

#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * the Dense class represent the whole Dense layer
 */
class Dense
{
private:
	Matrix Wights;
	Matrix bias;
	Activation activation;
public:
	/**
	 * regular constructor
	 * @param weights the wight matrix in the Dense layer
	 * @param bias the bias vector in the Dense layer
	 * @param type the activation type
	 * to define which function to activate on the out put of the
	 * linear part
	 */
	Dense(Matrix &weights, Matrix &biases, ActivationType type);

	/**
	 * get the values of the weights matrix
	 * @return the value of the weights matrix
	 */
	const Matrix &getWeights() const;

	/**
	 * get the values of the bias matrix
	 * @return the value of the bias matrix
	 */
	const Matrix &getBias() const;

	/**
	 * get the activation type of the layer
	 * @return the activation type of the layer
	 */
	const Activation getActivation() const;

	/**
	 * activate the layer on an input
	 * @param vec the matrix of the vector
	 * which is the input to the layer
	 * @return the output vector of the layer
	 */
	Matrix operator()(const Matrix &vec) const;
};


#endif //DENSE_H
