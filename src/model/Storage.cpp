#include <fstream>
#include <utility>
#include <sys/stat.h> // mkdir
#include <unistd.h> // access()
#include "../config.h"
#include "Storage.h"

namespace model {

const char* Storage::STORAGE_DIRECTORY = "storage";
const char* Storage::PHONEMES_FILE = "phonemes.dat";
const char* Storage::PHONEME_PAIRS_FILE = "phoneme_pairs.dat";

Storage::Storage()
{
	this->phonemeMap = NULL;
	this->phonemePairMap = NULL;
	mkdir(STORAGE_DIRECTORY, S_IRWXU | S_IROTH);
}

Storage::~Storage()
{
	if (phonemeMap != NULL) {
		delete phonemeMap;
	}

	if (phonemePairMap != NULL) {
		delete phonemePairMap;
	}
}

const PhonemeMap* Storage::fetchPhonemeMap()
{
	if (phonemeMap != NULL) {
		return phonemeMap;
	}

	phonemeMap = loadFromStorageFile(PHONEMES_FILE);
	return phonemeMap;
}

const PhonemeMap* Storage::fetchPhonemePairMap()
{
	if (phonemePairMap != NULL) {
		return phonemePairMap;
	}

	phonemePairMap = loadFromStorageFile(PHONEME_PAIRS_FILE);
	return phonemePairMap;
}

void Storage::updatePhonemes(const char* sourceDirectory)
{
	if (phonemeMap != NULL) {
		delete phonemeMap;
	}

	phonemeMap = createFromDirectory(PHONEMES_FILE, sourceDirectory);
}

void Storage::updatePhonemePairs(const char* sourceDirectory)
{
	if (phonemePairMap != NULL) {
		delete phonemePairMap;
	}

	phonemePairMap = createFromDirectory(PHONEME_PAIRS_FILE, sourceDirectory);
}

PhonemeMap* Storage::loadFromStorageFile(const char* storageFile)
{
	string storageFilePath = string(STORAGE_DIRECTORY) + "/" + storageFile;

	ifstream stream;
	stream.open(storageFilePath, ios::in);

	if (!stream.good()) {
		cerr << "Failed to open storage file \"" << storageFilePath << "\"." << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Loading data from the storage file \"" << storageFilePath << "\"..." << endl;

	PhonemeMap* data = new PhonemeMap();
	stream >> *data;
	stream.close();

	cout << "\nData from storage file successfully loaded!" << endl;
	
	return data;
}

PhonemeMap* Storage::createFromDirectory(const char* storageFile, const char* sourceDirectory)
{
	string storageFilePath = getStorageFilePath(storageFile);

	ofstream stream;
	stream.open(storageFilePath, ios::out | ios::binary);

	if (!stream.good()) {
		cerr << "Error: Failed to create storage file \"" << storageFilePath << "\"." << endl;
		exit(EXIT_FAILURE);
	}

	PhonemeMap* data = PhonemeMap::loadFromDirectory(sourceDirectory);
	if (data == NULL) {
		exit(EXIT_FAILURE);
	}

	stream << *data;
	stream.close();

	cout << "Storage file \"" << storageFilePath << "\" with " << data->getSize();
	cout << " models successfully created!" << endl;
	return data;
}

string Storage::getStorageFilePath(const char* storageFile) {
	return string(STORAGE_DIRECTORY) + "/" + storageFile;
}

void Storage::readHeader(istream& stream, string name)
{
	string temp;

	bool isOk = static_cast<bool>(stream >> temp);
	processReadResults(isOk, name, temp);
}

int Storage::readNamedInt(istream& stream, string name, bool isPositive) {
	int32_t result = 0;
	string temp;

	bool isOk = static_cast<bool>(stream >> temp >> result);
	processReadResults(isOk, name, temp);

	if (isPositive && result < 0) {
		cerr << "Invalid storage: " << name << " should be a positive value" << endl;
		exit(EXIT_FAILURE);
	}

	return result;
}

double Storage::readNamedDouble(istream& stream, string name) {
	double result = 0;
	string tmpStr;

	bool isOk = static_cast<bool>(stream >> tmpStr >> result);
	processReadResults(isOk, name, tmpStr);

	return result;
}

string Storage::readNamedString(istream& stream, string name) {
	string result;
	string temp;

	bool isOk = static_cast<bool>(stream >> temp >> result);
	processReadResults(isOk, name, temp);

	return result;
}

void Storage::processReadResults(bool isOk, string name, string actualValue) {

	if (isOk) {
		if (name.compare(actualValue) != 0) {
			cerr << "Invalid storage: " << name << " is missing" << endl;
			exit(EXIT_FAILURE);
		}

	} else {
		cerr << "Invalid storage: " << name << " is missing or corrupted" << endl;
		exit(EXIT_FAILURE);
	}
}

} /* namespace model */
