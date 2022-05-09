#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <map>
#include <utility>
#include <memory>
#include "../common.h"
#include "Wav.h"
#include "Frame.h"

namespace audio {

/**
 * Processor is a mediator which splits raw data into frames and words
 * and keeps relations between these objects
 */
class Processor {
public:

	Processor(WavData* wavData);
	~Processor();

	void init();
	void initMfcc(Frame* frame);

	const std::vector<Frame*>* getFrames() const { return this->frames; }

private:
	WavData* wavData;

	std::vector<Frame*>* frames;
	std::map<uint32_t, std::pair<uint32_t, uint32_t> >* frameToRaw;
	uint32_t samplesPerFrame;

	void divideIntoFrames();
};

typedef Processor* ProcessorPtr;

} /* namespace audio */

#endif /* PROCESSOR_H */
