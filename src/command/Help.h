#ifndef HELP_H
#define HELP_H

#include <getopt.h>

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

	"-m, --mfcc       Display the mfcc coefficients retrieved from the input data"

	EOL;

} /* namespace command */

#endif /* HELP_H */
