/**
 * Configuration
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * This macro is defined when compiling
 * C++ source files with the option -std=c++0x or -std=gnu++0x.
 *  */
#define __GXX_EXPERIMENTAL_CXX0X__ 1

#include <stdio.h>
#include <iostream>
#include <sstream>

/**
 * Build information
 */
#define PROJECT_NAME "speech_recognizer"
#define PROJECT_VERSION "1.0"

/**
 * Raw audio data type
 */
typedef int16_t raw_t;

/**
 * Length of frame (ms)
 */
const uint32_t FRAME_LENGTH = 50;

/**
 * Percentage of overlap for frames (0 <= x < 1)
 */
const double FRAME_OVERLAP = 0.5;

/**
 * Amount of MFCC coefficients
 */
const unsigned short MFCC_SIZE = 12;

/**
 * Frequency bounds
 */
const short MFCC_FREQ_MIN = 300;
const short MFCC_FREQ_MAX = 4000;

/**
 * Entropy parameters
 */
const short ENTROPY_BINS = 75;
const double ENTROPY_THRESHOLD = 0.1;

// Helpers

#define UNUSED(expr) do { (void)(expr); } while (0)

#endif /* CONFIG_H */
