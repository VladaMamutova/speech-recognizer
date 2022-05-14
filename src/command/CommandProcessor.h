#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "../audio/AudioProcessor.h"
#include "../model/Storage.h"

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
	// Input arguments
	int argc;
	char** argv;

	/**
	 * Audio processor
	 */
	AudioProcessor* audioProcessor;

	/**
	 * Speech data storage
	 */
	Storage* storage;

	/**
	 * Checks whether input arguments are specified
	 */
	void checkInputArguments();

	/**
	 * Reads speech input data
	 */
	void readSpeechData();

	/**
	 * Checks if the speech input data specified
	 */
	void checkSpeechData();

	/**
	 * Print build information
	 */
	void printVersion();
	
	/**
	 * Print help information
	 */
	void printHelp();

	/**
	 * Display the MFCC features retrieved from the input data
	 */
	void displayMfcc();

	/**
	 * Display the MFCC features for the phoneme
	 */
	void printPhonemeFeatures(const char* phonemeLabel);
};

} /* namespace command */

#endif /* COMMAND_PROCESSOR_H */
