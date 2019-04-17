#include "ActivationFunctions.h"
#include <cfloat>

auto Activation::Identity(float x) -> float 
{
	return x;
}

auto Activation::Identity_Derivative(float x) -> float 
{
	return 1;
}

auto Activation::Fast_Sigmoid(float x) -> float
{
	return (x / (1 + fabs(x)));
}

auto Activation::Fast_Sigmoid_Derivative(float x) -> float
{
	float tmp = 1.f / ((1.f + fabs(x)) * (1.f + fabs(x)));
	// if (!std::isnan(x))
		return tmp;
	// else
	// {
	// 	throw "ERRORRRR";
	// }
}

auto Activation::Sigmoid(float x) -> float
{
	return (float)(1 / 1 + exp(-x));
}

auto Activation::Sigmoid_Derivative(float x) -> float 
{
	float ex = exp(-x);
	return ex / ((1 + ex) * (1 + ex));
}

auto Activation::Relu(float x) -> float
{
	return fmax(0, x);
}

auto Activation::Relu_Derivative(float x) -> float 
{
	return x > 0 ? 1 : 0;
}

auto Activation::Hyperbolic_Tangent(float x) -> float
{
	return tanh(x);
}

auto Activation::Hyperbolic_Tangent_Derivative(float x) -> float 
{
	float th = tanh(x);
	return 1 - th * th;
}