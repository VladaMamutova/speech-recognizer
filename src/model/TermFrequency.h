#ifndef TERM_FREQUENCY_H
#define TERM_FREQUENCY_H

#include <map>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

namespace model {

string wstringToString(const wstring& wstr);

/**
 * The TermFrequency is a "term-document" matrix for the LSA algorithm.
 */
class TermFrequency {
public:
	/**
	 * Field width of the output of frequency
	 */
	static const streamsize FREQUENCY_WIDTH;

	/**
	 * Delimeter in the output of frequency
	 */
	static const string FREQUENCY_DELIMETER;

	TermFrequency();
	~TermFrequency();

	int getTermCount();
	int getDocumentCount();
	map<wstring, vector<int>*>* getTable();
	void addDocumentTermFrequency(string documentName, map<wstring, int> frequency, bool fillMissingValues);
	void fillMissingValues();

	static TermFrequency* createFromDirectory(const char* directoryName);
	friend ostream& operator<<(ostream& stream, const TermFrequency& termFrequency);
private:
	map<wstring, vector<int>*>* table; // term (pair of phonemes) and frequencies per documents
	vector<string>* header; // document file names

	const static wchar_t SEPARATORS[];
    static vector<wstring> getWords(wfstream& fstream);
	static map<wstring, int> getPhonemePairsFrequency(vector<wstring> words);
};

} /* namespace model */

#endif /* TERM_FREQUENCY_H */
