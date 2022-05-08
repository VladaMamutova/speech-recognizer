/**
 * Represents WAV file data.
 *
 * Currently supports only PCM format.
 */
#ifndef WAV_H
#define WAV_H

#include <cstdint>
#include <iostream>
#include <string>
#include "../common.h"

using namespace std;

namespace audio {

/**
 * WAV header
 */
struct WavHeader {
    char              riff[4];        // RIFF Header
    int               chunkSize;      // RIFF Chunk Size
    char              wave[4];        // WAVE Header
    char              fmt[4];         // FMT header
    unsigned int      subchunk1Size;  // Size of the fmt chunk
    unsigned short    audioFormat;    // Audio format 1=PCM (Other formats are unsupported)
    unsigned short    numOfChan;      // Number of channels 1=Mono, 2=Stereo
    unsigned int			samplesPerSec;  // Sampling Frequency in Hz
    unsigned int      bytesPerSec;    // bytes per second
    unsigned short    blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short    bitsPerSample;  // Number of bits per sample

    // The data below depends on audioFormat, but we work only with PCM cases
    char              data[4];        // DATA header
    unsigned int      subchunk2Size;  // Sampled data length

		static WavHeader* readFromFile(std::fstream& fs);
};

class WavData;

/**
 * WAV data
 */
class WavData {
public:

	~WavData() {
		if (NULL != this->rawData) {
			delete [] this->rawData;
		}
		if (NULL != this->normalizedData) {
			delete [] this->normalizedData;
		}
	}

	static WavData* readFromFile(const string& file);

	uint32_t getNumberOfSamples() const { return numberOfSamples; }
	void setNumberOfSamples(uint32_t numberOfSamples) { this->numberOfSamples = numberOfSamples; }

	raw_t getMaxVal() const { return maxVal; }
	void setMaxVal(raw_t maxVal) { this->maxVal = maxVal; }

	raw_t getMinVal() const { return minVal; }
	void setMinVal(raw_t minVal) { this->minVal = minVal; }

	const WavHeader& getHeader() const { return header; }
	const raw_t* getRawData() const { return rawData; }
	const double* getNormalizedData() const { return normalizedData; }

private:
	WavHeader		header;
	raw_t*			rawData;
	double*			normalizedData;

	raw_t			maxVal;
	raw_t			minVal;
	uint32_t		numberOfSamples;

	WavData(WavHeader header) {
		this->header = header;
		this->rawData = NULL;
		this->normalizedData = NULL;

		this->maxVal = 0;
		this->minVal = 0;
		this->numberOfSamples = 0;
	}

	static bool checkHeader(const WavHeader& wavHeader);
	static void readData(fstream& fs, const WavHeader& wavHeader, WavData& wavFile);
};

} // namespace audio

#endif /* WAV_H */
