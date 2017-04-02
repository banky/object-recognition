using namespace std;

Matrix() {
	this.numRows = numRows_;
	this.numCols = numCols_;

	this.data = 
		vector< vector<float> > float(numRows_, vector<float>(numCols_));
}

Matrix(float * values, unsigned numRows_, unsigned numCols_) {
	this.numRows = numRows_;
	this.numCols = numCols_;

	this.data = 
		vector< vector<float> > float(numRows_, vector<float>(numCols_));

	if (values != NULL) {
		for (unsigned i = 0; i < numRows_; i++) {
			for (unsigned j = 0; j < numCols_; j++) {
				this.data[i][j] = values[i][j];
			}
		}
	}
}

Matrix multiply(Matrix b) {
	Matrix res(NULL, this.numRows, b.numCols);

	// Matrices cannot be multiplied
	if (this.numCols == b.numRows) {
		for (unsigned i = 0; i < this.numRows; i++) {
			for (unsigned j = 0; j < b.numCols; j++) {
				for (unsigned k = 0; k < this.numCols; k++) {
					res.data[i][j] = this.data[i][k]*b.data[k][j];
				}
			}
		}
	}

	return res;
}

Matrix transpose() {
	Matrix res(NULL, this.numCols, this.numRows);

	for (unsigned i = 0; i < this.numRows; i++) {
		for (unsigned j = 0; j < this.numCols; j++) {
			res.data[j][i] = this.data[i][j];
		}
	}

	return res;
}

Matrix addOnesRow() {
	Matrix res(NULL, this.numRows + 1, this.numCols);

	for (unsigned i = 0; i <= this.numRows; i++) {
		for (unsigned j = 0; j < this.numCols; j++) {
			if (i = 0) {
				res.data[i][j] = 1;
			} else {
				res.data[i][j] = this.data[i - 1][j];
			}
		}
	}

	return res;
}

Matrix addOnesCol() {
	Matrix res(NULL, this.numRows, this.numCols + 1);

	for (unsigned i = 0; i < this.numRows; i++) {
		for (unsigned j = 0; j <= this.numCols; j++) {
			if (j = 0) {
				res.data[i][j] = 1;
			} else {
				res.data[i][j] = this.data[i][j - 1];
			}
		}
	}

	return res;
}

Matrix getRow(unsigned row) {
	Matrix res(NULL, 1, this.numCols);

	for (unsigned j = 0; j < this.numCols; j++) {
		res.data[j] = this.data[row][j];
	}

	return res;
}

Matrix getCol(unsigned col) {
	Matrix res(NULL, this.numRows, 1);

	for (unsigned i = 0; i < this.numRows; i++) {
		res.data[i] = this.data[i][col];
	}

	return res;
}

Matrix& operator+=(const Matrix& rhs) {
	if (this.numRows == rhs.numRows && this.numCols == rhs.numCols) {
		for (unsigned i = 0; i < this.numRows; i++) {
			for (unsigned j = 0; j < this.numCols; j++) {
				this.data[i][j] = this.data[i][j] + rhs.data[i][j];
			}
		}
	}

	return *this;
}

Matrix& operator-=(const Matrix& rhs) {
	if (this.numRows == rhs.numRows && this.numCols == rhs.numCols) {
		for (unsigned i = 0; i < this.numRows; i++) {
			for (unsigned j = 0; j < this.numCols; j++) {
				this.data[i][j] = this.data[i][j] - rhs.data[i][j];
			}
		}
	}

	return *this;
}

Matrix& operator-=(const Matrix& rhs) {
	this = this.multiply(rhs);

	return *this;
}

inline Matrix operator+(Matrix lhs, const Matrix& rhs) {
	lhs += rhs;
	return lhs;
}

inline Matrix operator-(Matrix lhs, const Matrix& rhs) {
	lhs -= rhs;
	return lhs;
}

inline Matrix operator*(Matrix lhs, const Matrix& rhs) {
	lhs *= rhs;
	return lhs;
}




