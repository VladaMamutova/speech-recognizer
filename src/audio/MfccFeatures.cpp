#include <iomanip>
#include "MfccFeatures.h"
#include "../config.h"

namespace audio {

const streamsize MfccFeatures::MFCC_PRECISION = 6;
const streamsize MfccFeatures::MFCC_WIDTH = MFCC_PRECISION + 4; // 1 ('-') + 2 (numbers) + 1 ('.')
const string MfccFeatures::MFCC_DELIMETER = " ";
const size_t MfccFeatures::SIZE = MFCC_SIZE;

MfccFeatures::MfccFeatures()
{
	this->data = NULL;
}

MfccFeatures::MfccFeatures(double* data)
{
	this->data = data;
}

MfccFeatures::~MfccFeatures()
{
	if (this->data != NULL) {
		delete[] this->data;
	}
}

ostream& operator<<(ostream& stream, const MfccFeatures& mfccFeatures)
{
	stream.precision(MfccFeatures::MFCC_PRECISION);
	stream.setf(std::ios_base::fixed, std::ios_base::floatfield);

	for (size_t i = 0; i < mfccFeatures.getSize(); i++) {
		stream << setw(MfccFeatures::MFCC_WIDTH) << mfccFeatures.data[i];

		if (i < mfccFeatures.getSize() - 1) {
			stream << MfccFeatures::MFCC_DELIMETER;
		}
	}

	return stream;
}

ostream& operator<<(ostream& stream, const vector<MfccFeatures*>& featureVector)
{
	vector<MfccFeatures*>::const_iterator features;
	for (features = featureVector.begin(); features != featureVector.end(); features++) {
		stream << **features << endl;
	}
	return stream;
}

istream& operator>>(istream& stream, MfccFeatures& mfccFeatures)
{
	stream.precision(MfccFeatures::MFCC_PRECISION);
	stream.setf(std::ios_base::fixed, std::ios_base::floatfield);

	mfccFeatures.data = new double[mfccFeatures.getSize()];
	for (size_t i = 0; i < mfccFeatures.getSize(); i++) {
		if (!(stream >> setw(MfccFeatures::MFCC_WIDTH) >> mfccFeatures.data[i])) {

			cerr << "Failed to read mfcc features." << endl;
			exit(EXIT_FAILURE);
		}
	}

	return stream;
}

} /* namespace audio */
