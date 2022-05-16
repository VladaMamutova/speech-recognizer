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

const vector<string>* SpeechProcessor::findLabelsByFeatures(const vector<MfccFeatures*>* mfccFeatures)
{
	vector<string>* labels = new vector<string>();
	const PhonemeMap* phonemeMap = storage->getPhonemeMap();

	cout << "Labels:" << endl;

	vector<MfccFeatures*>::const_iterator features;
	int i = 0;
	for (features = mfccFeatures->begin(); features != mfccFeatures->end(); ++features) {
		MfccFeatures* currentMfccFeatures = *features;
		string label = phonemeMap->findLabelByFeatures(currentMfccFeatures);

		labels->push_back(label);

		cout << label << ": " << *currentMfccFeatures << endl;
		i++;
	}

	return labels;
}

} /* namespace model */
