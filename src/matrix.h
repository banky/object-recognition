#include <vector>
#include <iostream>

typedef std::vector< std::vector<float> > tbt; // Two dimensional matrix

class Matrix {
	public:
		unsigned numRows;
		unsigned numCols;
		tbt data;
		
		// Constructors
		Matrix();
		Matrix(tbt values, unsigned numRows_, unsigned numCols_);
		Matrix(float value, unsigned numRows_, unsigned numCols_);

		// Methods
		Matrix multiply(Matrix b);
		Matrix transpose();
		Matrix addOnesRow();
		Matrix addOnesCol();
		Matrix getBinary();
		Matrix getRow(unsigned row);
		Matrix getCol(unsigned col);
		void print();
		
		// Operators
		Matrix& operator+=(const Matrix& rhs);
		Matrix& operator-=(const Matrix& rhs);
		Matrix& operator*=(const Matrix& rhs);
		Matrix& operator=(Matrix rhs);
};

Matrix operator+(Matrix lhs, const Matrix& rhs);
Matrix operator-(Matrix lhs, const Matrix& rhs);
Matrix operator*(Matrix lhs, const Matrix& rhs);