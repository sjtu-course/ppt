#include "wav_head_writer.h"
#include <limits>
#include <string>

struct ChunkHeader {
  uint32_t ID;
  uint32_t Size;
};

struct RiffHeader {
  ChunkHeader header;
  uint32_t Format;
};

struct FmtSubchunk {
  ChunkHeader header;
  uint16_t AudioFormat;
  uint16_t NumChannels;
  uint32_t SampleRate;
  uint32_t ByteRate;
  uint16_t BlockAlign;
  uint16_t BitsPerSample;
};

struct WavHeader {
  WavHeader(const WavHeader &) = default;
  WavHeader() = default;
  WavHeader &operator=(const WavHeader &) = default;
  RiffHeader riff;
  FmtSubchunk fmt;
  struct {
    ChunkHeader header;
  } data;
};

const uint32_t kFmtSubchunkSize = sizeof(FmtSubchunk) - sizeof(ChunkHeader);

static inline void WriteLE16(uint16_t *f, uint16_t x) { *f = x; }
static inline void WriteLE32(uint32_t *f, uint32_t x) { *f = x; }
static inline void WriteFourCC(uint32_t *f, char a, char b, char c, char d) {
  *f = static_cast<uint32_t>(a) | static_cast<uint32_t>(b) << 8 |
       static_cast<uint32_t>(c) << 16 | static_cast<uint32_t>(d) << 24;
}

static inline uint16_t ReadLE16(uint16_t x) { return x; }
static inline uint32_t ReadLE32(uint32_t x) { return x; }
static inline std::string ReadFourCC(uint32_t x) {
  return std::string(reinterpret_cast<char *>(&x), 4);
}

static inline uint32_t RiffChunkSize(size_t bytes_in_payload) {
  return static_cast<uint32_t>(bytes_in_payload + kWavHeaderSize -
                               sizeof(ChunkHeader));
}

static inline uint32_t ByteRate(size_t num_channels, int sample_rate,
                                size_t bytes_per_sample) {
  return static_cast<uint32_t>(num_channels * sample_rate * bytes_per_sample);
}

static inline uint16_t BlockAlign(size_t num_channels,
                                  size_t bytes_per_sample) {
  return static_cast<uint16_t>(num_channels * bytes_per_sample);
}

bool CheckWavParameters(size_t num_channels, int sample_rate, WavFormat format,
                        size_t bytes_per_sample, size_t num_samples) {
    // num_channels, sample_rate, and bytes_per_sample must be positive, must fit
    // in their respective fields, and their product must fit in the 32-bit
    // ByteRate field.
    if (num_channels == 0 || sample_rate <= 0 || bytes_per_sample == 0)
        return false;
    if (static_cast<uint64_t>(sample_rate) > std::numeric_limits<uint32_t>::max())
        return false;
    if (num_channels > std::numeric_limits<uint16_t>::max())
        return false;
    if (static_cast<uint64_t>(bytes_per_sample) * 8 >
        std::numeric_limits<uint16_t>::max())
        return false;
    if (static_cast<uint64_t>(sample_rate) * num_channels * bytes_per_sample >
        std::numeric_limits<uint32_t>::max())
        return false;

    // format and bytes_per_sample must agree.
    switch (format) {
    case kWavFormatPcm:
        // Other values may be OK, but for now we're conservative:
        if (bytes_per_sample != 1 && bytes_per_sample != 2)
        return false;
        break;
    case kWavFormatALaw:
    case kWavFormatMuLaw:
        if (bytes_per_sample != 1)
        return false;
        break;
    default:
        return false;
    }

      // The number of bytes in the file, not counting the first ChunkHeader, must
    // be less than 2^32; otherwise, the ChunkSize field overflows.
    const size_t header_size = kWavHeaderSize - sizeof(ChunkHeader);
    const size_t max_samples =
        (std::numeric_limits<uint32_t>::max() - header_size) / bytes_per_sample;
    if (num_samples > max_samples)
      return false;

    // Each channel must have the same number of samples.
    if (num_samples % num_channels != 0)
      return false;

    return true;
}

void WriteWavHeader(uint8_t *buf, size_t num_channels, int sample_rate,
                    WavFormat format, size_t bytes_per_sample,
                    size_t num_samples) {
    if (!CheckWavParameters(num_channels, sample_rate, format, bytes_per_sample, num_samples)) {
        return;
    }

    WavHeader header;
    const size_t bytes_in_payload = bytes_per_sample * num_samples;

    WriteFourCC(&header.riff.header.ID, 'R', 'I', 'F', 'F');
    WriteLE32(&header.riff.header.Size, RiffChunkSize(bytes_in_payload));
    WriteFourCC(&header.riff.Format, 'W', 'A', 'V', 'E');

    WriteFourCC(&header.fmt.header.ID, 'f', 'm', 't', ' ');
    WriteLE32(&header.fmt.header.Size, kFmtSubchunkSize);
    WriteLE16(&header.fmt.AudioFormat, format);
    WriteLE16(&header.fmt.NumChannels, static_cast<uint16_t>(num_channels));
    WriteLE32(&header.fmt.SampleRate, sample_rate);
    WriteLE32(&header.fmt.ByteRate,
                ByteRate(num_channels, sample_rate, bytes_per_sample));
    WriteLE16(&header.fmt.BlockAlign, BlockAlign(num_channels, bytes_per_sample));
    WriteLE16(&header.fmt.BitsPerSample,
                static_cast<uint16_t>(8 * bytes_per_sample));

    WriteFourCC(&header.data.header.ID, 'd', 'a', 't', 'a');
    WriteLE32(&header.data.header.Size, static_cast<uint32_t>(bytes_in_payload));

    // Do an extra copy rather than writing everything to buf directly, since buf
    // might not be correctly aligned.
    memcpy(buf, &header, kWavHeaderSize);
  }