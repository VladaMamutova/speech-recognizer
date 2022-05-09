#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Context.h>

namespace command {

/**
 * This processor creates commands based on input parameters
 */
class CommandProcessor {
public:
	CommandProcessor(int argc, char** argv):
		argc(argc), argv(argv) {

		this->context = new Context();
	};

	~CommandProcessor() {
		delete this->context;
	}

	void process();

private:

	// Input data
	int argc;
	char** argv;

	/**
	 * Current execution context
	 */
	Context* context;

	/**
	 * Print build information
	 */
  void printVersion();
	
	/**
	 * Print help information
	 */
	void printHelp();

	/**
	 * Read Wav data from the file
	 */
	bool readData(Context& context, const char* inputFile);

	/**
	 * Display the MFCC coefficients retrieved from the input data
	 */
	void displayMfcc(Context& context);
};

} /* namespace command */

#endif /* COMMAND_PROCESSOR_H */
