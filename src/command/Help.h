#ifndef HELP_H
#define HELP_H

namespace command {

#define EOL "\n"

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

	"-m, --mfcc              Display MFCCs retrieved from the input file" EOL
	"-f, --phoneme-features	 Display phoneme features" EOL

	EOL

	"-p, --predict-labels	 Predict phonemes on frames from the input file" EOL
	"-t, --term-frequency    Calculate the term frequency (frequency of phoneme pairs) for all texts from the input folder" EOL
	
	EOL

	"-l, --lsa-train         Calculate the correlation between pairs of phonemes of all texts from the input folder" EOL
	;

} /* namespace command */

#endif /* HELP_H */
