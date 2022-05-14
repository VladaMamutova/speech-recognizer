#ifndef SPEECH_PROCESSOR_H
#define SPEECH_PROCESSOR_H

#include <vector>
#include <map>
#include <utility>
#include <memory>
#include "../config.h"
#include "WavData.h"
#include "Frame.h"

using namespace std;

namespace audio {

/**
 * Speech Processor is a mediator which splits raw data into frames and words
 * and keeps relations between these objects
 */
class SpeechProcessor {
public:
	SpeechProcessor(WavData* wavData);
	~SpeechProcessor();

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

#endif /* SPEECH_PROCESSOR_H */
