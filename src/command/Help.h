#ifndef HELP_H
#define HELP_H

namespace command {

#define EOL "\n"

/**
 * Help info
 */
static const char* const helpInfo =	EOL
	"Usage: ./speech_recognizer [options] \033[4mFILE\033[0m" EOL EOL
	"Recognizes speech from a WAV \033[4mFILE\033[0m or performs other actions based on options." EOL

	EOL

	"\033[4mFILE\033[0m                    Absolute or relative path to the wav-file for processing" EOL

	EOL

	"Options:" EOL
	"-v, --version           Display version information" EOL
	"-h, --help              Display help information" EOL

	EOL

    "--update-phonemes=<directory>                  Update phoneme models from the \033[4mdirectory\033[0m" EOL
	"--update-phoneme-pairs=<directory>             Update phoneme pairs models from the \033[4mdirectory\033[0m" EOL
	"-l <directory>, --lsa-train=<directory>        Update the correlation between pairs of phonemes from texts from the \033[4mdirectory\033[0m" EOL
	"-t <directory>, --term-frequency=<directory>   Calculate the term frequency of phoneme pairs for texts from the \033[4mdirectory\033[0m" EOL
	
	EOL

	"-f <phoneme>, --phoneme-features=<phoneme>     Display features of the \033[4mphoneme\033[0m or \033[4mphoneme pair\033[0m" EOL
	
	EOL

	"-m, --mfcc                   Display MFCCs retrieved from the \033[4mFILE\033[0m" EOL
    "--predict-phonemes           Predict phonemes on frames from the \033[4mFILE\033[0m" EOL
	"--predict-phoneme-pairs      Predict phoneme pairs on frames from the \033[4mFILE\033[0m" EOL

	EOL

    "-r, --recognize              Recognize speech from the \033[4mFILE\033[0m" EOL	

	;

} /* namespace command */

#endif /* HELP_H */
