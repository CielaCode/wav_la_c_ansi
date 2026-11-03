#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void write_wav_header(FILE *file, uint32_t sample_rate, uint32_t data_size) {
    // Écriture de l'en-tête RIFF
    fwrite("RIFF", 1, 4, file);
    uint32_t file_size = data_size + 36; // 36 = 44 - 8 (car RIFF et WAVE prennent 8 octets)
    fwrite(&file_size, 4, 1, file);
    fwrite("WAVE", 1, 4, file);

    // Écriture du chunk fmt
    fwrite("fmt ", 1, 4, file);
    uint32_t fmt_size = 16;
    fwrite(&fmt_size, 4, 1, file);
    uint16_t audio_format = 1;
    fwrite(&audio_format, 2, 1, file);
    uint16_t num_channels = 1;
    fwrite(&num_channels, 2, 1, file);
    fwrite(&sample_rate, 4, 1, file);
    uint32_t byte_rate = sample_rate * num_channels * 8 / 8;
    fwrite(&byte_rate, 4, 1, file);
    uint16_t block_align = num_channels * 8 / 8;
    fwrite(&block_align, 2, 1, file);
    uint16_t bits_per_sample = 8;
    fwrite(&bits_per_sample, 2, 1, file);

    // Écriture du chunk data
    fwrite("data", 1, 4, file);
    fwrite(&data_size, 4, 1, file);
}

int main() {
    const char *filename = "la440.wav";
    const uint32_t sample_rate = 8000;
    const uint32_t duration = 1;
    const double frequency = 440.0;
    const double amplitude = 127.0;
    const double offset = 128.0;

    const uint32_t num_samples = sample_rate * duration;
    const uint32_t data_size = num_samples;

    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur: impossible de créer le fichier %s\n", filename);
        return 1;
    }

    write_wav_header(file, sample_rate, data_size);

    for (uint32_t i = 0; i < num_samples; i++) {
        double time = (double)i / sample_rate;
        double sample = sin(2.0 * M_PI * frequency * time);
        uint8_t audio_sample = (uint8_t)(offset + amplitude * sample);
        fwrite(&audio_sample, 1, 1, file);
    }

    fclose(file);

    printf("Fichier %s créé avec succès!\n", filename);
    printf("Format: 8 bits mono, %d Hz, 1 seconde\n", sample_rate);
    printf("Fréquence: %.1f Hz (La4)\n", frequency);

    return 0;
}
