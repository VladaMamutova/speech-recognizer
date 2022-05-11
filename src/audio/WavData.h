/**
 * Represents WAV file data.
 */

#ifndef WAV_DATA_H
#define WAV_DATA_H

#include <cstdint>
#include <iostream>
#include <string>
#include "../config.h"
#include "WavHeader.h"

using namespace std;

namespace audio {

/**
 * WAV data
 */
class WavData {
public:
	WavData();
	~WavData();

	uint32_t getNumberOfSamples() const;
	void setNumberOfSamples(uint32_t numberOfSamples);

	raw_t getMaxVal() const;
	void setMaxVal(raw_t maxVal);

	raw_t getMinVal() const;
	void setMinVal(raw_t minVal);

	const WavHeader& getHeader() const;
	const raw_t* getRawData() const;
	const double* getNormalizedData() const;

	static WavData* readFromFile(const string& file);

private:
	WavHeader header;
	raw_t* rawData;
	double* normalizedData;

	raw_t maxVal;
	raw_t minVal;
	uint32_t numberOfSamples;

	WavData(WavHeader header);
	static void readData(fstream& fs, const WavHeader& wavHeader, WavData& wavFile);
};

} // namespace audio

#endif /* WAV_DATA_H */
