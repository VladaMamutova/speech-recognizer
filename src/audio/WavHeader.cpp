#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include "WavHeader.h"

using namespace std;

namespace audio {

bool WavHeader::isValid()
{
	if (strncmp(riff, "RIFF", sizeof(riff) != 0)
		|| strncmp(wave, "WAVE", sizeof(wave) != 0)) {
		fprintf(stderr, "Invalid RIFF/WAVE format\n");
		return false;
	}

	if (audioFormat != 1) {
		fprintf(stderr, "Invalid WAV format: only PCM audio format is supported\n");
		return false;
	}

	if (numOfChan > 2) {
		fprintf(stderr, "Invalid WAV format: only 1 or 2 channels audio is supported\n");
		return false;
	}

	unsigned long bitsPerChannel = bitsPerSample / numOfChan;
	if (bitsPerChannel != 16) {
		fprintf(stderr, "Invalid WAV format: only 16-bit per channel is supported\n");
		return false;
	}

	assert(subchunk2Size > 0);

	return true;
}

} // namespace audio
