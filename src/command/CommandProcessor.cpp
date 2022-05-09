#include <getopt.h>
#include <iostream>
#include <vector>
#include "CommandProcessor.h"
#include "MfccEntry.h"
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
		cout << "No input parameters specified. " << endl;
		cout << "Please take a look on the help info for details: " << endl;
		printHelp();
		return;
	}

	if (argv[argc - 1][0] != '-' && !readData(*this->context, argv[argc - 1])) {
		cout << "Error: Failed to load input data from the file \"" << argv[argc - 1] << "\"." << endl;
		return;
	}

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
			  displayMfcc(*this->context);
				break;
			default:
				cout << "Please, use -h (--help) for details." << endl;
				processed = true;
				break;
		}
	} while (!processed);
}

void CommandProcessor::printVersion()
{
	cout << PROJECT_NAME << " v" << PROJECT_VERSION << endl;
}

void CommandProcessor::printHelp()
{
	cout << helpInfo << endl;
}

bool CommandProcessor::readData(Context& context, const char* inputFile)
{
	if (inputFile == NULL) {
		cout << "Input file is not specified." << endl;
		return false;
	}

	WavData* wavData = WavData::readFromFile(inputFile);

	if (wavData != NULL) {
		context.setWavData(wavData);
	}

	return wavData != NULL;
}

void CommandProcessor::displayMfcc(Context& context)
{
  // Check pre-requirements
	if (context.getWavData() == NULL) {
		cerr << "Input data is not specified :(" << endl;
		return;
	}

  cout << "Calculating MFCC for input data... " << endl;

	Processor* processor = new Processor(context.getWavData());
	processor->init();

	const vector<Frame*>* frames = processor->getFrames();
	vector<MfccEntry*>* mfcc = new vector<MfccEntry*>();

	vector<Frame*>::const_iterator frame;
	for (frame = frames->begin(); frame != frames->end(); ++frame) {
		processor->initMfcc(*frame);

		MfccEntry* entry = new MfccEntry((*frame)->getMFCC());
		mfcc->push_back(entry);
	}

  // Print MFCC
	cout << endl << "MFCC coefficients: " << endl;
	for(vector<MfccEntry*>::const_iterator iter = mfcc->begin();
			iter != mfcc->end(); iter++) {
		(*iter)->print();
	}
	cout << endl;

	delete mfcc;
}

} /* namespace command */
