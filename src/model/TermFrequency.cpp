#include <algorithm> // transform
#include <cctype> // towlower
#include <dirent.h>
#include <iomanip>
#include <string.h>
#include <codecvt>
#include <locale.h>
#include "TermFrequency.h"

namespace model {
const wchar_t TermFrequency::SEPARATORS[] = L".,;!?()/*-—…\"\'“«»\n\t 1234567890";
const streamsize TermFrequency::FREQUENCY_WIDTH = 3;
const string TermFrequency::FREQUENCY_DELIMETER = " ";

TermFrequency::TermFrequency()
{
	this->table = new map<wstring, vector<int>*>();
	this->header = new vector<string>();
}

TermFrequency::~TermFrequency()
{
	if (this->table != NULL) {
		map<wstring, vector<int>*>::iterator row;
		for (row = this->table->begin(); row != this->table->end(); row++)
		{
			if (row->second != NULL) {
				row->second->clear();
				delete row->second;
			}
		}

		delete this->table;
	}

	if (this->header != NULL) {
		delete this->header;
	}
}

int TermFrequency::getTermCount()
{
	return table->size();
}

map<wstring, vector<int>*>* TermFrequency::getTable()
{
	return table;
}

int TermFrequency::getDocumentCount()
{
	return header->size();
}

void TermFrequency::addDocumentTermFrequency(string documentName, map<wstring, int> termFrequency, bool fillMissingValues = true)
{
	this->header->push_back(documentName);
	map<wstring, int>::iterator row;
	for (row = termFrequency.begin(); row != termFrequency.end(); row++)
	{
		vector<int>* values = (*this->table)[row->first];
		if (values == NULL) {
			values = new vector<int>();
			(*this->table)[row->first] = values;
		}

		size_t missingCount = header->size() - values->size() - 1;
		for (size_t i = 0; i < missingCount; i++) {
			values->push_back(0);
		}
		values->push_back(row->second);
	}

	if (fillMissingValues) {
		this->fillMissingValues();
	}
}

void TermFrequency::fillMissingValues()
{
	map<wstring, vector<int>*>::iterator row;
	for (row = table->begin(); row != table->end(); row++)
	{
		size_t missingCount = header->size() - row->second->size();
		for (size_t i = 0; i < missingCount; i++) {
			row->second->push_back(0);
		}
	}
}

TermFrequency* TermFrequency::createFromDirectory(const char* directoryName)
{
	locale::global(locale("")); // "" - set default locale (or "ru_RU.UTF-8") to display russian characters

	TermFrequency *termFrequency = new TermFrequency();
	DIR *directory;
	if ((directory = opendir(directoryName)) == NULL) {
		cout << "Error: Failed to load corpus from directory \"" << directoryName << "\"" << endl;
		return NULL;
	}

	struct dirent *file;
	while ((file = readdir(directory)) != NULL) {
		const char *dot = strrchr(file->d_name, '.');
		if (dot != NULL && strcmp(dot, ".txt") == 0) {
			string fileName = string(directoryName) + "/" + file->d_name;
			cout << "File \"" << fileName << "\"";

			wfstream fstream;
			fstream.open(fileName);
			if (!fstream.is_open()) {
				cout << "Error: File \"" << fileName << "\" not found.\n";
				return NULL;
			}

			vector<wstring> words = getWords(fstream);
			fstream.close();
			cout << " processed: " << words.size() << " words read";

			map<wstring, int> frequency = termFrequency->getPhonemePairsFrequency(words);
			cout << ", " << frequency.size() << " different phoneme pairs obtained." << endl;

			string documentName = file->d_name;
			documentName = documentName.substr(0, documentName.find_last_of('.'));
			termFrequency->addDocumentTermFrequency(documentName, frequency, false);
		}
	}
	termFrequency->fillMissingValues();

    // cout << *termFrequency;

	return termFrequency;
}

string wstringToString(const wstring& wstr)
{
    using convert_typeX = codecvt_utf8<wchar_t>;
    wstring_convert<convert_typeX, wchar_t> converter;

    return converter.to_bytes(wstr);
}

ostream& operator<<(ostream& stream, const TermFrequency& termFrequency)
{
	map<wstring, vector<int>*>::iterator row;
	for (row = termFrequency.table->begin(); row != termFrequency.table->end(); row++)
	{
		// we can't mix wcout and cout:
		// https://stackoverflow.com/questions/22950412/c-cant-get-wcout-to-print-unicode-and-leave-cout-working,
		// so convert wstring to string
		stream << wstringToString(row->first) << ": ";
		
		for (size_t i = 0; i < row->second->size(); ++i)
		{
			stream << setw(TermFrequency::FREQUENCY_WIDTH) << (*row->second)[i];

			if (i < row->second->size() - 1) {
			 	stream << TermFrequency::FREQUENCY_DELIMETER;
			}
		}

		stream << endl;
	}

	return stream;
}

vector<wstring> TermFrequency::getWords(wfstream& fstream)
{
	vector<wstring> words;
	wstring word;

	while (fstream >> word) // read the word (until the next space)
	{
		wchar_t* ptr;
		wchar_t* token =  wcstok(const_cast<wchar_t*>(word.c_str()), SEPARATORS, &ptr);
		while (token != NULL)
		{
			word = wstring(token);
			transform(word.begin(), word.end(), word.begin(), towlower);
			words.push_back(word);

			token = wcstok(NULL, SEPARATORS, &ptr); // continue with the previous word
		}
	}

	return words;
}

map<wstring, int> TermFrequency::getPhonemePairsFrequency(vector<wstring> words)
{
	map<std::wstring, int> frequency;
	wstring phonemePair = L"  ";

	vector<wstring>::iterator word;
	for (word = words.begin(); word != words.end(); word++)
	{
		for (size_t i = 0; i < (*word).size(); ++i)
		{
			if (i + 1 < (*word).size()) {
				phonemePair[0] = (*word)[i];
				phonemePair[1] = (*word)[i + 1];
				
				// if there is no previous key entry,
				// "+=" creates the entry and sets its value to one.
				frequency[phonemePair] += 1;
			}	
		}
	}

	return frequency;
}

} /* namespace model */
