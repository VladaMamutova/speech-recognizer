#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <map>
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
	vector<Frame*>* fetchFrames();
	vector<MfccFeatures*>* fetchFrameMfccs();
	vector<pair<uint32_t, uint32_t>>* fetchFrameGroups();
	vector<MfccFeatures*>* fetchFrameGroupMfccs(uint32_t frameGroupStart, uint32_t frameGroupEnd);

private:
	WavData* wavData;

	vector<Frame*>* frames;
	uint32_t samplesPerFrame;

	vector<pair<uint32_t, uint32_t>>* frameGroups;

	double rmsMax;
	double silenceThreshold;

	uint32_t calculateSamplesPerFrame();
	void divideIntoFrames();
	void divideIntoFrameGroupsBySilence();

	int processSilence(uint32_t firstFrameInGroup, uint32_t currentFrame,
		uint32_t& previousGroupStart, uint32_t& previousGroupEnd);
	bool findSilenceThreshold();
	void cleanUpShortFrameGroups();
	void mergeAllFramesIntoOneGroup();
};

} /* namespace audio */

#endif /* AUDIO_PROCESSOR_H */
