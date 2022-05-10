#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include <vector>
#include "../common.h"
#include "../math/MFCC.h"

using namespace math;

namespace audio {

/**
 * Frame is a short fragment of a signal
 */
class Frame {

public:
	Frame(uint32_t id, uint32_t start, uint32_t end);
	~Frame();

	/**
	 * Init the frame using a part of wave data
	 */
	void init(const raw_t* source, const double* sourceNormalized, uint32_t frequency);

	uint32_t getId() const;
	uint32_t getStart() const;
	uint32_t getEnd() const;

	double getRms() const;
	double getEntropy() const;
	double* getMfcc() const;

private:
	const uint32_t id;
	const uint32_t start;
	const uint32_t end;

	double rms;
	double entropy;
	double* mfcc;
};

} /* namespace audio */

#endif /* FRAME_H */
