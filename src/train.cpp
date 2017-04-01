#include "train.h"

/*
 * Computes the sigmoid for a given matrix
 */
Matrix sigmoid(Matrix a) {
	for (unsigned i = 0; i < a.numRows; i++) {
		for (unsigned j = 0; j < a.numCols; j++) {
			a.data[i][j] = 1/(1 + exp(a.data[i][j]));
		}
	}
	return a;
}

Matrix forwardProp(Matrix x, std::vector<Matrix> theta,
			       unsigned numLayers) {
	Matrix a = Matrix();

	// numLayers does not include input layer
	for (unsigned i = 0; i < numLayers; i++) {
		if (i == 0) {
			x = x.addOnesRow();
			a = theta[i].multiply(x);
			a = a.sigmoid();
		} else {
			a = a.addOnesRow();
			a = theta[i].multiply(a);
			a = a.sigmoid();
		}
	}

	return a;
}

float cost(Matrix x, Matrix y, std::vector<Matrix> theta, float lambda
	       unsigned m) {
	
}