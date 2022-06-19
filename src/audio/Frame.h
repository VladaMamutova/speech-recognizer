#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include "../config.h"
#include "../math/MFCC.h"
#include "MfccFeatures.h"

using namespace math;

namespace audio {

/**
 * Frame is a short fragment of a signal
 */
class Frame {

public:
	Frame(uint32_t number, uint32_t start, uint32_t end);
	~Frame();

	/**
	 * Init the frame using a part of wave data
	 */
	void init(const raw_t* source, const double* sourceNormalized, uint32_t frequency);

	uint32_t getNumber() const;
	uint32_t getStart() const;
	uint32_t getEnd() const;

	double getRms() const;
	double getEntropy() const;
	MfccFeatures* getMfcc() const;

private:
	const uint32_t number;
	const uint32_t start;
	const uint32_t end;

	double rms;
	double entropy;
	MfccFeatures* mfcc;
};

} /* namespace audio */

#endif /* FRAME_H */
