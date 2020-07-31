//
// Created by ziv on 11/06/2020.
//

#include "Dense.h"

/**
 *
 * @param weights
 * @param biases
 * @param type
 */
Dense::Dense(Matrix &weights, Matrix &biases, ActivationType type) : Wights(weights), bias(biases),
																	 activation(type)
{}

/**
 *
 * @return
 */
const Matrix &Dense::getWeights() const
{
	return Wights;
}

/**
 *
 * @return
 */
const Matrix &Dense::getBias() const
{
	return bias;
}

/**
 *
 * @return
 */
const Activation Dense::getActivation() const
{
	return activation;
}

/**
 *
 * @param vec
 * @return
 */
Matrix Dense::operator()(const Matrix &vec) const
{
	return activation((Wights * vec) + bias);
}

