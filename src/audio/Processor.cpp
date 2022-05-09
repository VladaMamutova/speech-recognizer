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

	this->frames = new vector<Frame*>();
	this->frameToRaw = new map<uint32_t, std::pair<uint32_t, uint32_t> >();
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

	if (NULL != this->frameToRaw) {
		delete this->frameToRaw;
	}
}

void Processor::init()
{
	// Init "samples per frame" measure
	uint32_t bytesPerFrame = static_cast<uint32_t>(
			this->wavData->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);
	uint32_t bytesPerSample = static_cast<uint32_t>(
			this->wavData->getHeader().bitsPerSample / 8);
	this->samplesPerFrame = static_cast<uint32_t>(bytesPerFrame / bytesPerSample);
	assert(this->samplesPerFrame > 0);

	// The main part of splitting
	divideIntoFrames();
}

void Processor::initMfcc(Frame* frame)
{
	uint32_t rawBegin = (*this->frameToRaw)[frame->getId()].first;
	uint32_t rawFinish = (*this->frameToRaw)[frame->getId()].second;

	frame->initMFCC(this->wavData->getNormalizedData(), rawBegin, rawFinish,
			this->wavData->getHeader().samplesPerSec);
}

void Processor::divideIntoFrames()
{
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
			Frame* frame = new Frame(frameId);
			frame->init(this->wavData->getRawData(), this->wavData->getNormalizedData(),
					indexBegin, indexEnd);

			this->frames->insert(this->frames->begin() + frameId, frame);
			this->frameToRaw->insert(std::make_pair(frameId, make_pair(indexBegin, indexEnd)));
		}
	}
}

} /* namespace audio */
