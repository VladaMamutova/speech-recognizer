#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "../audio/AudioProcessor.h"
#include "../model/Storage.h"
#include "../model/SpeechProcessor.h"

using namespace audio;
using namespace model;

namespace command {

/**
 * This processor creates commands based on input parameters
 */
class CommandProcessor {
public:
	CommandProcessor(int argc, char** argv);
	~CommandProcessor();

	void process();

private:
	int argc;
	char** argv;

	AudioProcessor* audioProcessor;
	SpeechProcessor* speechProcessor;
	Storage* storage;

	void checkInputArguments();
	void readSpeechData();

	void printVersion();
	void printHelp();

	void printPhonemeFeatures(const char* phonemeLabel);

	void displayMfcc();
	void predictPhonemes();

};

} /* namespace command */

#endif /* COMMAND_PROCESSOR_H */
