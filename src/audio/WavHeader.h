/**
 * Represents WAV file header.
 *
 * Supports only PCM format.
 * 
 * @see http://en.wikipedia.org/wiki/WAV
 * @see http://en.wikipedia.org/wiki/Linear_pulse-code_modulation
 * @see https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 * @see https://audiocoding.ru/articles/2008-05-22-wav-file-structure/
 * @see https://radioprog.ru/post/1025
 * @see http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
 */

#ifndef WAV_HEADER_H
#define WAV_HEADER_H

#include <cstdint>
#include <iostream>
#include <string>
#include "../config.h"

using namespace std;

namespace audio {

/**
 * WAV header
 */
struct WavHeader {
    char              riff[4];        // RIFF Header
    int               chunkSize;      // RIFF Chunk Size
    char              wave[4];        // WAVE Header
    char              fmt[4];         // FMT header
    unsigned int      subchunk1Size;  // Size of the fmt chunk
    unsigned short    audioFormat;    // Audio format 1=PCM (Other formats are unsupported)
    unsigned short    numOfChan;      // Number of channels 1=Mono, 2=Stereo
    unsigned int	  samplesPerSec;  // Sampling Frequency in Hz
    unsigned int      bytesPerSec;    // bytes per second
    unsigned short    blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short    bitsPerSample;  // Number of bits per sample

    // The data below depends on audioFormat, but we work only with PCM cases
    char              data[4];        // DATA header
    unsigned int      subchunk2Size;  // Sampled data length

    bool isValid();
};

} // namespace audio

#endif /* WAV_HEADER_H */
