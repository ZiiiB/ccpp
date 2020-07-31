//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Activation.h"
#include "Matrix.h"
#include "Dense.h"
#include "Digit.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
								  {64,  128},
								  {20,  64},
								  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
							   {64,  1},
							   {20,  1},
							   {10,  1}};

/**
 * class to hold and run the whole NN
 */
class MlpNetwork
{
private:
	Dense _layers[MLP_SIZE];

	/**
	 * return the digit object from the probabilities vector
	 * @param vec the probabilities vector
	 * @return Digit struct which contains the value
	 * and the probability of the digit written in
	 * the image
	 */
	Digit _probabilitiesToDigit(const Matrix &vec) const;

public:
	/**
	 * constructor
	 * constructs the whole NN
	 * structure from Matrices and so
	 */
	MlpNetwork(Matrix weights[], Matrix biases[]);

	/**
	 * apply the NN on a vector
	 * @param vec the vector which the NN will be applied on
	 * @return the digit
	 */
	Digit operator()(Matrix &vec) const;
};

#endif // MLPNETWORK_H
