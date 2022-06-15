#ifndef SVD_H
#define SVD_H

#include "Matrix.h"

namespace math {

class SVD {
private:
	Matrix A;

	Matrix U;
	Matrix Sigma;
	Matrix V;

public:
	SVD(const Matrix &A);

	void calculate();

	Matrix getU() const;
	Matrix getSigma() const;
	Matrix getV() const;

	Matrix lowRankApproximation(int rank);

	void printSVD() const;
	void printSVD(std::ostream& os) const;
};

} /* namespace math */

#endif