#include <iostream>
#include <vector>
#include "CommandProcessor.h"
#include "Help.h"
#include "../model/LsaTrainer.h"
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>

namespace command {

/**
 * Commands map
 */
static struct option longOptions[] = {
	// { const char *name, int has_arg, int *flag, int val }
	// если flag = NULL, то getopt_long для длинной опции возвращает val
	{ "version", no_argument, NULL, 'v' },
	{ "help", no_argument, NULL, 'h' },

    { "update-phonemes", required_argument, NULL, '1' },
	{ "update-phoneme-pairs", required_argument, NULL, '2' },
	{ "lsa-train", required_argument, NULL, 'l' },
	{ "term-frequency", required_argument, NULL, 't' },

	{ "phoneme-features", required_argument, NULL, 'f' },

	{ "mfcc", no_argument, NULL, 'm' },
	{ "predict-phonemes", no_argument, NULL, 'p' },

	{ "recognize", no_argument, NULL, 'r' },

	{ NULL, 0, NULL, 0 }
};

static const char* const shortOptions = "vhl:t:f:mpr";

void CommandProcessor::process()
{
	checkInputArguments();

	bool processed = false;
	int option;
	int option_index;

	LsaTrainer *lsaTrainer;
	do
	{
		option = getopt_long(argc, argv, shortOptions, longOptions, &option_index);
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
			case '1':
				storage->updatePhonemes(optarg);
				break;
			case '2':
				storage->updatePhonemePairs(optarg);
				break;
			case 'l':
				lsaTrainer = new LsaTrainer(optarg);
				lsaTrainer->train();
				break;
			case 't':
				printTermFrequency(optarg);
				break;
			case 'f':
				printPhonemeFeatures(optarg);
				break;
			case 'm':
				readSpeechData();
				displayMfcc();
				break;
			case 'p':
				readSpeechData();
				predictPhonemes();
				break;
			case 'r':
				readSpeechData();
				recognize();
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
	this->audioProcessor = NULL;
	this->storage = new Storage();
	this->speechProcessor = new SpeechProcessor(storage);
}

CommandProcessor::~CommandProcessor()
{
	if (this->audioProcessor != NULL) {
		delete this->audioProcessor;
	}

	delete this->storage;
}

void CommandProcessor::checkInputArguments()
{
	if (argc <= 1) {
		printVersion();
		printHelp();
		exit(EXIT_FAILURE);
	}
}

void CommandProcessor::readSpeechData()
{
	// Skip this step if the last or prelast argument starts with a '-' (it is an option or argument).
	if (argv[argc - 1][0] == '-') {
		cout << "Error: No input data specified." << endl;
		cout << "Please use -h (--help) for details." << endl;
		exit(EXIT_FAILURE);
	}

	WavData* wavData = WavData::readFromFile(argv[argc - 1]);
	if (wavData == NULL) {
		exit(EXIT_FAILURE);
	}

	audioProcessor = new AudioProcessor(wavData);
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
	cout << *audioProcessor->fetchFrameMfccs();
}

void CommandProcessor::printTermFrequency(const char* directory)
{
	TermFrequency* termFrequency = TermFrequency::createFromDirectory(directory);
	cout << *termFrequency;
}

void CommandProcessor::printPhonemeFeatures(const char* phonemeLabel)
{
	const map<string, Phoneme*>* phonemes = storage->fetchPhonemeMap()->getPhonemes();

	if (phonemes->count(phonemeLabel) == 0) {
		phonemes = storage->fetchPhonemePairMap()->getPhonemes();
		if (phonemes->count(phonemeLabel) == 0) {
			cout << "Phoneme with label \"" << phonemeLabel << "\" not found." << endl;
			return;
		}
	}

	Phoneme* phoneme = phonemes->at(phonemeLabel);
	cout << "\nPhoneme \"" << phonemeLabel << "\" is based on " << phoneme->getFeatureVectorSize() << " feature vectors: " << endl;
	cout << *phoneme->getFeatureVector();
}

void CommandProcessor::predictPhonemes()
{
	speechProcessor->predictPhonemesByFeatures(audioProcessor->fetchFrameMfccs());
}

void CommandProcessor::recognize()
{
	speechProcessor->predictPhonemesByFeatures(audioProcessor->fetchFrameMfccs());
	cout << endl;
	//speechProcessor->predictPhonemePairsByFeatures(mfccFeatures);

	cout << endl;
	speechProcessor->recognize(audioProcessor);
}

} /* namespace command */
