#include <iostream>
#include <vector>
#include "CommandProcessor.h"
#include "Help.h"

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

void CommandProcessor::process()
{
	checkInputArguments();
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

CommandProcessor::~CommandProcessor()
{
	if (this->speechProcessor != NULL)
	{
		delete this->speechProcessor;
	}
}

void CommandProcessor::checkInputArguments()
{
	if (argc <= 1) {
		cout << "Error: No input parameters specified." << endl;
		cout << "Please take a look on the help info for details:" << endl;
		printHelp();
		exit(EXIT_FAILURE);
	}
}

void CommandProcessor::readSpeechData()
{
	// Skip this step if the argument starts with a '-' (it is an option).
	if (argv[argc - 1][0] == '-') return;

	WavData* wavData = WavData::readFromFile(argv[argc - 1]);
	if (wavData == NULL) {
		exit(EXIT_FAILURE);
	}

	speechProcessor = new SpeechProcessor(wavData);
}

void CommandProcessor::checkSpeechData()
{
	if (speechProcessor == NULL || speechProcessor->getWavData() == NULL)
	{
		cout << "Error: No input data specified." << endl;
		cout << "Please take a look on the help info for details:" << endl;
		exit(EXIT_FAILURE);
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
