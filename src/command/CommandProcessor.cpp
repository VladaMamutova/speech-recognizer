#include <CommandProcessor.h>
#include <getopt.h>
#include <iostream>

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
				printVersion();
				printHelp();
				break;

			case 'm':
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

} /* namespace command */
