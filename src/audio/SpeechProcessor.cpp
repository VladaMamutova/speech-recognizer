#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include "SpeechProcessor.h"
#include "Frame.h"

using namespace std;

namespace audio {

SpeechProcessor::SpeechProcessor(WavData* wavData)
{
	this->wavData = wavData;
	this->frames = NULL;
	this->samplesPerFrame = 0;
}

SpeechProcessor::~SpeechProcessor()
{
	if (this->frames != NULL) {
		vector<Frame*>::const_iterator frame;

		for (frame = this->frames->begin(); frame != this->frames->end(); ++frame) {
			delete *frame;
		}

		delete this->frames;
	}
}

const WavData* SpeechProcessor::getWavData() const { return this->wavData; }

void SpeechProcessor::divideIntoFrames()
{
  if (this->frames != NULL) return;

  cout << "Dividing into frames and calculating MFCC... " << endl;
  this->samplesPerFrame = calculateSamplesPerFrame();
  this->frames = new vector<Frame*>();

	unsigned int samplesPerNonOverlap =
		static_cast<unsigned int>(this->samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int framesCount =
		(this->wavData->getHeader().subchunk2Size / (this->wavData->getHeader().bitsPerSample / 8))
			/ samplesPerNonOverlap;

  uint32_t size = wavData->getNumberOfSamples();
	uint32_t indexBegin = 0, indexEnd = 0;
	for (uint32_t frameId = 0; frameId < framesCount && indexEnd < size; ++frameId) {
		indexBegin = frameId * samplesPerNonOverlap;
		indexEnd = indexBegin + samplesPerFrame;
		if (indexEnd < size) {
			Frame* frame = new Frame(frameId, indexBegin, indexEnd);
			frame->init(this->wavData->getRawData(),this->wavData->getNormalizedData(),
				this->wavData->getHeader().samplesPerSec);

			this->frames->insert(this->frames->begin() + frameId, frame);
		}
	}
}

vector<Frame*>* SpeechProcessor::getFrames() { return this->frames; }

void SpeechProcessor::printFramesMfcc()
{ 
	vector<Frame*>::const_iterator frame;
	for (frame = this->frames->begin(); frame != this->frames->end(); ++frame) {	
		cout << *(*frame)->getMfcc() << endl;
	}
}

uint32_t SpeechProcessor::calculateSamplesPerFrame()
{
	uint32_t bytesPerFrame = static_cast<uint32_t>(
		this->wavData->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);

	uint32_t bytesPerSample = static_cast<uint32_t>(
		this->wavData->getHeader().bitsPerSample / 8);

	uint32_t samplesPerFrame = static_cast<uint32_t>(bytesPerFrame / bytesPerSample);
	assert(samplesPerFrame > 0);
	return samplesPerFrame;
}

} /* namespace audio */
