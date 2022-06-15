#ifndef LSA_TRAINER_H
#define LSA_TRAINER_H

#include "TermFrequency.h"
#include "../math/Matrix.h"
#include "../math/Tree.h"

using namespace std;
using namespace math;

namespace model {

/**
 * LSA Trainer is used to calculate the correlation between pairs of phonemes.
 */
class LsaTrainer {
public:
	LsaTrainer(const char* directoryName);
	~LsaTrainer();

	void train();

private:
	const char* corpusDirectory;

	static vector<wstring> prepareMatrix(map<wstring, vector<int>*>* termFrequency, Matrix &matrix);
	map<wstring, Tree<string, double>::Node*>* calculateCorrelation(vector<wstring> terms, Matrix matrix);
};

} /* namespace model */

#endif /* LSA_TRAINER_H */
