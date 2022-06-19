#ifndef MFCC_FEATURES_H
#define MFCC_FEATURES_H

#include <iostream>
#include <vector>

using namespace std;

namespace audio {

class MfccFeatures {

public:
	/**
	 * Accuracy in the output of MFCC
	 */
	static const streamsize MFCC_PRECISION;

	/**
	 * Field width of the output of MFCC
	 */
	static const streamsize MFCC_WIDTH;

	/**
	 * Delimeter in the output of MFCC
	 */
	static const string MFCC_DELIMETER;

	/** 
	 * Fixed size of mfcc data	
	 */
	static const size_t SIZE;

	MfccFeatures();
	MfccFeatures(double* data, int frameId);
	~MfccFeatures();

	inline size_t getSize() const { return SIZE; } // inline for optimization
	inline double* getData() const { return this->data; }
	inline int getFrameId() const { return this->frameId; }

	friend ostream& operator<<(ostream& stream, const MfccFeatures& mfccFeatures);
	friend ostream& operator<<(ostream& stream, const vector<MfccFeatures*>& featureVector);
	friend istream& operator>>(istream& stream, MfccFeatures& mfccFeatures);
private:
	double*	data;
	int frameId;
};

} /* namespace audio */

#endif /* MFCC_FEATURES_H */
