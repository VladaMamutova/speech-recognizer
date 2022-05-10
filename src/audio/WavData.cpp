#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include "WavData.h"

using namespace std;

namespace audio {

WavData::WavData(WavHeader header) {
	this->header = header;
	this->rawData = NULL;
	this->normalizedData = NULL;

	this->maxVal = 0;
	this->minVal = 0;
	this->numberOfSamples = 0;
}

WavData::~WavData() {
	if (NULL != this->rawData) {
		delete [] this->rawData;
	}
	if (NULL != this->normalizedData) {
		delete [] this->normalizedData;
	}
}

uint32_t WavData::getNumberOfSamples() const { return numberOfSamples; }
void WavData::setNumberOfSamples(uint32_t numberOfSamples) { this->numberOfSamples = numberOfSamples; }

raw_t WavData::getMaxVal() const { return maxVal; }
void WavData::setMaxVal(raw_t maxVal) { this->maxVal = maxVal; }

raw_t WavData::getMinVal() const { return minVal; }
void WavData::setMinVal(raw_t minVal) { this->minVal = minVal; }

const WavHeader& WavData::getHeader() const { return header; }
const raw_t* WavData::getRawData() const { return rawData; }
const double* WavData::getNormalizedData() const { return normalizedData; }

WavData* WavData::readFromFile(const string& file)
{
	WavHeader wavHeader;

	fstream fs;
	fs.open(file.c_str(), ios::in | ios::binary);

	if (!fs.good()) {
		fprintf(stderr, "Error: input file \"%s\" not found.\n", file.c_str());
		return NULL;
	}

	cout << "Reading WAV data from \"" << file << "\"..." << endl;

	// Read header
	fs.read((char*)(&wavHeader), sizeof(WavHeader));
	if (!wavHeader.isValid()) {
		return NULL;
	}

	// Read raw data
	WavData* wavData = new WavData(wavHeader);
	readData(fs, wavHeader, *wavData);
	fs.close();

	return wavData;
}

void WavData::readData(fstream& fs, const WavHeader& wavHeader, WavData& wavFile)
{
	raw_t value, minValue = 0, maxValue = 0;
	int16_t value16, valueLeft16, valueRight16;

	uint32_t bytesPerSample = static_cast<uint32_t>(wavHeader.bitsPerSample / 8);
	unsigned long numberOfSamplesXChannels = wavHeader.subchunk2Size /
			(wavHeader.numOfChan * bytesPerSample);

	wavFile.rawData = new raw_t[numberOfSamplesXChannels];

	uint32_t sampleNumber;
	for (sampleNumber = 0; sampleNumber < numberOfSamplesXChannels && !fs.eof(); sampleNumber++) {

		if (1 == wavHeader.numOfChan) {
			fs.read((char*)(&value16), sizeof(int16_t));
			value = static_cast<raw_t>(value16);

		} else {
			fs.read((char*)(&valueLeft16), sizeof(int16_t));
			fs.read((char*)(&valueRight16), sizeof(int16_t));
			value = static_cast<raw_t>((abs(valueLeft16) + abs(valueRight16)) / 2);
		}

		if (maxValue < value) {
			maxValue = value;
		}

		if (minValue > value) {
			minValue = value;
		}

		wavFile.rawData[sampleNumber] = value;
	}
	assert(sampleNumber > 0);
	sampleNumber++;

	// Normalization
	wavFile.normalizedData = new double[sampleNumber];
	double maxAbs = max(fabs(minValue), fabs(maxValue));

	for (uint32_t i = 0; i < sampleNumber; i++) {
		wavFile.normalizedData[i] = static_cast<double>(wavFile.rawData[i]) / maxAbs;
	}

	// Update values
	wavFile.setMinVal(minValue);
	wavFile.setMaxVal(maxValue);
	wavFile.setNumberOfSamples(sampleNumber);
}

} // namespace audio
