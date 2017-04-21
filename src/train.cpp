#include "train.h"

#define CONVERGENCE_LIMIT 0.1

using namespace Eigen;
using namespace std;

double sigmoid(double x) {
	return 1/(1 + exp(-x));
}

double sigmoidGradient(double x) {
	return (1 - sigmoid(x)) * sigmoid(x);
}

double mLog(double x) {
	return log(x);
}

void removeRow(MatrixXd& matrix, unsigned rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if(rowToRemove < numRows) {
    	matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = 
    	matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);
    }
    matrix.conservativeResize(numRows,numCols);
}

/*
 *	Append a row of ones the top of a matrix
 */
MatrixXd appendOnes(MatrixXd a) {
	MatrixXd tmp = MatrixXd(1 + a.rows(), a.cols());
	tmp.block(1, 0, a.rows(), a.cols()) = a;
	MatrixXd ones = MatrixXd::Ones(1, a.cols());
	tmp << ones, 
		  a;
	return tmp;
}

std::vector<MatrixXd> forwardProp(MatrixXd x, std::vector<MatrixXd> theta,
	unsigned numLayers) {

	// Add one to include the input layer. Can optimize by removing this layer?
	std::vector<MatrixXd> a (numLayers + 1);

	a[0] = x;
	
	// numLayers does not include input layer
	for (unsigned i = 0; i < numLayers; i++) {
		MatrixXd tmp = appendOnes(a[i]);
		tmp = theta[i]*tmp;
		tmp = tmp.unaryExpr(&sigmoid);
		a[i + 1] = tmp;
	}

	return a;
}

std::vector<MatrixXd> backProp(unsigned numLayers, unsigned m, MatrixXd x,
	std::vector<MatrixXd> theta, MatrixXd y, MatrixXd tags, double lambda) {

	std::vector<MatrixXd> Del (numLayers);
	std::vector<MatrixXd> a (numLayers + 1);	// Add 1 to include training example layer
	std::vector<MatrixXd> thetaGrad (numLayers);

	// Initialize Del to zeroes
	for (unsigned i = 0; i < numLayers; i++) {
		Del[i] = MatrixXd::Zero(theta[i].rows(), theta[i].cols());
	}

	for (unsigned i = 0; i < m; i++) {
		// Get the first row of x and transpose
		MatrixXd input = x.block(i, 0, 1, x.cols()).transpose();

		a = forwardProp(input, theta, numLayers);

		Matrix<bool, Dynamic, Dynamic> currY = y(i, 0) == tags.array();

		std::vector<MatrixXd> diff(numLayers);

		diff[numLayers - 1] = a[numLayers] - currY.cast <double> ();
		for (int layer = numLayers - 2; layer >= 0; layer--) {
			MatrixXd aTmp = appendOnes(a[layer]);
			MatrixXd z = theta[layer]*aTmp;
			diff[layer] = theta[layer+1].transpose()*diff[layer+1];

			MatrixXd zTmp = appendOnes(z.unaryExpr(&sigmoidGradient));
			diff[layer] = diff[layer].array() * zTmp.array();
			removeRow(diff[layer], 0);
		}
		
		for (unsigned layer = 0; layer < numLayers; layer++) {
			MatrixXd tmp = appendOnes(a[layer]);
			tmp.transposeInPlace();
			Del[layer] += diff[layer] * tmp;
		}
		cout << i << endl;
	}

	for (unsigned layer = 0; layer < numLayers; layer++) {
		unsigned thetaNumRows = theta[layer].rows();
		unsigned thetaNumCols = theta[layer].cols();
		thetaGrad[layer] = MatrixXd::Zero(thetaNumRows, thetaNumCols);

 		for (unsigned i = 0; i < thetaNumRows; i++) {
			for (unsigned j = 0; j < thetaNumCols; j++) {
				if (j == 0) {
					thetaGrad[layer](i, j) = (Del[layer](i, j))/m;
				} else {
					thetaGrad[layer](i, j) = (Del[layer](i, j))/m;
					thetaGrad[layer](i, j) += lambda * theta[layer](i, j);
				}
			}
		}
	}

	// cout << thetaGrad[0] << endl;
	// cout << thetaGrad[1] << endl;

	return thetaGrad;
}

std::vector<MatrixXd> gradientCheck(std::vector<MatrixXd> theta, double e, unsigned numLayers,
	MatrixXd x, MatrixXd y, MatrixXd tags, double lambda, unsigned m) {
	// Initialize thetaGrad vector
	std::vector<MatrixXd> thetaGrad (numLayers);
	for (unsigned i = 0; i < numLayers; i++) {
		thetaGrad[i] = MatrixXd::Zero(theta[i].rows(), theta[i].cols());
	}

	for (unsigned layer = 0; layer < numLayers; layer++) {
		for (unsigned i = 0; i < theta[layer].rows(); i++) {
			for (unsigned j = 0; j < theta[layer].cols(); j++) {
				std::vector<MatrixXd> rTheta = theta;
				std::vector<MatrixXd> lTheta = theta;

				rTheta[layer](i, j) += e;
				lTheta[layer](i, j) -= e;

				double rCost = cost(x, y, rTheta, tags, lambda, 
					m, numLayers);
				double lCost = cost(x, y, lTheta, tags, lambda,
					m, numLayers);

				thetaGrad[layer](i, j) = (rCost - lCost)/(2 * e);
			}
		}
	}

	// cout << thetaGrad[0] << endl;
	// cout << thetaGrad[1] << endl;
	
	return thetaGrad;
}

double cost(MatrixXd x, MatrixXd y, std::vector<MatrixXd> theta, MatrixXd tags, 
	double lambda, unsigned m, unsigned numLayers) {

	double cost = 0;

	// Cost
	for (unsigned i = 0; i < m; i++) {
		MatrixXd input = x.row(i).transpose();

		std::vector<MatrixXd> a = forwardProp(input, theta, numLayers);
		MatrixXd h = a[numLayers];

		Matrix<bool, Dynamic, Dynamic> currY = tags.array() == y(i, 0);

		//currY is a matrix of 1's and 0's
		currY.transposeInPlace();
		MatrixXd cost1 = (currY.cast <double> ())*h.unaryExpr(&mLog);
		MatrixXd cost0 = (1 - (currY.cast <double> ()).array()).matrix()*
						(1 - h.array()).matrix().unaryExpr(&mLog);

		cost += cost1(0, 0) + cost0(0, 0);
	}

	cost *= (-1/(double)m);

	double reg = 0;

	// Regularization
	for (unsigned layer = 0; layer < numLayers; layer++) {
		for (unsigned i = 0; i < theta[layer].rows(); i++) {
			for (unsigned j = 0; j < theta[layer].cols(); j++) {
				reg += pow(theta[layer](i, j), 2);
			}
		}
	}

	reg *= (double)(lambda/(2*(double)m));
	cost += reg;

	return cost;
}

std::vector<MatrixXd> randomInit(unsigned numLayers, std::vector<unsigned> numel) {
	std::vector<MatrixXd> theta (numLayers);
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
		theta[layer] = MatrixXd::Zero(numRows, numCols);
		for (unsigned i = 0; i < numRows; i++) {
			for (unsigned j = 0; j < numCols; j++) {
				theta[layer](i, j) = (2 * e * (double)rand())/RAND_MAX - e;
				//theta[layer].data[i][j] = (float)(i + 1)/float(numRows);
			}
		}
	}

 	return theta;
}

/*bool checkConvergence(std::vector<MatrixXd> a, std::vector<MatrixXd> b, unsigned numLayers, 
	double limit) {
	for (unsigned layer = 0; layer < numLayers; layer++) {
		if (fabs(a[layer].sum() - b[layer].sum()) >= limit) {
			return false;
		}
	}
	return true;
}*/

std::vector<MatrixXd> gradientDescent(std::vector<MatrixXd> theta, unsigned m, unsigned numLayers, 
	double alpha, MatrixXd x, MatrixXd y, MatrixXd tags, double lambda) {

	std::vector<MatrixXd> pastTheta = theta;
	unsigned i = 0;

	do {
		std::vector<MatrixXd> grad = backProp(numLayers, m, x, theta, y, tags, lambda);
		cout << "1\n";
		// std::vector<MatrixXd> grad2 = gradientCheck(theta, 0.00001, numLayers,
		// 	x, y, tags, lambda, m);
		pastTheta = theta;

		for (unsigned layer = 0; layer < numLayers; layer++) {
			double coeff = alpha/m;
			theta[layer] = theta[layer].array() - coeff*grad[layer].array();
		}
		double currentCost = cost(x, y, theta, tags, lambda, m, numLayers);
		std::cout << "Current cost: " << currentCost << std::endl;
		i++;
	//} while (checkConvergence(pastTheta, theta, numLayers, CONVERGENCE_LIMIT));
	} while(i < 2);
	return theta;
}
