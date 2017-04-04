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

Matrix sigmoidGradient(Matrix a) {
	Matrix ones(1, a.numRows, a.numCols);
	Matrix rhs = ones - sigmoid(a);
	Matrix lhs = sigmoid(a);

	for (unsigned i = 0; i < a.numRows; i++) {
		for (unsigned j = 0; j < a.numCols; j++) {
			a.data[i][j] = lhs.data[i][j] * rhs.data[i][j];
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

std::vector<Matrix> forwardProp(Matrix x, std::vector<Matrix> theta,
	unsigned numLayers) {

	std::vector<Matrix> a (numLayers);
	Matrix tmp = Matrix();
	
	// numLayers does not include input layer
	for (unsigned i = 0; i < numLayers; i++) {
		if (i == 0) {
			tmp = x.addOnesRow();
			tmp = theta[i]*tmp;
			tmp = sigmoid(tmp);
			a[i] = x;
		} else {
			tmp = tmp.addOnesRow();
			tmp = theta[i]*tmp;
			tmp = sigmoid(tmp);
			tmp.removeRow(0);
			a[i] = tmp;
		}
	}

	return a;
}

std::vector<Matrix> backProp(unsigned numLayers, unsigned m, Matrix x,
	std::vector<Matrix> theta, Matrix y, unsigned numOutNodes, Matrix tags, float lambda) {

	std::vector<Matrix> Del (numLayers - 1);
	std::vector<Matrix> a (numLayers);
	std::vector<Matrix> thetaGrad (numLayers - 1);

	for (unsigned i = 0; i < m; i++) {
		Matrix input = x.getRow(i).transpose();
		a = forwardProp(input, theta, numLayers);
		
		Matrix currY = Matrix(y.data[i][0], numOutNodes, 1);
		currY = currY.compare(tags);

		std::vector<Matrix> diff(numLayers);
		diff[numLayers - 1] = a[numLayers - 1] - currY;
		for (unsigned layer = numLayers - 2; layer >= 1; layer--) {
			diff[layer] = theta[layer].transpose()*diff[layer+1];
			diff[layer] = diff[layer].elementMultiply(sigmoidGradient(a[layer]));
		}

		for (unsigned layer = 0; layer < numLayers - 1; layer++) {
			Del[layer] += a[layer]*(diff[layer + 1].transpose());
		}
	}

	for (unsigned layer = 0; layer < numLayers - 1; layer++) {
		unsigned thetaNumRows = theta[layer].numRows;
		unsigned thetaNumCols = theta[layer].numCols;
		thetaGrad[layer] = Matrix(0, thetaNumRows, thetaNumCols);

 		for (unsigned i = 0; i < thetaNumRows; i++) {
			for (unsigned j = 0; j < thetaNumCols; j++) {
				if (j == 0) {
					thetaGrad[layer].data[i][j] = (Del[layer].data[i][j])/m;
				} else {
					thetaGrad[layer].data[i][j] = (Del[layer].data[i][j])/m;
					thetaGrad[layer].data[i][j] += lambda * theta[layer].data[i][j];
				}
			}
		}
	}

	return thetaGrad;
}

float cost(Matrix x, Matrix y, std::vector<Matrix> theta, Matrix tags, 
	float lambda, unsigned m, unsigned numOutNodes, unsigned numLayers) {

	float cost = 0;
	Matrix ones(1, numOutNodes, 1);

	// Cost
	for (unsigned i = 0; i < m; i++) {
		Matrix input = x.getRow(i).transpose();
		
		std::vector<Matrix> a = forwardProp(input, theta, numLayers);
		Matrix h = a[numLayers - 1];
		Matrix currY = Matrix(y.data[i][0], numOutNodes, 1);
		currY = currY.compare(tags);
		
		//currY is a matrix of 1's and 0's
		Matrix cost1 = currY.transpose()*mLog(h);
		Matrix cost0 = (ones - currY).transpose()*mLog(ones - h);

		float cost = cost1.data[0][0] + cost0.data[0][0];

		cost = cost * (-1/m);
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

std::vector<Matrix> randomInit(unsigned numLayers, std::vector<unsigned> numel) {
	std::vector<Matrix> theta (numLayers);
	srand(time(NULL));
	unsigned e = 1;
	// All theta values are between e and -e

	for (unsigned layer = 0; layer < numLayers; layer++) {
		unsigned numRows = numel[layer + 1];
		unsigned numCols = numel[layer] + 1;
		theta[layer] = Matrix(0, numRows, numCols);
		for (unsigned i = 0; i < numRows; i++) {
			for (unsigned j = 0; j < numCols; j++) {
				theta[layer].data[i][j] = (2 * e * rand())/RAND_MAX - e;
			}
		}
	}
 
 	return theta;
}

