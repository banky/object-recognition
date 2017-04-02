#include <vector>

typedef std::vector< std::vector<float> > tbt; // Two dimensional matrix

class Matrix {
	private:
		const unsigned numRows;
		const unsigned numCols;
		tbt data;
	public:
		// Constructors
		Matrix();
		Matrix(float * values, unsigned numRows_, unsigned numCols_);
		Matrix(float value, unsigned numRows_, unsigned numCols_);

		// Methods
		Matrix multiply(Matrix b);
		Matrix transpose();
		Matrix addOnesRow();
		Matrix addOnesCol();
		Matrix getBinary();
		Matrix getRow(unsigned row);
		Matrix getCol(unsigned col);
		
		// Operators
		Matrix& operator+=(const Matrix& rhs);
		Matrix& operator-=(const Matrix& rhs);
		Matrix& operator*=(const Matrix& rhs);
};

inline Matrix operator+(Matrix lhs, const Matrix& rhs);
inline Matrix operator-(Matrix lhs, const Matrix& rhs);
inline Matrix operator*(Matrix lhs, const Matrix& rhs);
