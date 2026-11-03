/*****************************************************************************************
    ___       ________          ___   ___  ___   ___  ________  ___  ___  ________
   |\  \     |\   __  \        |\  \ |\  \|\  \ |\  \|\   __  \|\  \|\  \|\_____  \
   \ \  \    \ \  \|\  \       \ \  \\_\  \ \  \\_\  \ \  \|\  \ \  \\\  \\|___/  /|
    \ \  \    \ \   __  \       \ \______  \ \______  \ \  \\\  \ \   __  \   /  / /
     \ \  \____\ \  \ \  \       \|_____|\  \|_____|\  \ \  \\\  \ \  \ \  \ /  /_/__
      \ \_______\ \__\ \__\             \ \__\     \ \__\ \_______\ \__\ \__\\________\
       \|_______|\|__|\|__|              \|__|      \|__|\|_______|\|__|\|__|\|_______|
******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void write_wav_header(int fd, uint32_t sample_rate, uint32_t data_size) 
{
    uint32_t file_size = data_size + 36; /* 44 - 8 */
    uint32_t fmt_size = 16;
    uint16_t audio_format = 1; /* PCM */
    uint16_t num_channels = 1; /* mono */
    uint32_t byte_rate = sample_rate * num_channels * 8 / 8;
    uint16_t block_align = num_channels * 8 / 8;
    uint16_t bits_per_sample = 8;
    
    /* Écriture de l'en-tête RIFF */
    write(fd, "RIFF", 4);
    write(fd, &file_size, 4);
    write(fd, "WAVE", 4);
    
    /* Écriture du chunk fmt */
    write(fd, "fmt ", 4);
    write(fd, &fmt_size, 4);
    write(fd, &audio_format, 2);
    write(fd, &num_channels, 2);
    write(fd, &sample_rate, 4);
    write(fd, &byte_rate, 4);
    write(fd, &block_align, 2);
    write(fd, &bits_per_sample, 2);
    
    /* Écriture du chunk data */
    write(fd, "data", 4);
    write(fd, &data_size, 4);
    return;
}

int main(int argc, char **argv) 
{
    const char *filename = "la440.wav";
    const uint32_t sample_rate = 8000;
    const uint32_t duration = 1;
    const double frequency = 440.0;
    const double amplitude = 127.0;
    const double offset = 128.0;
    
    const uint32_t num_samples = sample_rate * duration;
    const uint32_t data_size = num_samples;

    uint32_t i;
    double time;
    double sample;
    uint8_t audio_sample;
        
    /* Ouverture du fichier avec open() */
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Erreur: impossible de créer le fichier %s\n", filename);
        return EXIT_FAILURE;
    }
    
    /* Écriture de l'en-tête manuellement */
    write_wav_header(fd, sample_rate, data_size);
    
    /* Génération des données audio */
    for ( i = 0 ; i < num_samples; i++) 
    {
        time = (double)i / sample_rate;
        sample = sin(2.0 * M_PI * frequency * time);
        audio_sample = (uint8_t)(offset + amplitude * sample);
        write(fd, &audio_sample, 1);
    }
    
    /* Fermeture du fichier */
    close(fd);
    
    printf("Fichier %s créé avec succès!\n", filename);
    printf("Format: 8 bits mono, %d Hz, 1 seconde\n", sample_rate);
    printf("Fréquence: %.1f Hz (La4)\n", frequency);
    
    return EXIT_SUCCESS;
}
