#include <math.h>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
//#include "matrix.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

double sigmoid(double x);
double sigmoidGradient(double x);
double mLog(double x);
void removeRow(Eigen::MatrixXd& matrix, unsigned rowToRemove);
Eigen::MatrixXd appendOnes(Eigen::MatrixXd a);
std::vector<Eigen::MatrixXd> forwardProp(Eigen::MatrixXd x, std::vector<Eigen::MatrixXd> theta,
	unsigned numLayers);
std::vector<Eigen::MatrixXd> backProp(unsigned numLayers, unsigned m, Eigen::MatrixXd x,
	std::vector<Eigen::MatrixXd> theta, Eigen::MatrixXd y, Eigen::MatrixXd tags, double lambda);
std::vector<Eigen::MatrixXd> gradientCheck(std::vector<Eigen::MatrixXd> theta, double e, unsigned numLayers,
	Eigen::MatrixXd x, Eigen::MatrixXd y, Eigen::MatrixXd tags, double lambda, unsigned m);
double cost(Eigen::MatrixXd x, Eigen::MatrixXd y, std::vector<Eigen::MatrixXd> theta, Eigen::MatrixXd tags, 
	double lambda, unsigned m, unsigned numLayers);
std::vector<Eigen::MatrixXd> randomInit(unsigned numLayers, std::vector<unsigned> numel);
std::vector<Eigen::MatrixXd> gradientDescent(std::vector<Eigen::MatrixXd> theta, unsigned m, unsigned numLayers, 
	double alpha, Eigen::MatrixXd x, Eigen::MatrixXd y, Eigen::MatrixXd tags, double lambda);