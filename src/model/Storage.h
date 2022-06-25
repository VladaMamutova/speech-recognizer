#ifndef STORAGE_H
#define STORAGE_H

#include <cstdint>
#include <map>
#include "PhonemeMap.h"
#include "../math/Tree.h"

using namespace std;

namespace model {

/**
 * Just a simple binary storage (requires improved efficiency).
 */
class Storage {

public:
	Storage();
	~Storage();

	const PhonemeMap* fetchPhonemeMap();
	const PhonemeMap* fetchPhonemePairMap();

	void updatePhonemes(const char* sourceDirectory);
	void updatePhonemePairs(const char* sourceDirectory);

	/**
	 * Load data from a storage file
	 */
	PhonemeMap* loadFromStorageFile(const char* storageFile);

	/**
	 * Create a storage file from a directory
	 */
	PhonemeMap* createFromDirectory(const char* storageFile, const char* sourceDirectory);

	// Read helpers
	static void readHeader(istream& stream, string name);
	static int readNamedInt(istream& stream, string name, bool isPositive);
	static double readNamedDouble(istream& stream, string name);
	static string readNamedString(istream& stream, string name);
	static void processReadResults(bool isOk, string name, string actualValue);

	// Common attributes
	static const char* STORAGE_DIRECTORY;
	static const char* PHONEMES_FILE;
	static const char* PHONEME_PAIRS_FILE;

private:

	/**
	 * Phoneme Map
	 */
	PhonemeMap* phonemeMap;

	/**
	 * Phoneme Pair Map
	 */
	PhonemeMap* phonemePairMap;

	// /**
	//  * Phoneme Pair Correlation Map
	//  */
	// map<wstring, Tree<string, double>::Node*>* phonemePairCorrelation;

	string getStorageFilePath(const char* storageFile);
};

} /* namespace model */

#endif /* STORAGE_H */
