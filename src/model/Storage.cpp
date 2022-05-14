#include <fstream>
#include <utility>
#include <unistd.h> // access()
#include "../config.h"
#include "Storage.h"

namespace model {

const char* Storage::STORAGE_FILE = "storage.dat";
const char* Storage::PHONEMES_DIRECTORY = "resources/phonemes";

Storage::Storage()
{
	this->phonemeMap = NULL;
}

Storage::~Storage()
{
	if (phonemeMap != NULL) {
		delete phonemeMap;
	}
}

const map<string, Phoneme*>* Storage::getPhonemeMap()
{
	if (phonemeMap == NULL) {
		loadPhonemes();
	}

	return phonemeMap->getPhonemes();
}

bool Storage::loadPhonemes()
{
	if (access(STORAGE_FILE, F_OK) == -1) {
		cout << "Storage not found, creating an empty one..." << endl;
		return persist();
	}

	cout << "Loading data from the storage..." << endl;

	ifstream stream;
	stream.open(STORAGE_FILE, ios::in);

	if (!stream.good()) {
		cerr << "Failed to open storage file \"" << STORAGE_FILE << "\"" << endl;
		exit(EXIT_FAILURE);
	}

	phonemeMap = new PhonemeMap();
	stream >> *(phonemeMap);

	stream.close();

	return true;
}

bool Storage::persist()
{
	ofstream stream;
	stream.open(STORAGE_FILE, ios::out | ios::binary);

	if (!stream.good()) {
		cerr << "Error: Failed to open storage file \"" << STORAGE_FILE << "\"" << endl;
		return false;
	}

	phonemeMap = PhonemeMap::loadFromDirectory(PHONEMES_DIRECTORY);
	stream << *phonemeMap;

	stream.close();
	cout << "\nStorage data successfully updated!" << endl;

	return true;
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
