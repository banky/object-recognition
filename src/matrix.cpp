#include "matrix.h"

using namespace std;

Matrix::Matrix() {
	this->numRows = 0;
	this->numCols = 0;

	this->data = std::vector< std::vector<float> > (0, 
				std::vector<float>(0, 0));
}

Matrix::Matrix(unsigned rowIdx, unsigned colIdx, unsigned numRows_, 
	unsigned numCols_) {

	this->numRows = numRows_;
	this->numCols = numCols_;
	this->data = 
		vector< vector<float> > (numRows_, vector<float>(numCols_, 0));

	this->data[rowIdx][colIdx] = 1;
}


Matrix::Matrix(float value, unsigned numRows_, unsigned numCols_) {
	this->numRows = numRows_;
	this->numCols = numCols_;

	this->data = 
		vector< vector<float> > (numRows_, vector<float>(numCols_));

	for (unsigned i = 0; i < numRows_; i++) {
		for (unsigned j = 0; j < numCols_; j++) {
			this->data[i][j] = value;
		}
	}
}

Matrix::Matrix(tbt values, unsigned numRows_, unsigned numCols_) {
	this->numRows = numRows_;
	this->numCols = numCols_;

	this->data = 
		vector< vector<float> > (numRows_, vector<float>(numCols_));

	for (unsigned i = 0; i < numRows_; i++) {
		for (unsigned j = 0; j < numCols_; j++) {
			this->data[i][j] = values[i][j];
		}
	}
}

Matrix Matrix::multiply(Matrix b) {
	Matrix res(0, this->numRows, b.numCols);

	// Matrices cannot be multiplied
	if (this->numCols == b.numRows) {
		for (unsigned i = 0; i < this->numRows; i++) {
			for (unsigned j = 0; j < b.numCols; j++) {
				for (unsigned k = 0; k < this->numCols; k++) {
					res.data[i][j] = this->data[i][k]*b.data[k][j];
				}
			}
		}
	} else {
		cout << "Matrix multiplication failed. Size mismatch\n";
		exit(1);
	}

	return res;
}

Matrix Matrix::elementMultiply(Matrix b) {
	Matrix res(0, this->numRows, this->numCols);

	if (this->numRows == b.numRows && this->numCols == b.numCols) {
		for (unsigned i = 0; i < this->numRows; i++) {
			for (unsigned j = 0; j < this->numCols; j++) {
				res.data[i][j] = this->data[i][j] * b.data[i][j];
			}
		}
	} else {
		cout << "Element wise multiplication failed. Size mismatch\n";
	}

	return res;
}

Matrix Matrix::transpose() {
	Matrix res(0, this->numCols, this->numRows);

	for (unsigned i = 0; i < this->numRows; i++) {
		for (unsigned j = 0; j < this->numCols; j++) {
			res.data[j][i] = this->data[i][j];
		}
	}

	return res;
}

Matrix Matrix::addOnesRow() {
	Matrix res(0, this->numRows + 1, this->numCols);

	for (unsigned i = 0; i <= this->numRows; i++) {
		for (unsigned j = 0; j < this->numCols; j++) {
			if (i == 0) {
				res.data[i][j] = 1;
			} else {
				res.data[i][j] = this->data[i - 1][j];
			}
		}
	}

	return res;
}

Matrix Matrix::addOnesCol() {
	Matrix res(0, this->numRows, this->numCols + 1);

	for (unsigned i = 0; i < this->numRows; i++) {
		for (unsigned j = 0; j <= this->numCols; j++) {
			if (j == 0) {
				res.data[i][j] = 1;
			} else {
				res.data[i][j] = this->data[i][j - 1];
			}
		}
	}

	return res;
}

void Matrix::removeRow(unsigned row) {
	this->data.erase(this->data.begin() + row);
	this->numRows--;
}

void Matrix::removeCol(unsigned col) {
	for (unsigned i = 0; i < this->numRows; i++) {
		this->data[i].erase(this->data[i].begin() + col);
	}
	this->numCols--;
}

Matrix Matrix::getRow(unsigned row) {
	Matrix res(0, 1, this->numCols);

	for (unsigned j = 0; j < this->numCols; j++) {
		res.data[row][j] = this->data[row][j];
	}

	return res;
}

Matrix Matrix::getCol(unsigned col) {
	Matrix res(0, this->numRows, 1);

	for (unsigned i = 0; i < this->numRows; i++) {
		res.data[i][col] = this->data[i][col];
	}

	return res;
}

Matrix Matrix::compare(Matrix b) {
	Matrix res(0, this->numRows, this->numCols);

	if(this->numRows == b.numRows && this->numCols == b.numCols) {
		for (unsigned i = 0; i < this->numRows; i++) {
			for (unsigned j = 0; j < this->numCols; j++) {
				res.data[i][j] = this->data[i][j] == b.data[i][j];
			}
		}
	} else {
		std::cout << "Cannot compare matrices. Size mismatch\n";
		exit(1);
	}

	return res;

}

float Matrix::toFloat() {
	if (this->numRows == 1 && this->numCols == 1) {
		return this->data[0][0];
	} else {
		cout << "Cannot convert multidimensional matrix to float";
		exit(1);
	}
}

void Matrix::print() {
	std::cout << "[";
	for (unsigned i = 0; i < this->numRows; i++) {
		for (unsigned j = 0; j < this->numCols; j++) {
			std::cout << " " << this->data[i][j] << ",";
		}
		std::cout << "\n";
	}
	std::cout << "]";
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
	if (this->numRows == rhs.numRows && this->numCols == rhs.numCols) {
		for (unsigned i = 0; i < this->numRows; i++) {
			for (unsigned j = 0; j < this->numCols; j++) {
				this->data[i][j] = this->data[i][j] + rhs.data[i][j];
			}
		}
	} else {
		std::cout << "Matrix addition failed. Size mismatch\n";
		exit(1);
	}

	return *this;
}

Matrix& Matrix::operator-=(const Matrix& rhs) {
	if (this->numRows == rhs.numRows && this->numCols == rhs.numCols) {
		for (unsigned i = 0; i < this->numRows; i++) {
			for (unsigned j = 0; j < this->numCols; j++) {
				this->data[i][j] = this->data[i][j] - rhs.data[i][j];
			}
		}
	} else {
		std::cout << "Matrix subtraction failed. Size mismatch\n";
		exit(1);
	}

	return *this;
}

Matrix& Matrix::operator*=(const Matrix& rhs) {
	Matrix a = *this;
	(*this) = a.multiply(rhs);
	return *this;
}

Matrix& Matrix::operator=(Matrix rhs)
{
	this->numRows = rhs.numRows;
	this->numCols = rhs.numCols;
	this->data = rhs.data;

	return *this;
}

Matrix operator+(Matrix lhs, const Matrix& rhs) {
	lhs += rhs;
	return lhs;
}

Matrix operator-(Matrix lhs, const Matrix& rhs) {
	lhs -= rhs;
	return lhs;
}

Matrix operator*(Matrix lhs, const Matrix& rhs) {
	lhs *= rhs;
	return lhs;
}