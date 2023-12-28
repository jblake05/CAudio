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

struct wav_header wavh;

int main(void){
    FILE *fr = fopen("test.wav", "r");
    const int l = 25;
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

    char *file_buffer = malloc(wavh.file_size);
    fgets(file_buffer, wavh.file_size, fr);
    
    const int BUFFER_SIZE = wavh.data_size;

    // for (int i = 44; i < wavh.file_size; i++) {
    //     printf("%d\n", file_buffer[i]);
    //     if (i > 44 + l)
    //         break;
    // }

    // // isn't copying in.........
    // char *buffer = malloc(BUFFER_SIZE);

    // strcpy(buffer, &file_buffer[44]);

    // free(file_buffer);
    fclose(fr);

    const int header_length = sizeof(struct wav_header);

    // printf("riff:%s\n", wavh.riff);
    // printf("file_size:%d\n", wavh.file_size);
    // printf("wave:%s\n", wavh.wave);
    // printf("fmt:%s\n", wavh.fmt);
    // printf("chunk_size:%d\n", wavh.chunk_size);
    // printf("format_type:%d\n", wavh.format_type);

    // printf("num_chans:%d\n", wavh.num_channels);
    // printf("srate:%d\n", wavh.sample_rate);
    // printf("bytes/sec:%d\n", wavh.bytes_per_second);
    // printf("bytes/sam:%d\n", wavh.bytes_per_sample);
    // printf("bits/sam:%d\n", wavh.bits_per_sample);
    // printf("data:%s\n", wavh.data);
    // printf("data_size:%d\n", wavh.data_size);

    // printf("-------------------------------------\n");

    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     printf("%d\n", buffer[i]);
    //     if (i > l)
    //         break;
    // }
    // printf("riff:%s\nfile_size:%d\nwave:%s\nfmt:%s\nchunk_size:%d\nformat_type:%d\nnum_chans:%d\nsrate:%d\nbytes/sec:%d\nbytes/samp:%d\nbits/samp:%d\ndata:%s\ndata_size:%d",
    // wavh.riff, wavh.file_size, wavh.wave, wavh.fmt,
    // wavh.chunk_size, wavh.format_type, wavh.num_channels,
    // wavh.sample_rate, wavh.bytes_per_second, wavh.bytes_per_sample,
    // wavh.bits_per_sample, wavh.data, wavh.data_size);

    // Sleep(10000);

    // 
    // still has unfreed file_buffer
    FILE *fw = fopen("after.wav", "w");
    fwrite(&wavh, 1, header_length, fw);
    fwrite(&file_buffer[44], 2, BUFFER_SIZE, fw);
}

// int main(void){

//     return 0;
// }