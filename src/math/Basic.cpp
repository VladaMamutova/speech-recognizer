#include <cmath>
#include <cstdlib>
#include <limits>
#include "Basic.h"

using namespace std;

namespace math {

double Basic::rms(const raw_t* source, uint32_t start, uint32_t finish) {
	double value = 0;

	for (uint32_t i = start; i <= finish; i++) {
		value += source[i] * source[i];
	}
	value /= (finish - start + 1);

	return sqrt(value);
}

double Basic::entropy(const double* source, uint32_t start, uint32_t finish,
		uint8_t binsCount, double minRaw, double maxRaw) {
	double entropy = 0;

	double binSize = abs(maxRaw - minRaw) / static_cast<double>(binsCount);
	if (fabs(binSize) < numeric_limits<double>::epsilon()) {
		return 0;
	}

	double* p = new double[binsCount];
	for (uint8_t i = 0; i < binsCount; i++) {
		p[i] = 0.;
	}

	// Calculate probabilities
	uint8_t index;
	for (uint32_t i = start; i <= finish; i++) {
		double value = source[i];
		index = floor((value - minRaw) / binSize);

		if (index >= binsCount) {
			index = binsCount - 1;
		}

		p[index] += 1.;
	}

	// Normalize probabilities
	uint8_t size = finish - start + 1;
	for (uint8_t i = 0; i < binsCount; i++) {
		p[i] /= size;
	}

	// Calculate entropy
	for (uint8_t i = 0; i < binsCount; i++) {
		if (p[i] > numeric_limits<double>::epsilon()) {
			entropy += p[i] * log2(p[i]);
		}
	}

	delete [] p;

	entropy = -entropy;
	return entropy;
}

double Basic::euclideanDistance(const double* a, const double* b, size_t size) {
	double distance = 0;

	for (size_t i = 0; i < size; i++) {
		distance += pow((a[i] - b[i]), 2);
	}

	return sqrt(distance);
}

double Basic::euclideanDistanceWithWeights(const double* a, const double* b, const double* weights, size_t size) {
	double distance = 0;

	for (size_t i = 0; i < size; i++) {
		distance += pow((a[i] - b[i]), 2) * weights[i];
	}

	return sqrt(distance);
}

vector<double> Basic::rankify(const vector<double> &values) 
{
	int N = values.size();
	vector<double> ranks(N);
	
	for(int i = 0; i < N; i++)
	{
		int r = 1, s = 1;
		for(int j = 0; j < i; j++) 
		{
			if (values[j] < values[i] ) r++;
			if (values[j] == values[i] ) s++;
		}
		for (int j = i+1; j < N; j++) 
		{
			if (values[j] < values[i] ) r++;
			if (values[j] == values[i] ) s++;
		}
		ranks[i] = r + (s-1) * 0.5;		
	}

	return ranks;
}


double Basic::spearmanCorrelation(const vector<double> &x, const vector<double> &y)
{
	vector<double> xRanks = rankify(x);
	vector<double> yRanks = rankify(y);

	int n = x.size();
	double xSum = 0, ySum = 0, xySum = 0, x2Sum = 0, y2Sum = 0;
	for (int i = 0; i < n; i++)
	{
		xSum = xSum + xRanks[i];
		ySum = ySum + yRanks[i];
		xySum = xySum + xRanks[i] * yRanks[i];
		x2Sum = x2Sum + xRanks[i] * xRanks[i];
		y2Sum = y2Sum + yRanks[i] * yRanks[i];
	}
	double p1 = n * xySum -xSum * ySum;
	double p2 = n * x2Sum -xSum * xSum;
	double p3 = n * y2Sum -ySum * ySum;
	double spearman = p1 / sqrt(p2 * p3);
	return spearman;
}

double Basic::DwtDistance(const vector<double*> &actual, const vector<double*> &training, int size)
{
	int m = actual.size();
    int n = training.size();

    vector<vector<double>> cost = vector<vector<double>>(m, vector<double>(n));

    cost[0][0] = euclideanDistance(actual[0], training[0], size);

    for(int i = 1; i < m; i++)
            cost[i][0] = cost[i-1][0] + euclideanDistance(actual[i], training[0], size);
    for(int j = 1; j < n; j++)
            cost[0][j] = cost[0][j-1] + euclideanDistance(actual[0], training[j], size);

    for(int i = 1; i < m; i++)
            for(int j = 1; j < n; j++)
                cost[i][j] = min(cost[i-1][j], min(cost[i][j-1], cost[i-1][j-1])) 
                    + euclideanDistance(actual[i], training[j], size);
    
    return cost[m-1][n-1];

}

} /* namespace math */
