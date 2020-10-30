#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
static const size_t kWavHeaderSize = 44;

enum WavFormat {
  kWavFormatPcm = 1,   // PCM, each sample of size bytes_per_sample
  kWavFormatALaw = 6,  // 8-bit ITU-T G.711 A-law
  kWavFormatMuLaw = 7, // 8-bit ITU-T G.711 mu-law
};

// Return true if the given parameters will make a well-formed WAV header.
bool CheckWavParameters(size_t num_channels, int sample_rate, WavFormat format,
                        size_t bytes_per_sample, size_t num_samples);

// Write a kWavHeaderSize bytes long WAV header to buf. The payload that
// follows the header is supposed to have the specified number of interleaved
// channels and contain the specified total number of samples of the specified
// type. CHECKs the input parameters for validity.
void WriteWavHeader(uint8_t *buf, size_t num_channels, int sample_rate,
                    WavFormat format, size_t bytes_per_sample,
                    size_t num_samples);