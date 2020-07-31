//
// Created by ziv on 11/06/2020.
//

#include "Activation.h"

using std::exp;


/**
 *
 * @param actType
 */
Activation::Activation(ActivationType actType) : type(actType)
{}

/**
 *
 * @return
 */
ActivationType Activation::getActivationType() const
{
	return type;
}

/**
 *
 * @param vec
 * @return
 */
Matrix Activation::operator()(const Matrix &vec) const
{
	int len = vec.getRows() * vec.getCols();
	Matrix r = Matrix(vec.getRows(), vec.getCols());
	// Relu
	if (type == Relu)
	{
		for (int i = 0; i < len; i++)
		{
			if (0 < vec[i])
			{
				r[i] = vec[i];
			}
		}
	}
		// Softmax
	else
	{
		float s = 0;
		for (int i = 0; i < len; i++)
		{
			r[i] = exp(vec[i]);
			s += exp(vec[i]);
		}
		r = r * (1 / s);
	}
	return r;
}
