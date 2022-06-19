#ifndef SPEECH_PROCESSOR_H
#define SPEECH_PROCESSOR_H

#include <vector>
#include "Storage.h"

using namespace std;

namespace model {

class SpeechProcessor {
public:
	SpeechProcessor(Storage* storage);
	~SpeechProcessor();

    vector<PhonemePrediction*>* predictPhonemesByFeatures(const vector<MfccFeatures*>* features);
	vector<PhonemePrediction*>* predictPhonemePairsByFeatures(const vector<MfccFeatures*>* features);

private:
	Storage* storage;
};

} /* namespace model */

#endif /* SPEECH_PROCESSOR_H */
