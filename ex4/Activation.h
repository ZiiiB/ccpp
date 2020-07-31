//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
#include <cmath>

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
	Relu,
	Softmax
};


/**
 * the activation class represent the
 * activation function and its
 * utilities and abilities
 */
class Activation
{
private:
	ActivationType type;
public:
	/**
	 * regular constructor
	 * @param type the type of activation function to active
	 */
	Activation(ActivationType type);

	/**
	 * get the type of function will be activated
	 * @return activation type
	 */
	ActivationType getActivationType() const;

	/**
	 * use the activation function on the vector vec
	 * @param vec the vector to active the function on
	 * @return new vector r which is the value
	 * of the activation on vec
	 */
	Matrix operator()(const Matrix &vec) const;
};

#endif //ACTIVATION_H
