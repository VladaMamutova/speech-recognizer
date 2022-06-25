#include <math.h>
#include <iomanip>
#include "PhonemePrediction.h"

namespace model {

const streamsize PhonemePrediction::DISTANCE_PRECISION = 3;
const string PhonemePrediction::PREDICTION_DELIMETER = ", ";

PhonemePrediction::PhonemePrediction()
{
	this->predictions = new Tree<string, double>();
}

PhonemePrediction::~PhonemePrediction()
{
	delete predictions;
}

void PhonemePrediction::addPrediction(string phonemeLabel, double distance)
{
	predictions->add(phonemeLabel, distance);
}

ostream& operator<<(ostream& stream, const PhonemePrediction& phonemePrediction)
{
	stream.precision(PhonemePrediction::DISTANCE_PRECISION);
	stream.setf(std::ios_base::fixed, std::ios_base::floatfield);

	vector<Tree<string, double>::Node*> nodes;
	phonemePrediction.predictions->leftRightTraversal(phonemePrediction.predictions->getTree(), nodes);

	vector<Tree<string, double>::Node*>::const_iterator node;
	int i = 0;
	for (node = nodes.begin(); node != nodes.end(); node++) {
		if (i < 8) {
			stream << left << setfill(' ') << setw(5) << (*node)->key << ": " << right << setw(PhonemePrediction::DISTANCE_PRECISION + 4) << (*node)->value;
		}

		if (i < 7) {
			stream << ", ";
		}
		i++;
	}

	stream << left;
	return stream;
}

} /* namespace model */
