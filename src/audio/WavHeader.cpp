#include <limits.h>
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
	if (0 != strncmp(riff, "RIFF", sizeof(riff))
		|| 0 != strncmp(wave, "WAVE", sizeof(wave))) {
		fprintf(stderr, "Invalid RIFF/WAVE format\n");
		return false;
	}

	if (1 != audioFormat) {
		fprintf(stderr, "Invalid WAV format: only PCM audio format is supported\n");
		return false;
	}

	if (numOfChan > 2) {
		fprintf(stderr, "Invalid WAV format: only 1 or 2 channels audio is supported\n");
		return false;
	}

	unsigned long bitsPerChannel = bitsPerSample / numOfChan;
	if (16 != bitsPerChannel) {
		fprintf(stderr, "Invalid WAV format: only 16-bit per channel is supported\n");
		return false;
	}

	assert(subchunk2Size > 0);

	return true;
}

} // namespace audio
