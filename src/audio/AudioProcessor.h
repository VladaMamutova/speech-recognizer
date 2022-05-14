#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

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
 * AudioProcessor is a mediator which splits raw data into frames,
 * calculates their MFCC features, keeps relations between these objects.
 */
class AudioProcessor {
public:
	AudioProcessor(WavData* wavData);
	~AudioProcessor();

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

#endif /* AUDIO_PROCESSOR_H */
