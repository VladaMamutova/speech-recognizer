#ifndef PHONEME_H
#define PHONEME_H

#include "../config.h"
#include "LsaTrainer.h"
#include "../math/SVD.h"
#include "../math/Basic.h"

using namespace std;
using namespace math;

namespace model {


LsaTrainer::LsaTrainer(const char* directoryName)
{
	this->corpusDirectory = directoryName;
}


void LsaTrainer::train()
{
	TermFrequency *termFrequency = TermFrequency::createFromDirectory(corpusDirectory);
	int termCount = termFrequency->getTermCount();
	int documentCount = termFrequency->getDocumentCount();
	Matrix termFrequencyMatrix = Matrix(termCount, documentCount);
	vector<wstring> terms = prepareMatrix(termFrequency->getTable(), termFrequencyMatrix);
    cout << "\nTerm Frequency Matrix (" << termCount << " x " << documentCount << ") is created!" << endl;
	
	SVD svd(termFrequencyMatrix);
	svd.calculate();
	cout << "\nSVD of the matrix has been performed." << endl;
	//svd.printSVD();

	int rank = max((int)(documentCount * K), 1);
	Matrix lowRankMatrix = svd.lowRankApproximation(rank);
	cout << "\nThe rank of the matrix has been reduced to " << rank << "." << endl;

	//map<wstring, Tree<string, double>::Node*>* correlationMap = calculateCorrelation(terms, lowRankMatrix);
	// TODO:: запись в файл
}

vector<wstring> LsaTrainer::prepareMatrix(map<wstring, vector<int>*>* termFrequency, Matrix &matrix)
{
	vector<wstring> terms;
	map<wstring, vector<int>*>::iterator row;
	int i = 0;
	for (row = termFrequency->begin(); row != termFrequency->end(); row++)
	{
		terms.push_back(row->first);
		for (size_t j = 0; j < row->second->size(); j++) {
			matrix.setElem(i, j, (double)(row->second->at(j)));
		}

		i++;
	}

	return terms;
}

map<wstring, Tree<string, double>::Node*>* LsaTrainer::calculateCorrelation(vector<wstring> terms, Matrix matrix)
{
	// TODO::
	map<wstring, Tree<string, double>::Node*>* correlationMap = new map<wstring, Tree<string, double>::Node*>();
	for (int i = 0; i < matrix.numRows(); ++i) {
		// инициализировать дерево для терма
		// correlationMap[terms[i]] = vector<double>(matrix.numCols());
		for (int j = 0; j < matrix.numCols(); ++j) {
			// Получить коэф. для двух строк матрицы
			// Добавить для i-го терма узел дерева - j-й ключ и коэф. коррел.
		}
	}

	return correlationMap;
}

} /* namespace model */

#endif /* PHONEME_H */
