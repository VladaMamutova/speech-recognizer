#ifndef MFCC_ENTRY_H
#define MFCC_ENTRY_H

#include <fstream>

using namespace std;

namespace math {

class MfccEntry {

public:
	MfccEntry();
	MfccEntry(double* data);
	~MfccEntry();

	inline size_t getSize() const { return this->size; }
	inline double* getData() const { return this->data; }

	friend std::ostream& operator<<(std::ostream& fs, const MfccEntry& obj);
	friend std::istream& operator>>(std::istream& fs, MfccEntry& obj);

	void print();
private:
	size_t	size;
	double*	data;
};

} /* namespace math */

#endif /* MFCC_ENTRY_H */
