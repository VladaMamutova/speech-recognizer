#include <iostream>
#include <cassert>
#include <iomanip>
#include <cmath>

#include "Matrix.h"

namespace math {

bool sameDouble(double a, double b) {
	return fabs(a - b) < eps;
}

double dotProduct(const vector<double>& a, const vector<double>& b) {
	assert(a.size() == b.size());
	double sum = 0;
	for (size_t i = 0; i < a.size(); ++i) {
		sum += a[i]*b[i];
	}
	return sum;
}

const streamsize Matrix::ELEMENT_PRECISION = 6;
const streamsize Matrix::ELEMENT_WIDTH = ELEMENT_PRECISION + 4;
const string Matrix::ELEMENT_DELIMETER = " ";

Matrix::Matrix(const Matrix &matrix) {
	m = matrix.getMatrix();
	nRows = matrix.numRows();
	nCols = matrix.numCols();
}

Matrix::Matrix() {
	Matrix(3, 3);
}

Matrix::Matrix(int rows, int columns) {
	assert(rows > 0);
	assert(columns > 0);

	m = vector<vector<double>>(rows, vector<double>(columns));
	nRows = rows;
	nCols = columns;
}

Matrix::Matrix(const vector<vector<double>>& data) {

	updateMatrix(data);
}

Matrix& Matrix::operator=(const Matrix& a) {
	if (this != &a) {
		m = a.getMatrix();
		nRows = a.numRows();
		nCols = a.numCols();
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& a) {
	if (this != &a) {
		Matrix tmp(m);
		*this = tmp * a;
	}
	return *this;
}

Matrix& Matrix::operator*=(double k) {
	Matrix tmp(m);
	*this = tmp * k;
	return *this;
}

Matrix& Matrix::operator+=(const Matrix& a) {
	if (this != &a) {
		Matrix tmp(m);
		*this = tmp + a;
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
	if (this != &a) {
		Matrix tmp(m);
		*this = tmp - a;
	}
	return *this;
}

int Matrix::numRows() const {
	return nRows;
}

int Matrix::numCols() const {
	return nCols;
}

double Matrix::getElem(int row, int column) const {
	assert(row >= 0);
	assert(column >= 0);
	assert(row < nRows);
	assert(column < nCols);

	return m[row][column];
}

void Matrix::setElem(int row, int column, double value) {
	assert(row >= 0);
	assert(column >= 0);
	assert(row < nRows);
	assert(column < nCols);

	m[row][column] = value;
}

vector<vector<double>> Matrix::getMatrix() const {
	return m;
}

bool Matrix::isSymmetric() const {
	for (int row = 0; row < nRows; ++row) {
		for (int column = 0; column < nCols; ++column) {
			if (!sameDouble(m[row][column], m[column][row])) return false;
		}
	}
	return true;
}

bool Matrix::isDiagonal() const {
	for (int row = 0; row < nRows; ++row) {
		for (int column = 0; column < nCols; ++column) {
			if (row == column) continue;
			if (!sameDouble(m[row][column], 0.0)) return false;
		}
	}
	return true;
}

bool Matrix::hasOrthonormalColumns() const {
	Matrix tmp = adjoint();
	vector<vector<double>> matrix = tmp.getMatrix();

	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix.size(); ++j) {
			if (i == j) {
				if (!sameDouble(dotProduct(matrix[i], matrix[j]), 1)) {
					return false;
				}
			} else {
				if (!sameDouble(dotProduct(matrix[i], matrix[j]), 0)) {
					return false;
				}
			}
		}
	}
	return true;
}

double Matrix::frobeniusNorm() const {
	double norm = 0;
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			norm += m[i][j]*m[i][j];
		}
	}
	return sqrt(norm);
}

void Matrix::updateMatrix(const vector<vector<double>>& data) {
	assert(data.size() > 0);
	for (size_t i = 0; i < data.size() - 1; ++i) {
		assert(data[i].size() == data[i + 1].size());
	}

	m = data;
	nRows = m.size();
	nCols = m[0].size();
}

Matrix& Matrix::setZeroMatrix() {
	for (auto row = m.begin(); row != m.end(); ++row) {
		for (auto column = row->begin(); column != row->end(); ++column) {
			*column = 0;
		}
	}
	return *this;
}

Matrix& Matrix::setIdentityMatrix(int n) {
	m = vector<vector<double>>(n, vector<double>(n));

	for (int row = 0; row < n; ++row) {
		for (int column = 0; column < n; ++column) {
			if (row != column) {
				m[row][column] = 0;
			} else {
				m[row][column] = 1;
			}
		}
	}
	nRows = n;
	nCols = n;

	return *this;
}

Matrix Matrix::adjoint() const {
	Matrix adj(nCols, nRows);

	for (int column = 0; column < nCols; ++column) {
		for (int row = 0; row < nRows; ++row) {
			adj.setElem(column, row, m[row][column]);
		}
	}
	return adj;
}

void Matrix::printMatrix() const {
	printMatrix(cout);
}

void Matrix::printMatrix(ostream& os) const {
	cout.precision(ELEMENT_PRECISION);
	cout.setf(std::ios_base::fixed, std::ios_base::floatfield);

	for (auto row = m.begin(); row != m.end(); ++row) {
		for (auto column = row->begin(); column != row->end(); ++column){
			os << setw(ELEMENT_WIDTH) << *column << ELEMENT_DELIMETER;
		}
		os << endl;
	}

}

Matrix getGivensRotation(int row, int column, double cs, double sn, int n) {
	Matrix givens;
	givens.setIdentityMatrix(n);
	givens.setElem(row, row, cs);
	givens.setElem(row, column, sn);
	givens.setElem(column, row, -sn);
	givens.setElem(column, column, cs);
	return givens;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
	
	assert(a.numCols() == b.numRows());

	Matrix res(a.numRows(), b.numCols());

	for (int i = 0; i < a.numRows(); ++i) {
		for (int j = 0; j < b.numCols(); ++j) {
			double val = 0;
			for (int k = 0; k < a.numCols(); ++k) {
				val += a.getElem(i, k)*b.getElem(k, j);
			}
			res.setElem(i, j, val);
		}
	}

	return res;
}

Matrix operator*(double k, const Matrix& b) {
	
	Matrix res(b.numRows(), b.numCols());

	for (int i = 0; i < b.numRows(); ++i) {
		for (int j = 0; j < b.numCols(); ++j) {
			res.setElem(i, j, k*b.getElem(i, j));
		}
	}

	return res;
}

Matrix operator*(const Matrix& b, double k) {
	return (k * b);
}

Matrix operator+(const Matrix& a, const Matrix& b) {
	assert(a.numCols() == b.numCols() && a.numRows() == b.numRows());

	Matrix res(a.numRows(), a.numCols());

	for (int i = 0; i < a.numRows(); ++i) {
		for (int j = 0; j < b.numCols(); ++j) {
			res.setElem(i, j, a.getElem(i, j) + b.getElem(i, j));
		}
	}
	return res;
}

Matrix operator-(const Matrix& a, const Matrix& b) {
	Matrix column = a;
	Matrix d = b;
	d *= -1;
	Matrix res = column + d;
	return res;
}

bool operator==(const Matrix& a, const Matrix& b) {
	if (a.numCols() != b.numCols() || a.numRows() != b.numRows()) return false;

	for (int row = 0; row < a.numRows(); ++row) {
		for (int column = 0; column < a.numCols(); ++column) {
			if (!sameDouble(a.getElem(row, column), b.getElem(row, column))) return false;
		}
	}
	return true;
}

double maxMetric(const Matrix& a, const Matrix& b) {
	assert(a.numCols() == b.numCols() && a.numRows() == b.numRows());

	double distance = 0;

	for (int row = 0; row < a.numRows(); ++row) {
		for (int column = 0; column < a.numCols(); ++column) {
			distance = fmax(distance, fabs(a.getElem(row, column) - b.getElem(row, column)));
		}
	}
	return distance;
}

ostream& operator<<(ostream& os, const Matrix& m) {
	m.printMatrix(os);
	return os;
}

} /* namespace math */
