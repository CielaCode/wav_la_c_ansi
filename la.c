#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    // RIFF Header
    char riff[4];           // "RIFF"
    uint32_t file_size;     // Taille totale - 8
    char wave[4];           // "WAVE"
    
    // Format chunk
    char fmt[4];            // "fmt "
    uint32_t fmt_size;      // 16 pour PCM
    uint16_t audio_format;  // 1 pour PCM
    uint16_t num_channels;  // 1 pour mono
    uint32_t sample_rate;   // 8000 Hz
    uint32_t byte_rate;     // sample_rate * num_channels * bits_per_sample/8
    uint16_t block_align;   // num_channels * bits_per_sample/8
    uint16_t bits_per_sample; // 8
    
    // Data chunk
    char data[4];           // "data"
    uint32_t data_size;     // Nombre d'octets de données
} WavHeader;
#pragma pack(pop)

int main() {
    const char *filename = "la440.wav";
    const uint32_t sample_rate = 8000;  // Fréquence d'échantillonnage
    const uint32_t duration = 1;        // Durée en secondes
    const double frequency = 440.0;     // Fréquence du La en Hz
    const double amplitude = 127.0;     // Amplitude (0-127 pour 8 bits non signé)
    const double offset = 128.0;        // Offset pour centrer sur 128 (silence en 8 bits)
    
    // Calcul du nombre d'échantillons
    const uint32_t num_samples = sample_rate * duration;
    const uint32_t data_size = num_samples;
    
    // Création et remplissage de l'en-tête WAV
    WavHeader header;
    
    // RIFF header
    memcpy(header.riff, "RIFF", 4);
    header.file_size = data_size + sizeof(WavHeader) - 8;
    memcpy(header.wave, "WAVE", 4);
    
    // Format chunk
    memcpy(header.fmt, "fmt ", 4);
    header.fmt_size = 16;
    header.audio_format = 1; // PCM
    header.num_channels = 1; // Mono
    header.sample_rate = sample_rate;
    header.bits_per_sample = 8;
    header.byte_rate = sample_rate * header.num_channels * header.bits_per_sample / 8;
    header.block_align = header.num_channels * header.bits_per_sample / 8;
    
    // Data chunk
    memcpy(header.data, "data", 4);
    header.data_size = data_size;
    
    // Ouverture du fichier en écriture binaire
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur: impossible de créer le fichier %s\n", filename);
        return 1;
    }
    
    // Écriture de l'en-tête
    fwrite(&header, sizeof(WavHeader), 1, file);
    
    // Génération des données audio (sinusoïde 440 Hz)
    for (uint32_t i = 0; i < num_samples; i++) {
        double time = (double)i / sample_rate;
        double sample = sin(2.0 * M_PI * frequency * time);
        
        // Conversion en 8 bits non signé (0-255) avec offset à 128
        uint8_t audio_sample = (uint8_t)(offset + amplitude * sample);
        fwrite(&audio_sample, 1, 1, file);
    }
    
    // Fermeture du fichier
    fclose(file);
    
    printf("Fichier %s créé avec succès!\n", filename);
    printf("Format: 8 bits mono, %d Hz, 1 seconde\n", sample_rate);
    printf("Frquence: %.1f Hz (La4)\n", frequency);
    
    return 0;
}
