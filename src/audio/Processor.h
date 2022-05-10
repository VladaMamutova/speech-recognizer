#ifndef PROCESSOR_H
#define PROCESSOR_H // rename to audio processor

#include <vector>
#include <map>
#include <utility>
#include <memory>
#include "../common.h"
#include "Wav.h"
#include "Frame.h"

using namespace std;

namespace audio {

/**
 * Processor is a mediator which splits raw data into frames and words
 * and keeps relations between these objects
 */
class Processor {
public:

	Processor(WavData* wavData);
	~Processor();

  const WavData* getWavData() const;

  void divideIntoFrames();
  vector<Frame*>* getFrames();
  void printFramesMfcc();

private:
	WavData* wavData;

	vector<Frame*>* frames;
	uint32_t samplesPerFrame;

  uint32_t calculateSamplesPerFrame();
};

} /* namespace audio */

#endif /* PROCESSOR_H */
