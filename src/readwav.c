#include "wav.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

char *array_part(char* a, int start, int end) {
    const int length = end - start + 1;
    char *result = malloc(length);

    for (int i = start; i <= end; i++) {
        result[i - start] = a[i];
    }

    return result;
}

// int parsing help from https://www.reddit.com/r/C_Programming/comments/xv2qgr/how_to_extract_a_32_bit_integer_from_a_certain/
uint32_t int32_array_part(char* a, int end) {
    uint32_t result = 0;
    result = ((a[end] << 24) |
                (a[end - 1] << 16) |
                (a[end - 2] << 8) | 
                (a[end - 3]));
    return result;
}

uint16_t int16_array_part(char* a, int end) {
    uint16_t result = 0;
    result = ((a[end] << 8) |
                (a[end - 1]));
    return result;
}

struct wav read_wav(char* input_path){
    struct wav_header wavh;
    FILE *fr = fopen(input_path, "r");

    // Declare buffers, get file size
    char header_buffer[44];
    fgets(header_buffer, 44, fr);

    // declare other header variables
    char *riff = array_part(header_buffer, 0, 3);
    char *wave = array_part(header_buffer, 8, 11);
    char *fmt = array_part(header_buffer, 12, 15);
    char *data = array_part(header_buffer, 36, 39);

    strncpy(wavh.riff, riff, 4);
    strncpy(wavh.wave, wave, 4);
    strncpy(wavh.fmt, fmt, 4);
    strncpy(wavh.data, data, 4);

    free(riff);
    free(wave);
    free(fmt);
    free(data);

    wavh.file_size = int32_array_part(header_buffer, 7);
    wavh.chunk_size = int32_array_part(header_buffer, 19);
    wavh.format_type = int16_array_part(header_buffer, 21);
    wavh.num_channels = int16_array_part(header_buffer, 23);
    wavh.sample_rate = int32_array_part(header_buffer, 27);
    wavh.bits_per_sample = int16_array_part(header_buffer, 35);
    wavh.data_size = int32_array_part(header_buffer, 43);

    wavh.bytes_per_second = wavh.sample_rate * wavh.bits_per_sample / 8 * wavh.num_channels;
    wavh.bytes_per_sample = wavh.bits_per_sample / 8 * wavh.num_channels;

    const int header_length = sizeof(struct wav_header);

    char *file_buffer = malloc(wavh.file_size);
    fgets(file_buffer, wavh.file_size, fr);
    
    const int BUFFER_SIZE = wavh.data_size / wavh.bytes_per_sample;

    short *buffer = malloc(BUFFER_SIZE * sizeof(short) + header_length - 1);

    for (int i = 1; i < wavh.file_size; i+=wavh.bytes_per_sample) {
        short output = 0;

        byte top = file_buffer[i + 1];
        byte bottom = file_buffer[i];

        output = ((top << 8) | (bottom));

        buffer[(i - 1)/wavh.bytes_per_sample] = output;
    }
    
    free(file_buffer);
    fclose(fr);

    struct wav wav;

    wav.wavh = wavh;
    wav.header_length = header_length;

    wav.buffer = buffer;
    wav.buffer_size = BUFFER_SIZE;

    return wav;
}

int main(void) {
    struct wav wav = read_wav("test.wav");

    FILE *fw = fopen("after.wav", "w");
    fwrite(&wav.wavh, 1, wav.header_length, fw);
    fwrite(wav.buffer, 2, wav.buffer_size, fw);
    free(wav.buffer);
}