#include <stdlib.h>
#include <math.h>
#include "Phoneme.h"

namespace model {

Phoneme::Phoneme(string label)
{
	this->label = label;
	this->featureVector = new vector<MfccFeatures*>();
}

Phoneme::Phoneme(string label, vector<MfccFeatures*>* featureVector)
{
	this->label = label;
	this->featureVector = featureVector;
}

Phoneme::~Phoneme()
{
	vector<MfccFeatures*>::iterator features;
	for (features = featureVector->begin(); features != featureVector->end(); features++) {
		delete *features;
	}
	delete featureVector;
}

string Phoneme::getLabel() { return label; }
vector<MfccFeatures*>* Phoneme::getFeatureVector() { return featureVector; }
size_t Phoneme::getFeatureVectorSize() { return featureVector->size(); }

void Phoneme::addFeatures(MfccFeatures* entry) { featureVector->push_back(entry); }

ostream& operator<<(ostream& stream, const Phoneme& phoneme)
{
	vector<MfccFeatures*>::iterator features;
	for (features = phoneme.featureVector->begin(); features != phoneme.featureVector->end(); features++) {
		stream << **features << endl;
	}

	return stream;
}

} /* namespace model */
