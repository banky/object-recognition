#include <math.h>
#include "matrix.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

Matrix sigmoid(Matrix a);
Matrix sigmoidGradient(Matrix a);
Matrix mLog(Matrix a);
std::vector<Matrix> forwardProp(Matrix x, std::vector<Matrix> theta,
	unsigned numLayers);
std::vector<Matrix> backProp(unsigned numLayers, unsigned m, Matrix x,
	std::vector<Matrix> theta, Matrix y, Matrix tags, float lambda);
float cost(Matrix x, Matrix y, std::vector<Matrix> theta, Matrix tags, 
	float lambda, unsigned m, unsigned numLayers);
std::vector<Matrix> randomInit(unsigned numLayers, std::vector<unsigned> numel);
bool checkConvergence(std::vector<Matrix> a, std::vector<Matrix> b, unsigned numLayers, 
	float limit);
std::vector<Matrix> gradientDescent(std::vector<Matrix> theta, unsigned m, unsigned numLayers, 
	float alpha, Matrix x, Matrix y, Matrix tags, float lambda);