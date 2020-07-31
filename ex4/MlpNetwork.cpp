//
// Created by ziv on 11/06/2020.
//

#include "MlpNetwork.h"
#include <iostream>

using std::cerr;
using std::endl;

#define BAD_SIZE_ERR_MSG "Error: bad size of input to the MlpNetwork NN"

/**
 *
 * @param vec
 * @return
 */
Digit MlpNetwork::_probabilitiesToDigit(const Matrix &vec) const
{
	Digit best = {(unsigned int) 0, (float) 0};
	for (int i = 0; i < vec.getRows() * vec.getCols(); i++)
	{
		if (vec[i] > best.probability)
		{
			best = {(unsigned int) i, (float) vec[i]};
		}
	}
	return best;
}

/**
 *
 * @param weights
 * @param biases
 */
MlpNetwork::MlpNetwork(Matrix weights[], Matrix biases[]) :
		_layers
		{
				Dense(weights[0], biases[0], Relu),
				Dense(weights[1], biases[1], Relu),
				Dense(weights[2], biases[2], Relu),
				Dense(weights[3], biases[3], Softmax)
		}
{}

/**
 *
 * @param vec
 * @return
 */
Digit MlpNetwork::operator()(Matrix &vec) const
{
	if (vec.getRows() * vec.getCols() == imgDims.rows * imgDims.cols)
	{
		Matrix vec2 = Matrix(vec);
		for (int i = 0; i < MLP_SIZE; i++)
		{
			vec2 = _layers[i](vec2);
		}
		return _probabilitiesToDigit(vec2);
	}
	cerr << BAD_SIZE_ERR_MSG << endl;
	exit(EXIT_FAILURE);
}



