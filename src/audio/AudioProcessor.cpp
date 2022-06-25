#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include "AudioProcessor.h"

using namespace std;

namespace audio {

AudioProcessor::AudioProcessor(WavData* wavData)
{
	this->wavData = wavData;
	this->frames = NULL;
	this->samplesPerFrame = 0;

	this->frameGroups = NULL;

	this->rmsMax = 0;
	this->silenceThreshold = 0;
}

AudioProcessor::~AudioProcessor()
{
	if (frames != NULL) {
		vector<Frame*>::const_iterator frame;
		for (frame = frames->begin(); frame != frames->end(); ++frame) {
			delete *frame;
		}

		delete frames;
	}
}

const WavData* AudioProcessor::getWavData() const
{
	return wavData;
}

vector<Frame*>* AudioProcessor::fetchFrames()
{
	if (frames == NULL) {
		divideIntoFrames();
	}

	return frames;
}

vector<MfccFeatures*>* AudioProcessor::fetchFrameMfccs()
{
	if (frames == NULL) {
		divideIntoFrames();
	}

	assert(frames->size() > 0);

	vector<MfccFeatures*>* frameMfccs = new vector<MfccFeatures*>();

	vector<Frame*>::const_iterator frame;
	for (frame = frames->begin(); frame != frames->end(); ++frame) {	
		frameMfccs->push_back((*frame)->getMfcc());
	}

	return frameMfccs;
}

vector<pair<uint32_t, uint32_t>>* AudioProcessor::fetchFrameGroups()
{
	fetchFrames();
	if (frameGroups == NULL) {
		divideIntoFrameGroupsBySilence();
	}

	return frameGroups;
}

vector<MfccFeatures*>* AudioProcessor::fetchFrameGroupMfccs(uint32_t frameGroupStart, uint32_t frameGroupEnd)
{
	fetchFrameGroups();

	vector<MfccFeatures*>* frameMfccs = new vector<MfccFeatures*>();

	vector<Frame*>::const_iterator frame;
	for (frame = frames->begin(); frame != frames->end(); ++frame) {
		int frameNumber = (*frame)->getNumber();
		if ((int)frameGroupStart <= frameNumber && frameNumber <= (int)frameGroupEnd)	{
			frameMfccs->push_back((*frame)->getMfcc());
		}
	}

	return frameMfccs;
}

uint32_t AudioProcessor::calculateSamplesPerFrame()
{
	uint32_t bytesPerFrame = static_cast<uint32_t>(
		this->wavData->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);

	uint32_t bytesPerSample = static_cast<uint32_t>(
		this->wavData->getHeader().bitsPerSample / 8);

	uint32_t samplesPerFrame = static_cast<uint32_t>(bytesPerFrame / bytesPerSample);
	assert(samplesPerFrame > 0);
	return samplesPerFrame;
}

void AudioProcessor::divideIntoFrames()
{
	//cout << "Dividing into frames and calculating MFCC... " << endl;
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

void AudioProcessor::divideIntoFrameGroupsBySilence()
{
	assert(frames->size() >= 2);

	frameGroups = new vector<pair<uint32_t, uint32_t>>();

	// Let's find silence threshold
	bool hasSilence = findSilenceThreshold();

	// Divide the frames into groups by areas of silence (each group represents a single word)
	uint32_t firstFrameInGroup = -1;
	uint32_t previousGroupStart = -1;
	uint32_t previousGroupEnd = 0;

	if (hasSilence) {
		vector<Frame*>::const_iterator frame;
		for (frame = frames->begin(); frame != frames->end(); ++frame) {
			if ((*frame)->getRms() > silenceThreshold) { // Got a sound
				if ((int)firstFrameInGroup == -1) {
					firstFrameInGroup = (*frame)->getNumber();
					cout << "Got a sound at frame " << firstFrameInGroup << "." << endl;
				}
			} else { // Got silence
				if ((int)firstFrameInGroup >= 0) {
					// Let's find distance between start of the current group and end of the previous group
					processSilence(firstFrameInGroup, (*frame)->getNumber(), previousGroupStart, previousGroupEnd);
					firstFrameInGroup = -1;
				}
			}
		}

		// Clean up short frameGroups
		cleanUpShortFrameGroups();
	} else { // There is no any silence in the sound
		mergeAllFramesIntoOneGroup();
	}

	// If we have only one word let's consider whole sample
	if (hasSilence && 1 == frameGroups->size()) {
		//mergeAllFramesIntoOneGroup();
	}
}

int AudioProcessor::processSilence(uint32_t firstFrameInGroup, uint32_t currentFrame,
	uint32_t& previousGroupStart, uint32_t& previousGroupEnd)
{
	// Let's find distance between start of the current group and end of the previous group
	int distance = 0;
	bool previousFrameGroupInitialized = (int)previousGroupStart != -1 && (int)previousGroupEnd != -1;
	if (previousFrameGroupInitialized) {
		distance = (int)(firstFrameInGroup - previousGroupEnd);
	}

	// We have a new frame group
	if (!previousFrameGroupInitialized || distance >= FRAME_GROUP_MIN_DISTANCE) {
		frameGroups->push_back(make_pair(firstFrameInGroup, currentFrame));
		previousGroupStart = firstFrameInGroup;
		previousGroupEnd = currentFrame;

		cout << frameGroups->size() << " frame group" <<
			" [" << firstFrameInGroup << " - " << currentFrame << "] created." << endl;

	// We need to add the current group to the previous one
	} else if (previousFrameGroupInitialized && distance < FRAME_GROUP_MIN_DISTANCE) {
		// Compute RMS for current frame Group (word)
		double currentRms = 0;
		for (uint32_t i = firstFrameInGroup; i < currentFrame; i++) {
			currentRms += frames->at(i)->getRms();
		}
		currentRms /= currentFrame - firstFrameInGroup;

		// Add the frame group only if it has valuable RMS
		if (currentRms > silenceThreshold * 2) {
			frameGroups->erase(frameGroups->end());
			frameGroups->push_back(make_pair(previousGroupStart, currentFrame));
			cout << "Frame group [" << previousGroupStart <<  " - " << previousGroupEnd <<
				"] expanded to [" << previousGroupStart << " - " << currentFrame << "]." << endl;
			previousGroupEnd = currentFrame;
		}
	}

	return distance;
}

bool AudioProcessor::findSilenceThreshold()
{
	// Let's find max and min rms/entropy
	double rms, rmsMax, rmsSilence = 0;
	rms = rmsMax = frames->at(0)->getRms();

	// Try to guess the best threshold value
	bool hasSilence = false;
	uint32_t cnt = 0;
	vector<Frame*>::const_iterator frame;
	for (frame = frames->begin(); frame != frames->end(); ++frame) {
		rms = (*frame)->getRms();
		rmsMax = max(rmsMax, rms);

		if ((*frame)->getEntropy() < ENTROPY_THRESHOLD) {
			hasSilence = true;
			rmsSilence += (*frame)->getRms();
			cnt++;
		}
	}
	rmsSilence /= cnt;

	rmsMax = rmsMax;
	silenceThreshold = rmsSilence * 2;
	return hasSilence;
}

void AudioProcessor::cleanUpShortFrameGroups()
{
	vector<pair<uint32_t, uint32_t>>::iterator frameGroup;
	for (frameGroup = frameGroups->begin(); frameGroup != frameGroups->end(); ) {
		if (frameGroup->second - frameGroup->first < FRAME_GROUP_MIN_SIZE) {
			cout << "Frame group [" << frameGroup->first << " - " << frameGroup->second <<
				"] is too short and will be avoided." << endl;

			this->frameGroups->erase(frameGroup);
		} else {
			++frameGroup;
		}
	}
}

void AudioProcessor::mergeAllFramesIntoOneGroup()
{
	frameGroups->clear();
	frameGroups->push_back(make_pair(frames->at(0)->getNumber(), frames->back()->getNumber()));

	cout << "Seems we have only one frame group in the sample... All frames will be added into the one frame group!" << endl;
}

} /* namespace audio */
