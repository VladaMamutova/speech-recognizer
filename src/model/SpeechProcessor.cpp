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

	int featureLength = phonemePairMap->calcAverageFeatureNumber();
	for (size_t i = 0; i + (int)(featureLength * 0.5) + 1 < mfccFeatures->size(); i++) {
		vector<double*> featureVector;
		size_t j;
		for (j = i; j < i + featureLength && j < mfccFeatures->size(); j++)
		{
			featureVector.push_back(mfccFeatures->at(j)->getData());
		}
		
		PhonemePrediction* prediction = phonemePairMap->predictLabelByFeatures(featureVector);
		
		predictions->push_back(prediction);

		int offset = mfccFeatures->at(0)->getFrameId();
		cout << "[" << i + offset << " - " << j + offset - 1 << "]: " << *prediction << endl;
	}

	return predictions;
}

void SpeechProcessor::recognize(AudioProcessor* audioProcessor)
{
	vector<pair<uint32_t, uint32_t>>* frameGroups = audioProcessor->fetchFrameGroups();
	vector<pair<uint32_t, uint32_t>>::iterator frameGroup;
	int i = 1;
	for (frameGroup = frameGroups->begin(); frameGroup != frameGroups->end(); ++frameGroup) {
		uint32_t start = frameGroup->first;
		uint32_t end = frameGroup->second;
		vector<MfccFeatures*>* features = audioProcessor->fetchFrameGroupMfccs(start - 4, end + 4);

		cout << "Word " << i << ": " << endl;
		predictPhonemePairsByFeatures(features);
		i++;
	}

}

} /* namespace model */
