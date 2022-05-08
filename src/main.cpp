#include <iostream>
#include <stdlib.h>
#include "command/CommandProcessor.h"

using namespace std;
using namespace command;

/**
 * Entry point
 */
int main(int argc, char** argv) {

	CommandProcessor processor(argc, argv);
	processor.process();

	return EXIT_SUCCESS;
}
