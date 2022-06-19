#ifndef BASIC_H
#define BASIC_H

#include <vector>
#include <stdint.h>
#include "../config.h"

using namespace std;

namespace math {

/**
 * Common math
 */
class Basic {
public:

	/**
	 * Calculate Root Mean Square
	 * @see http://en.wikipedia.org/wiki/Root_mean_square
	 */
	static double rms(const raw_t* source, uint32_t start, uint32_t finish);

	/**
	 * Calculate entropy for data sequence
	 * @see http://en.wikipedia.org/wiki/Entropy_(information_theory)
	 */
	static double entropy(const double* source, uint32_t start, uint32_t finish,
			uint8_t binsCount, double minRaw, double maxRaw);

	static double euclideanDistance(const double* a, const double* b, size_t size);
	static double euclideanDistanceWithWeights(const double* a, const double* b, const double* weights, size_t size);

	static double spearmanCorrelation(const vector<double> &x, const vector<double> &y);
	static vector<double> rankify(const vector<double> &values);
	static double DwtDistance(const vector<double*> &actual, const vector<double*> &training, int size);
};

} /* namespace math */

#endif /* BASIC_H */
