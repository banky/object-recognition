#include <vector>

typedef std::vector< std::vector<float> > tbt; // Two dimensional matrix

class Matrix {
	private:
		const unsigned numRows;
		const unsigned numCols;
		tbt data;
	public:
		Matrix multiply(Matrix b);
		Matrix transpose();
		Matrix addOnesRow();
		Matrix addOnesCol();
		Matrix();
		Matrix(float * values, unsigned numRows_, unsigned numCols_);
}