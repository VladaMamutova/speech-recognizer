#include "Frame.h"
#include "../math/Basic.h"

namespace audio {

Frame::Frame(uint32_t id, uint32_t start, uint32_t end):
	id(id), start(start), end(end)
{
	this->rms = 0;
	this->entropy = 0;
	this->mfcc = NULL;
}

Frame::~Frame() {
	if (this->mfcc != NULL) {
		delete this->mfcc;
	}
}

void Frame::init(const raw_t* source, const double* sourceNormalized, uint32_t frequency)
{
	this->rms = Basic::rms(source, this->start, this->end);
	this->entropy = Basic::entropy(sourceNormalized, this->start, this->end, ENTROPY_BINS, -1, 1);
	double* mfcc = MFCC::transform(sourceNormalized, this->start, this->end, MfccFeatures::SIZE, frequency,
		MFCC_FREQ_MIN, MFCC_FREQ_MAX);
	this->mfcc = new MfccFeatures(mfcc);
}

uint32_t Frame::getId() const { return this->id; }
uint32_t Frame::getStart() const { return this->start; }
uint32_t Frame::getEnd() const { return this->end; }

double Frame::getRms() const { return this->rms; }
double Frame::getEntropy() const { return this->entropy; }
MfccFeatures* Frame::getMfcc() const { return this->mfcc; }

} /* namespace audio */
