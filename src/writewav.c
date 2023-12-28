#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "wav.h"
// main.c
// for now

struct wav_header wavh;

enum {
  SAMPLE_RATE = 8000,
  DURATION_SECONDS = 10,
  BUFFER_SIZE = SAMPLE_RATE * DURATION_SECONDS
};

// const int duration_seconds = 10;
const float MIDDLE_C = 256.00;

short int buffer[BUFFER_SIZE] = {};
const int header_length = sizeof(struct wav_header);

// adapted from https://devtails.xyz/@adam/how-to-write-a-wav-file-in-c
int main(void)
{
  strncpy(wavh.riff, "RIFF", 4);
  strncpy(wavh.wave, "WAVE", 4);
  strncpy(wavh.fmt, "fmt ", 4);
  strncpy(wavh.data, "data", 4);

  wavh.chunk_size = 16;
  wavh.format_type = 1;
  wavh.num_channels = 1;
  wavh.sample_rate = SAMPLE_RATE;
  wavh.bits_per_sample = 16;
  wavh.bytes_per_second = wavh.sample_rate * wavh.bits_per_sample / 8 * wavh.num_channels;
  wavh.bytes_per_sample = wavh.bits_per_sample / 8 * wavh.num_channels;
  
  // read results:
  // riff:RIFF,q☻
  // file_size:160044 //
  // wave:WAVEfmt ►
  // fmt:fmt ►
  // chunk_size:16 //
  // format_type:1 //
  // num_chans:1 //
  // srate:8000 //
  // bytes/sec:16000 //
  // bytes/sam:2 //
  // bits/sam:16 //
  // data:data //
  // data_size:160000 //

  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = (short int)((cos((2 * M_PI * MIDDLE_C * i) / SAMPLE_RATE) * 1000));
  }

  wavh.data_size = BUFFER_SIZE * wavh.bytes_per_sample;
  wavh.file_size = wavh.data_size + header_length;

  // for (int i = 0; i < BUFFER_SIZE; i++) {
  //       printf("%d\n", buffer[i]);
  // }

  FILE *fp = fopen("test.wav", "w");
  fwrite(&wavh, 1, header_length, fp);
  fwrite(buffer, 2, BUFFER_SIZE, fp);
}