#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using namespace std;

namespace math {

const double eps = 1e-6;

class Matrix {
private:
	vector<vector<double>> m;
	int nRows;
	int nCols;

public:
	/**
	 * Accuracy in the output of element
	 */
	static const streamsize ELEMENT_PRECISION;

	/**
	 * Field width of the output of element
	 */
	static const streamsize ELEMENT_WIDTH;

	/**
	 * Delimeter in the output of element
	 */
	static const string ELEMENT_DELIMETER;

	Matrix();
	Matrix(int rows, int columns);
	Matrix(const Matrix& matrix);
	Matrix(const vector<vector<double>>& data);

	Matrix& operator=(const Matrix& a);
	Matrix& operator*=(const Matrix& a); // right multiplication
	Matrix& operator*=(double k);
	Matrix& operator+=(const Matrix& a);
	Matrix& operator-=(const Matrix& a);

	int numRows() const;
	int numCols() const;
	double getElem(int row, int column) const;
	void setElem(int row, int column, double value);
	vector<vector<double>> getMatrix() const;
	bool isSymmetric() const;
	bool isDiagonal() const;
	bool hasOrthonormalColumns() const;
	double frobeniusNorm() const;

	void updateMatrix(const vector<vector<double>>& data);
	Matrix& setZeroMatrix();
	Matrix& setIdentityMatrix(int n);

	Matrix adjoint() const;

	void printMatrix() const;
	void printMatrix(ostream& os) const;

};

bool sameDouble(double a, double b);
Matrix getGivensRotation(int r, int c, double cs, double sn, int n);

Matrix operator*(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, double k);
Matrix operator*(double k, const Matrix& b);
bool operator==(const Matrix& a, const Matrix& b);
Matrix operator+(const Matrix& a, const Matrix& b);
Matrix operator-(const Matrix& a, const Matrix& b);

double maxMetric(const Matrix& a, const Matrix& b);

ostream& operator<<(ostream& os, const Matrix& m);

} /* namespace math */

#endif /* MATRIX_H */