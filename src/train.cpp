#include "train.h"
#include <Eigen/Dense>
#include <Eigen/Core>

#define CONVERGENCE_LIMIT 0.1

using namespace std;
using namespace Eigen;

double sigmoid(double x) {
	return 1/(1 + exp(-x));
}

/*
 * Computes the sigmoid for a given matrix
 */
MatrixXd sigmoid(MatrixXd a) {
	for (unsigned i = 0; i < a.numRows; i++) {
		for (unsigned j = 0; j < a.numCols; j++) {
			a(i, j) = 1/(1 + exp(-a(i, j));
		}
	}
	return a;
}

MatrixXd sigmoidGradient(MatrixXd a) {
	// Matrix ones(1, a.numRows, a.numCols);
	// Matrix rhs = ones - sigmoid(a);
	// Matrix lhs = sigmoid(a);
	// a = lhs.elementMultiply(rhs);

	a = (1 - sigmoid(a).array()) * sigmoid(a).array();

	return a;
}

MatrixXd mLog(MatrixXd a) {
	for (unsigned i = 0; i < a.numRows; i++) {
		for (unsigned j = 0; j < a.numCols; j++) {
			a.data[i][j] = log(a.data[i][j]);
		}
	}
	return a;
}

std::vector<Matrix> forwardProp(Matrix x, std::vector<Matrix> theta,
	unsigned numLayers) {

	// Add one to include the input layer. Can optimize by removing this layer?
	std::vector<Matrix> a (numLayers + 1);
	Matrix tmp = Matrix();

	a[0] = x;
	
	// numLayers does not include input layer
	for (unsigned i = 0; i < numLayers; i++) {
		tmp = a[i].addOnesRow();
		tmp = theta[i]*tmp;
		tmp = sigmoid(tmp);
		a[i + 1] = tmp;
	}

	return a;
}

std::vector<Matrix> backProp(unsigned numLayers, unsigned m, Matrix x,
	std::vector<Matrix> theta, Matrix y, Matrix tags, double lambda) {

	std::vector<Matrix> Del (numLayers);
	std::vector<Matrix> a (numLayers + 1);	// Add 1 to include training example layer
	std::vector<Matrix> thetaGrad (numLayers);
	unsigned numOutNodes = tags.numRows;

	// Initialize Del to zeroes
	for (unsigned i = 0; i < numLayers; i++) {
		Del[i] = Matrix(0, theta[i].numRows, theta[i].numCols);
	}

	for (unsigned i = 0; i < m; i++) {
		Matrix input = x.getRow(i).transpose();

		a = forwardProp(input, theta, numLayers);

		Matrix currY = Matrix(y.data[i][0], numOutNodes, 1);
		currY = currY.compare(tags);

		std::vector<Matrix> diff(numLayers);

		diff[numLayers - 1] = a[numLayers] - currY;
		for (int layer = numLayers - 2; layer >= 0; layer--) {
			Matrix z = theta[layer]*(a[layer].addOnesRow());
			diff[layer] = theta[layer+1].transpose()*diff[layer+1];
			diff[layer] = diff[layer].elementMultiply(sigmoidGradient(z).addOnesRow());
			diff[layer].removeRow(0);
		}
		
		for (unsigned layer = 0; layer < numLayers; layer++) {
			Del[layer] += (diff[layer])*(a[layer].addOnesRow().transpose());
		}
	}

	for (unsigned layer = 0; layer < numLayers; layer++) {
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

	// thetaGrad[0].print();
	// thetaGrad[1].print();

	return thetaGrad;
}

std::vector<Matrix> gradientCheck(std::vector<Matrix> theta, double e, unsigned numLayers,
	Matrix x, Matrix y, Matrix tags, double lambda, unsigned m) {
	// Initialize thetaGrad vector
	std::vector<Matrix> thetaGrad (numLayers);
	for (unsigned i = 0; i < numLayers; i++) {
		thetaGrad[i] = Matrix(0, theta[i].numRows, theta[i].numCols);
	}

	for (unsigned layer = 0; layer < numLayers; layer++) {
		for (unsigned i = 0; i < theta[layer].numRows; i++) {
			for (unsigned j = 0; j < theta[layer].numCols; j++) {
				std::vector<Matrix> rTheta = theta;
				std::vector<Matrix> lTheta = theta;

				rTheta[layer].data[i][j] += e;
				lTheta[layer].data[i][j] -= e;

				double rCost = cost(x, y, rTheta, tags, lambda, 
					m, numLayers);
				double lCost = cost(x, y, lTheta, tags, lambda,
					m, numLayers);

				thetaGrad[layer].data[i][j] = (rCost - lCost)/(2 * e);
			}
		}
	}

	// thetaGrad[0].print();
	// thetaGrad[1].print();
	
	return thetaGrad;
}

double cost(Matrix x, Matrix y, std::vector<Matrix> theta, Matrix tags, 
	double lambda, unsigned m, unsigned numLayers) {

	double cost = 0;
	unsigned numOutNodes = tags.numRows;
	Matrix ones(1, numOutNodes, 1);

	// Cost
	for (unsigned i = 0; i < m; i++) {
		Matrix input = x.getRow(i).transpose();

		std::vector<Matrix> a = forwardProp(input, theta, numLayers);
		Matrix h = a[numLayers];

		Matrix currY = Matrix(y.data[i][0], numOutNodes, 1);
		currY = currY.compare(tags);

		//currY is a matrix of 1's and 0's
		Matrix cost1 = currY.transpose()*mLog(h);
		Matrix cost0 = (ones - currY).transpose()*mLog(ones - h);

		cost += cost1.data[0][0] + cost0.data[0][0];
	}

	cost *= (-1/(double)m);

	double reg = 0;

	// Regularization
	for (unsigned layer = 0; layer < numLayers; layer++) {
		for (unsigned i = 0; i < theta[layer].numRows; i++) {
			for (unsigned j = 0; j < theta[layer].numCols; j++) {
				reg += pow(theta[layer].data[i][j], 2);
			}
		}
	}

	reg *= (double)(lambda/(2*(double)m));
	cost += reg;

	return cost;
}

std::vector<Matrix> randomInit(unsigned numLayers, std::vector<unsigned> numel) {
	std::vector<Matrix> theta (numLayers);
	srand(time(NULL));
	unsigned e = 1;
	// All theta values are between e and -e

	// numel has one element per group of nodes
	if (numLayers != numel.size() - 1) {
		std::cout << "Incorrect number of elements for number" 
				  <<" of layers on random initializtion. \n";
		exit(1);
	}

	for (unsigned layer = 0; layer < numLayers; layer++) {
		unsigned numRows = numel[layer + 1];
		unsigned numCols = numel[layer] + 1;
		theta[layer] = Matrix(0, numRows, numCols);
		for (unsigned i = 0; i < numRows; i++) {
			for (unsigned j = 0; j < numCols; j++) {
				theta[layer].data[i][j] = (2 * e * (double)rand())/RAND_MAX - e;
				//theta[layer].data[i][j] = (float)(i + 1)/float(numRows);
			}
		}
	}

 	return theta;
}

bool checkConvergence(std::vector<Matrix> a, std::vector<Matrix> b, unsigned numLayers, 
	double limit) {
	for (unsigned layer = 0; layer < numLayers; layer++) {
		if (fabs(a[layer].sum() - b[layer].sum()) >= limit) {
			return false;
		}
	}
	return true;
}

std::vector<Matrix> gradientDescent(std::vector<Matrix> theta, unsigned m, unsigned numLayers, 
	double alpha, Matrix x, Matrix y, Matrix tags, double lambda) {

	std::vector<Matrix> pastTheta = theta;
	unsigned i = 0;

	do {
		cout << "Starting back prop \n";
		std::vector<Matrix> grad = backProp(numLayers, m, x, theta, y, tags, lambda);
		cout << "Done back prop\n";
		// std::vector<Matrix> grad2 = gradientCheck(theta, 0.00001, numLayers,
		// 	x, y, tags, lambda, m);
		pastTheta = theta;

		cout << "Computing new theta\n";
		for (unsigned layer = 0; layer < numLayers; layer++) {
			double coeff = alpha/m;
			Matrix coeffMat = Matrix(coeff, theta[layer].numRows, theta[layer].numCols);
			theta[layer] = theta[layer] - coeffMat.elementMultiply(grad[layer]);
		}
		cout << "Computing cost\n";
		double currentCost = cost(x, y, theta, tags, lambda, m, numLayers);
		std::cout << "Current cost: " << currentCost << std::endl;
		i++;
		cout << "Done\n";
	//} while (checkConvergence(pastTheta, theta, numLayers, CONVERGENCE_LIMIT));
	} while(i < 2);
	return theta;
}
