#ifndef SPEECH_PROCESSOR_H
#define SPEECH_PROCESSOR_H

#include <vector>
#include "Storage.h"
#include "../audio/AudioProcessor.h"

using namespace std;

namespace model {

class SpeechProcessor {
public:
	SpeechProcessor(Storage* storage);
	~SpeechProcessor();

    vector<PhonemePrediction*>* predictPhonemesByFeatures(const vector<MfccFeatures*>* features);
	vector<PhonemePrediction*>* predictPhonemePairsByFeatures(const vector<MfccFeatures*>* features);

	void recognize(AudioProcessor* audioProcessor);

    //vector<PhonemeMap*> findUniqueObservations(vector<PhonemePrediction*>* predictions);
    //vector<PhonemeMap*> findSimilarPhonemePairs(Phoneme* phonemePair);
private:
	Storage* storage;
};

} /* namespace model */

#endif /* SPEECH_PROCESSOR_H */
