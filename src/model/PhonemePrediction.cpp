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
		if (i < 8) { // TODO
			stream << setw(-2) << (*node)->key << ": " << setw(PhonemePrediction::DISTANCE_PRECISION + 3) << (*node)->value;
		}

		if (i < 7) { // node != prev(nodes.end(), 1)
			stream << ", ";
		}
		i++;
	}

	return stream;
}

} /* namespace model */
