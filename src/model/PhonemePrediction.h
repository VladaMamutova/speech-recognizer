#ifndef PHONEME_PREDICTION_H
#define PHONEME_PREDICTION_H

#include <vector>
#include <string>
#include "../math/Tree.h"
#include "../audio/MfccFeatures.h"

using namespace std;
using namespace math;
using namespace audio;

namespace model {

/**
 * The PhonemePrediction is a map of the most likely phonemes on the frame.
 */
class PhonemePrediction {
public:
	/**
	 * Accuracy in the output of DISTANCE
	 */
	static const streamsize DISTANCE_PRECISION;

	/**
	 * Delimeter in the output of PREDICTION
	 */
	static const string PREDICTION_DELIMETER;

	PhonemePrediction();
	~PhonemePrediction();

	void addPrediction(string phonemeLabel, double distance);

	friend ostream& operator<<(ostream& stream, const PhonemePrediction& PhonemePrediction);

private:
	Tree<string, double>* predictions; // phoneme label and distance
};

} /* namespace model */

#endif /* PHONEME_PREDICTION_H */
