#ifndef STORAGE_H
#define STORAGE_H

#include <cstdint>
#include <map>
#include "PhonemeMap.h"

using namespace std;

namespace model {

/**
 * Just a simple binary storage (requires improved efficiency).
 */
class Storage {

public:
	Storage();
	~Storage();

	const PhonemeMap* getPhonemeMap();

	/**
	 * Load a code book from storage file
	 */
	bool loadPhonemes();

	/**
	 * Flush data into FS
	 */
	bool persist();

	// Read helpers
	static void readHeader(istream& stream, string name);
	static int readNamedInt(istream& stream, string name, bool isPositive);
	static double readNamedDouble(istream& stream, string name);
	static string readNamedString(istream& stream, string name);
	static void processReadResults(bool isOk, string name, string actualValue);

	// Common attributes
	static const char* STORAGE_FILE;
	static const char* PHONEMES_DIRECTORY;

private:

	/**
	 * Phoneme Map
	 */
	PhonemeMap* phonemeMap;
};

} /* namespace model */

#endif /* STORAGE_H */
