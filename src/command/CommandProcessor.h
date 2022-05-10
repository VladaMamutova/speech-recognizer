#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "../audio/Processor.h"

using namespace audio;

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
	// Input data
	int argc;
	char** argv;

	/**
	 * Speech processor
	 */
	Processor* speechProcessor;

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
	 * Display the MFCC coefficients retrieved from the input data
	 */
	void displayMfcc();
};

} /* namespace command */

#endif /* COMMAND_PROCESSOR_H */
