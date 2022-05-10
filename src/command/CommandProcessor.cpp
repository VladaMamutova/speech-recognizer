#include <getopt.h>
#include <iostream>
#include <vector>
#include "CommandProcessor.h"
#include "../audio/Processor.h"

namespace command {

/**
 * Commands map
 */
static struct option longOptions[] = {
	{ "version", no_argument, 0, 'v' },
	{ "help", no_argument, 0, 'h' },

	{ "mfcc", optional_argument, 0, 'm' },
};

static const char* const shortOptions = "vhm:";

/**
 * Help info
 */
static const char* const helpInfo =	EOL
	"Usage: ./speech_recognizer [options] \033[4mFILE\033[0m" EOL EOL
	"Recognizes speech from a WAV \033[4mFILE\033[0m and displays additional data based on options." EOL

	EOL

  "\033[4mFILE\033[0m             Absolute or relative path to the wav-file for processing" EOL

  EOL

	"Options:" EOL
	"-v, --version    Display version information" EOL
	"-h, --help       Display help information" EOL

	EOL

	"-m, --mfcc       Display the mfcc coefficients retrieved from the input data"

	EOL;

void CommandProcessor::process()
{
	if (argc <= 1) {
		cout << "No input parameters specified." << endl;
		cout << "Please take a look on the help info for details:" << endl;
		printHelp();
		return;
	}

	readSpeechData();

  bool processed = false;
	int option;
	do {
		option = getopt_long(argc, argv, shortOptions, longOptions, NULL);
		switch (option) {
			case -1: // The end of the options.
			  processed = true;
			  break;
			case 'v':
				printVersion();
				break;
			case 'h':
				printHelp();
				break;
			case 'm':
			  checkSpeechData();
			  displayMfcc();
				break;
			default:
				cout << "Please, use -h (--help) for details." << endl;
				processed = true;
				break;
		}
	} while (!processed);
}

CommandProcessor::CommandProcessor(int argc, char** argv)
{
	this->argc = argc;
	this->argv = argv;
	this->speechProcessor = NULL;
}

CommandProcessor::~CommandProcessor() {
	if (this->speechProcessor != NULL)
	{
		delete this->speechProcessor;
	}
}

void CommandProcessor::readSpeechData()
{
	// Skip this step if the argument starts with a '-' (it is an option).
	if (argv[argc - 1][0] == '-') return;

	WavData* wavData = WavData::readFromFile(argv[argc - 1]);
	if (wavData == NULL) {
		exit(1);
	}

	speechProcessor = new Processor(wavData);
}

void CommandProcessor::checkSpeechData()
{
	if (speechProcessor == NULL || speechProcessor->getWavData() == NULL)
	{
		cout << "Error: No input data specified." << endl;
		cout << "Please take a look on the help info for details:" << endl;
		exit(1);
	}
}

void CommandProcessor::printVersion()
{
	cout << PROJECT_NAME << " v" << PROJECT_VERSION << endl;
}

void CommandProcessor::printHelp()
{
	cout << helpInfo << endl;
}

void CommandProcessor::displayMfcc()
{
  speechProcessor->divideIntoFrames();
	speechProcessor->printFramesMfcc();
}

} /* namespace command */
