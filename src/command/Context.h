#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include <Wav.h>

using namespace audio;

namespace command {

/**
 * Command execution context
 */
class Context {
public:

	Context() {
		this->wavData = NULL;
	}

	~Context() {
		if (NULL != this->wavData) {
			delete this->wavData;
		}
	}

	WavData* getWavData() { return this->wavData; }
	void setWavData(WavData* wavData) { this->wavData = wavData; }

private:
	WavData* wavData;
};

} /* namespace command */

#endif /* CONTEXT_H */
