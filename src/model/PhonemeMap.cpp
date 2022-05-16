#include <stdlib.h>
#include <math.h>
#include <cstring>
#include <dirent.h>
#include "PhonemeMap.h"
#include "Storage.h"
#include "../audio/AudioProcessor.h"
#include "../math/Basic.h"


namespace model {

const string PhonemeMap::UNKNOWN_VALUE = "?";
const string PHONEME_MAP = "PHONEME_MAP";
const double MFCC_WEIGHTS[] = { 1.2, 1.1, 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1 };
//const double MFCC_WEIGHTS[] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
//const double MFCC_WEIGHTS[] = { 1.0, 1.0, 0.9, 0.9, 0.8, 0.8, 0.7, 0.7, 0.6, 0.6, 0.5, 0.5 };

const map<string, Phoneme*>* PhonemeMap::getPhonemes() const
{
	return this->phonemes;
}

PhonemeMap::PhonemeMap()
{
	this->phonemes = new map<string, Phoneme*>();
}

PhonemeMap::~PhonemeMap()
{
	if (this->phonemes != NULL) {
		map<string, Phoneme*>::iterator phoneme;
		for (phoneme = this->phonemes->begin(); phoneme != this->phonemes->end(); phoneme++)
		{
			if (phoneme->second != NULL) {
				delete phoneme->second;
			}
		}

		delete this->phonemes;
	}
}

void PhonemeMap::addPhoneme(Phoneme* phoneme)
{
	if ((*this->phonemes)[phoneme->getLabel()] != NULL) {
		cout << "The phoneme \"" << phoneme->getLabel() << "\" already exists. It will be replaced." << endl;
		removePhoneme(phoneme->getLabel());
	}

	(*this->phonemes)[phoneme->getLabel()] = phoneme;
}

void PhonemeMap::removePhoneme(string label)
{
	this->phonemes->erase(label);
}

string PhonemeMap::findLabelByFeatures(MfccFeatures* mfccFeatures) const
{
	double minDistance = -1;
	string label = UNKNOWN_VALUE;

	// The first values in a MFCC vector are more meaningful than
	// the last ones. That's why we use the MFCC_WEIGHTS.

	map<string, Phoneme*>::const_iterator phoneme;
	for (phoneme = this->phonemes->begin(); phoneme != this->phonemes->end(); phoneme++)
	{
		//cout << "\t" << phoneme->first << ": ";

		vector<MfccFeatures*>::const_iterator features;
		for (features = phoneme->second->getFeatureVector()->begin();
			features != phoneme->second->getFeatureVector()->end();
			features++)
		{
			MfccFeatures* currentMfccFeatures = *features;

			double distance = math::Basic::euclideanDistanceWithWeights(mfccFeatures->getData(),
				currentMfccFeatures->getData(), MFCC_WEIGHTS, currentMfccFeatures->getSize());

			if (UNKNOWN_VALUE.compare(label) == 0 || minDistance > distance) {
				minDistance = distance;
				label = phoneme->first;
			}
			//cout << distance << " ";
		}
		//cout << endl;
	}

	return label;
}

ostream& operator<<(ostream& stream, const PhonemeMap& phonemeMap)
{
	stream << PHONEME_MAP << " " <<  phonemeMap.phonemes->size() << endl;

	map<string, Phoneme*>::iterator phoneme;
	for (phoneme = phonemeMap.phonemes->begin(); phoneme != phonemeMap.phonemes->end(); phoneme++)
	{
		stream << phoneme->first << " " << phoneme->second->getFeatureVectorSize() << endl;
		stream << *phoneme->second->getFeatureVector();
	}

	return stream;
}

istream& operator>>(istream& stream, PhonemeMap& phonemeMap)
{
	phonemeMap.phonemes->clear();

	size_t phonemeMapSize = Storage::readNamedInt(stream, PHONEME_MAP, true);

	for (size_t i = 0; i < phonemeMapSize; i++)
	{
		string label;
		if (!(stream >> label)) {
			cerr << "Invalid PhonemeMap: label should be a string" << endl;
			exit(EXIT_FAILURE);
		}

		//cout << "Read phoneme [" << label << "]" << endl;

		size_t featureVectorSize;
		if (!(stream >> featureVectorSize)) {
			cerr << "Invalid PhonemeMap: featureVector count must be a positive string" << endl;
			exit(EXIT_FAILURE);
		}

		Phoneme* phoneme = new Phoneme(label);
		for (size_t j = 0; j < featureVectorSize; j++)
		{
			MfccFeatures* mfccFeatures = new MfccFeatures();
			stream >> *mfccFeatures;

			phoneme->addFeatures(mfccFeatures);
		}
	
		(*phonemeMap.phonemes)[phoneme->getLabel()] = phoneme;
	}

	return stream;
}

PhonemeMap* PhonemeMap::loadFromDirectory(const char* directoryName)
{
	DIR *directory;
	if ((directory = opendir(directoryName)) == NULL) {
		cout << "Error: Failed to load phonemes from directory \"" << directoryName << "\"" << endl;
		return NULL;
	}

	cout << "Reading phoneme files from the directory \"" << directoryName << "\"..." << endl;

	PhonemeMap* phonemeMap = new PhonemeMap();
	struct dirent *phonemeFile;
	while ((phonemeFile = readdir(directory)) != NULL) {
		if (WavData::isValidExtension(phonemeFile->d_name)) {
			string path = string(directoryName) + "/" + phonemeFile->d_name;
			WavData* wavData = WavData::readFromFile(path);
			if (wavData != NULL) {
				string label = phonemeFile->d_name;
				label = label.substr(0, label.find_last_of('.'));

				AudioProcessor *audioProcessor = new AudioProcessor(wavData);
				audioProcessor->divideIntoFrames();

				Phoneme* phoneme = new Phoneme(label, audioProcessor->getFrameMfccs());
				phonemeMap->addPhoneme(phoneme);
			}
		}
	}
	closedir(directory);
	return phonemeMap;
}

} /* namespace model */
