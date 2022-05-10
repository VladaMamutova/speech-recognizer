#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include "Processor.h"
#include "Frame.h"

using namespace std;

namespace audio {

Processor::Processor(WavData* wavData)
{
	this->wavData = wavData;
	this->frames = NULL;
	this->samplesPerFrame = 0;
}

Processor::~Processor()
{
	if (NULL != this->frames) {
		vector<Frame*>::const_iterator iter;

		for (iter = this->frames->begin(); iter != this->frames->end(); ++iter) {
			delete *iter;
		}

		delete this->frames;
	}
}

uint32_t Processor::calculateSamplesPerFrame()
{
	uint32_t bytesPerFrame = static_cast<uint32_t>(
		this->wavData->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);

	uint32_t bytesPerSample = static_cast<uint32_t>(
		this->wavData->getHeader().bitsPerSample / 8);

	uint32_t samplesPerFrame = static_cast<uint32_t>(bytesPerFrame / bytesPerSample);
	assert(samplesPerFrame > 0);
  return samplesPerFrame;
}

const WavData* Processor::getWavData() const { return this->wavData; }

void Processor::divideIntoFrames()
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

vector<Frame*>* Processor::getFrames() { return this->frames; }

void Processor::printFramesMfcc()
{ 
	vector<Frame*>::const_iterator frame;
	for (frame = this->frames->begin(); frame != this->frames->end(); ++frame) {
		cout << "[";

		double* mfcc = (*frame)->getMfcc();
		for (size_t i = 0; i < MFCC_SIZE; i++) {
			cout << mfcc[i];

			if (i < MFCC_SIZE - 1) {
				cout << ", ";
			}
		}

	  cout << "]" << endl;
	}
}

} /* namespace audio */
