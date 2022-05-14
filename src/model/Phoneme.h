#ifndef PHONEME_H
#define PHONEME_H

#include <map>
#include <vector>
#include <cstring>
#include "../audio/MfccFeatures.h"

using namespace std;
using namespace audio;

namespace model {

/**
 * The Phoneme is represented by a label and a MFCC feature vector.
 */
class Phoneme {
public:
	Phoneme(string label);
	Phoneme(string label, vector<MfccFeatures*>* featureVector);
	~Phoneme();

	string getLabel();
	vector<MfccFeatures*>* getFeatureVector();
	size_t getFeatureVectorSize();
	void addFeatures(MfccFeatures* features);
	friend ostream& operator<<(ostream& stream, const Phoneme& phoneme);

private:
	string label;
	vector<MfccFeatures*>* featureVector;
};

} /* namespace model */

#endif /* PHONEME_H */

