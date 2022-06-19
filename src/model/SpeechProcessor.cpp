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

vector<PhonemePrediction*>* SpeechProcessor::predictPhonemesByFeatures(const vector<MfccFeatures*>* mfccFeatures)
{
	vector<PhonemePrediction*>* predictions = new vector<PhonemePrediction*>();
	const PhonemeMap* phonemeMap = storage->fetchPhonemeMap();

	cout << "Phonemes:" << endl;

	vector<MfccFeatures*>::const_iterator features;
	int i = 0;
	for (features = mfccFeatures->begin(); features != mfccFeatures->end(); ++features) {
		MfccFeatures* currentFeature = *features;
		PhonemePrediction* prediction = phonemeMap->predictLabelByFeatures(currentFeature);

		predictions->push_back(prediction);

		cout << i << ": " << *prediction << endl;
		i++;
	}

	return predictions;
}

vector<PhonemePrediction*>* SpeechProcessor::predictPhonemePairsByFeatures(const vector<MfccFeatures*>* mfccFeatures)
{
	vector<PhonemePrediction*>* predictions = new vector<PhonemePrediction*>();
	const PhonemeMap* phonemePairMap = storage->fetchPhonemePairMap();

	cout << "Phoneme pairs:" << endl;

	for (size_t i = 0; i + 10 - 5 < mfccFeatures->size(); i += 2) {
		//cout << (i / 10) << " block: " << endl;
		vector<double*> featureVector;
		for (size_t j = i; j < i + 10 && j < mfccFeatures->size(); j++)
		{
			//cout << *mfccFeatures->at(j) << endl;
			featureVector.push_back(mfccFeatures->at(j)->getData());
		}
		
		PhonemePrediction* prediction = phonemePairMap->predictLabelByFeatures(featureVector);
		
		predictions->push_back(prediction);

		cout << i << ": " << *prediction << endl;
		i++;
	}

	return predictions;
}

} /* namespace model */
