#include <stdint.h>

struct wav_header{
    char riff[4];
    int32_t file_size;
    char wave[4];
    char fmt[4];
    int32_t chunk_size;
    int16_t format_type;
    int16_t num_channels;
    int32_t sample_rate;
    int32_t bytes_per_second;
    int16_t bytes_per_sample;
    int16_t bits_per_sample;
    char data[4];
    int32_t data_size;
};