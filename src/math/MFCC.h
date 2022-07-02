#ifndef MFCC_H
#define MFCC_H

#include <stdint.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <complex>
#include <valarray>
#include "../config.h"

using namespace std;

namespace math {

/**
 * Logic for transformation raw audio data into mel-frequency "cepstrum"
 *
 * @see http://en.wikipedia.org/wiki/Mel-frequency_cepstrum
 * @see http://www.pvsm.ru/programmirovanie/4344
 * @see https://habr.com/ru/post/140828/
 */
class MFCC {
public:

	/**
	 * Perform MFCC transformation
	 */
	static double* transform(const double* source, uint32_t start, uint32_t finish,
			uint8_t mfccSize, uint32_t frequency, uint32_t freqMin, uint32_t freqMax);

	/**
	 * Preemphasis digital filtration
	 */
	static double* filter(const double* source, uint32_t start, uint32_t finish);

	/**
	 * Compute singnal's magnitude (short-time Fourier transform with Hamming window)
	 *
	 * @see http://en.wikipedia.org/wiki/Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Discrete-time_Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Short-time_Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Window_function#Generalized_Hamming_windows
	 *
	 * @see http://www.robots.ox.ac.uk/~sjrob/Teaching/SP/l6.pdf
	 * @see http://www.robots.ox.ac.uk/~sjrob/Teaching/SP/l7.pdf
	 */
	static double* fourierTransform(const double* source, uint32_t length, bool useWindow);

	/**
	 * Compute singnal's magnitude (short-time Fourier transform with Hamming window)
	 *
	 * @see http://www.drdobbs.com/cpp/a-simple-and-efficient-fft-implementatio/199500857
	 * @see http://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B
	 */
	static double* fourierTransformFast(const double* source, uint32_t length, bool useWindow);

	/**
	 * Create mel filters (for range of frequencies), using triangular overlapping windows
	 *
	 * @see http://en.wikipedia.org/wiki/Mel_scale
	 * @see http://en.wikipedia.org/wiki/Window_function#Triangular_window
	 * @see http://practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/
	 * @see http://www.pvsm.ru/programmirovanie/4344
	 */
	static double** getMelFilters(uint8_t mfccSize, uint32_t filterLength, uint32_t frequency,
			uint32_t freqMin, uint32_t freqMax);

	/**
	 * Take the logs of the powers at each of the mel frequencies
	 */
	static double* calcPower(const double* fourierRaw, uint32_t fourierLength,
			double** melFilters, uint8_t mfccSize);

	/**
	 * Take the discrete cosine transform of the list of mel log powers
	 *
	 * @see http://en.wikipedia.org/wiki/Discrete_cosine_transform#DCT-II
	 */
	static double* dctTransform(const double* data, uint32_t length);


	// Mel convertors
	static double convertToMel(double f) { return 1125. * log(1. + f/700.); }
	static double convertFromMel(double m) { return 700. * (exp(m/1125.) - 1); }


private:
	static void fourierTransformFastRecursion(valarray<complex<double>>& data);
};

} /* namespace math */

#endif /* MFCC_H */
