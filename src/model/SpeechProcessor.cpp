#include <iterator>
#include "SpeechProcessor.h"

namespace model {

SpeechProcessor::SpeechProcessor(Storage* storage)
{
	this->storage = storage;
}

SpeechProcessor::~SpeechProcessor()
{
	// Storage is an independent object, SpeechProcessor doesn't own it.
}

const vector<PhonemePrediction*>* SpeechProcessor::predictPhonemesByFeatures(const vector<MfccFeatures*>* mfccFeatures)
{
	vector<PhonemePrediction*>* predictions = new vector<PhonemePrediction*>();
	const PhonemeMap* phonemeMap = storage->fetchPhonemeMap();

	cout << "Labels:" << endl;

	vector<MfccFeatures*>::const_iterator features;
	int i = 0;
	for (features = mfccFeatures->begin(); features != mfccFeatures->end(); ++features) {
		MfccFeatures* currentFeature = *features;
		PhonemePrediction* prediction = phonemeMap->findLabelByFeatures(currentFeature);

		predictions->push_back(prediction);

		cout << i << ": " << *prediction << endl;
		i++;
	}

	return predictions;
}

} /* namespace model */
