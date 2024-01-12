#include <stdint.h>

struct wav_header{
    char riff[4];
    uint32_t file_size;
    char wave[4];
    char fmt[4];
    uint32_t chunk_size;
    uint16_t format_type;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t bytes_per_second;
    uint16_t bytes_per_sample;
    uint16_t bits_per_sample;
    char data[4];
    uint32_t data_size;
};

struct wav{
    struct wav_header wavh;
    int header_length;

    short *buffer;
    int buffer_size;
};