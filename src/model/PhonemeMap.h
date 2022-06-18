#ifndef PHONEME_MAP_H
#define PHONEME_MAP_H

#include <map>
#include <vector>
#include <cstring>
#include "Phoneme.h"
#include "PhonemePrediction.h"

using namespace std;
using namespace audio;

namespace model {

/**
 * PhonemeMap
 *
 * The PhonemeMap is used for vector quantization process. It contains a set of
 * spectral vectors (mfcc) templates and related output labels. Using the PhonemeMap
 * you can convert a sequence of spectral vectors from input data into a sequence of
 * best template labels (discrete observations).
 */
class PhonemeMap {
public:
	PhonemeMap();
	~PhonemeMap();

	const map<string, Phoneme*>* getPhonemes() const;
	inline size_t getSize() const { return phonemes->size(); };

	void addPhoneme(Phoneme *phoneme);
	void removePhoneme(string phonemeLabel);

	PhonemePrediction* findLabelByFeatures(MfccFeatures* mfccFeatures) const;

	friend ostream& operator<<(ostream& stream, const PhonemeMap& phonemeMap);
	friend istream& operator>>(istream& stream, PhonemeMap& phonemeMap);

	static PhonemeMap* loadFromDirectory(const char* directoryName);

	/**
	 * Default value for unrecognised MFCC samples
	 */
	static const string UNKNOWN_VALUE;

private:
	map<string, Phoneme*>* phonemes;
};


} /* namespace model */

#endif /* PHONEME_MAP_H */
