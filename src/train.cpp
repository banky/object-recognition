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

Matrix mLog(Matrix a) {
	for (unsigned i = 0; i < a.numRows; i++) {
		for (unsigned j = 0; j < a.numCols; j++) {
			a.data[i][j] = log(a.data[i][j]);
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
			a = theta[i]*x;
			a = sigmoid(a);
		} else {
			a = a.addOnesRow();
			a = theta[i]*a;
			a = sigmoid(a);
		}
	}

	return a;
}

float backProp(unsigned numLayers) {
	std::vector<Matrix> Del (numLayers);
	
}

float cost(std::vector<Matrix> x, Matrix y, std::vector<Matrix> theta, 
	float lambda, unsigned m, unsigned numOutNodes, unsigned numLayers) {

	float cost = 0;
	Matrix ones(1, numOutNodes, 1);

	// Cost
	for (unsigned i = 0; i < m; i++) {
		for (unsigned k = 0; k < numOutNodes; k++) {
			Matrix input = x[i].transpose();
			
			Matrix h = forwardProp(input, theta, numLayers);
			Matrix cost1 = y.transpose()*mLog(h);
			Matrix cost0 = (ones - y).transpose()*mLog(ones - h);

			float cost = cost1.data[0][0] + cost0.data[0][0];

			cost = cost * (-1/m);
		}
	}

	float reg = 0;

	// Regularization
	for (unsigned layer = 0; layer < numLayers; layer++) {
		for (unsigned i = 0; i < theta[layer].numRows; i++) {
			for (unsigned j = 0; j < theta[layer].numCols; j++) {
				reg += pow(theta[layer].data[i][j], 2);
			}
		}
	}

	reg *= (lambda/(2*m));
	cost += reg;

	return cost;
}