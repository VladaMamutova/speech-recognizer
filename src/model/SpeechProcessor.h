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

    const vector<PhonemePrediction*>* findLabelsByFeatures(const vector<MfccFeatures*>* features);

private:
	Storage* storage;
};

} /* namespace model */

#endif /* SPEECH_PROCESSOR_H */
