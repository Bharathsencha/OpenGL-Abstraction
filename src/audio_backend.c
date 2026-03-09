// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Bharath
//
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif

#include "../blz.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Internal State

static ma_engine g_engine;
static int g_engine_initialized = 0;

#define PCM_BUFFER_SIZE 2048
static float g_pcm_buffer[PCM_BUFFER_SIZE];
static int g_pcm_cursor = 0;

#define MAX_MUSIC_SLOTS 16
typedef struct {
    ma_sound sound;       // miniaudio sound (used for streaming too)
    ma_decoder decoder;   // decoder for time queries
    int active;
    int decoder_active;
    float total_length;   // cached length in seconds
} MusicSlot;

static MusicSlot g_music[MAX_MUSIC_SLOTS];
static int g_music_count = 0;

#define MAX_SOUND_SLOTS 16
typedef struct {
    ma_sound sound;
    int active;
} SoundSlot;

static SoundSlot g_sounds[MAX_SOUND_SLOTS];
static int g_sound_count = 0;

// PCM Capture for FFT/Visualizer
// Captures samples from the current music stream for the visualizer buffer.

static ma_uint32 g_capture_source = -1;

// Backend Lifecycle

int init_audio(void) {
    if (g_engine_initialized) return 0;

    ma_engine_config config = ma_engine_config_init();
    config.channels = 2;
    config.sampleRate = 48000;

    if (ma_engine_init(&config, &g_engine) != MA_SUCCESS) {
        fprintf(stderr, "[Audio Backend] Failed to initialize audio engine\n");
        return -1;
    }

    g_engine_initialized = 1;
    memset(g_pcm_buffer, 0, sizeof(g_pcm_buffer));
    memset(g_music, 0, sizeof(g_music));
    memset(g_sounds, 0, sizeof(g_sounds));
    g_music_count = 0;
    g_sound_count = 0;
    return 0;
}

void close_audio(void) {
    // Unload all active music
    for (int i = 0; i < g_music_count; i++) {
        if (g_music[i].active) {
            ma_sound_uninit(&g_music[i].sound);
            if (g_music[i].decoder_active) {
                ma_decoder_uninit(&g_music[i].decoder);
            }
            g_music[i].active = 0;
        }
    }
    // Unload all active sounds
    for (int i = 0; i < g_sound_count; i++) {
        if (g_sounds[i].active) {
            ma_sound_uninit(&g_sounds[i].sound);
            g_sounds[i].active = 0;
        }
    }

    if (g_engine_initialized) {
        ma_engine_uninit(&g_engine);
        g_engine_initialized = 0;
    }

    g_music_count = 0;
    g_sound_count = 0;
}

// Music Playback

Music load_music(const char *path) {
    if (!g_engine_initialized || g_music_count >= MAX_MUSIC_SLOTS) return -1;

    int idx = g_music_count;
    MusicSlot *slot = &g_music[idx];

    // Initialize sound with streaming flag
    ma_uint32 flags = MA_SOUND_FLAG_STREAM;
    if (ma_sound_init_from_file(&g_engine, path, flags, NULL, NULL, &slot->sound) != MA_SUCCESS) {
        fprintf(stderr, "[Audio Backend] Failed to load music: %s\n", path);
        return -1;
    }

    // Initialize decoder for time queries and PCM capture
    ma_decoder_config dec_config = ma_decoder_config_init(ma_format_f32, 2, 48000);
    if (ma_decoder_init_file(path, &dec_config, &slot->decoder) == MA_SUCCESS) {
        slot->decoder_active = 1;
        // Get total length
        ma_uint64 total_frames;
        if (ma_decoder_get_length_in_pcm_frames(&slot->decoder, &total_frames) == MA_SUCCESS) {
            slot->total_length = (float)total_frames / 48000.0f;
        } else {
            slot->total_length = 0;
        }
    } else {
        slot->decoder_active = 0;
        slot->total_length = 0;
    }

    slot->active = 1;
    g_music_count++;

    // Set this as the capture source for PCM/FFT
    g_capture_source = idx;

    return idx;
}

void unload_music(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return;
    ma_sound_uninit(&g_music[m].sound);
    if (g_music[m].decoder_active) {
        ma_decoder_uninit(&g_music[m].decoder);
        g_music[m].decoder_active = 0;
    }
    g_music[m].active = 0;
    if (g_capture_source == (ma_uint32)m) g_capture_source = -1;
}

void play_music(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return;
    ma_sound_start(&g_music[m].sound);
}

void stop_music(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return;
    ma_sound_stop(&g_music[m].sound);
    ma_sound_seek_to_pcm_frame(&g_music[m].sound, 0);
}

void pause_music(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return;
    ma_sound_stop(&g_music[m].sound);
}

void resume_music(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return;
    ma_sound_start(&g_music[m].sound);
}

void update_music(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return;

    // Capture PCM samples for FFT visualization
    // We read from the engine's output by reading the sound's cursor position
    // and using the decoder to read PCM data at that position.
    if (g_music[m].decoder_active && g_capture_source == (ma_uint32)m) {
        // Get current cursor in PCM frames
        ma_uint64 cursor_frames;
        if (ma_sound_get_cursor_in_pcm_frames(&g_music[m].sound, &cursor_frames) == MA_SUCCESS) {
            // Seek decoder to current position and read a chunk
            ma_decoder_seek_to_pcm_frame(&g_music[m].decoder, cursor_frames);
            float temp[512]; // 256 frames * 2 channels
            ma_uint64 frames_read;
            if (ma_decoder_read_pcm_frames(&g_music[m].decoder, temp, 256, &frames_read) == MA_SUCCESS) {
                for (ma_uint64 i = 0; i < frames_read; i++) {
                    // Average stereo to mono
                    float mono = (temp[i * 2] + temp[i * 2 + 1]) * 0.5f;
                    g_pcm_buffer[g_pcm_cursor] = mono;
                    g_pcm_cursor = (g_pcm_cursor + 1) % PCM_BUFFER_SIZE;
                }
            }
        }
    }
}

void seek_music(Music m, float position) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return;
    ma_uint64 frame = (ma_uint64)(position * 48000.0f);
    ma_sound_seek_to_pcm_frame(&g_music[m].sound, frame);
}

float get_music_length(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return 0;

    // Try to get length from the sound's data source
    float length = 0;
    ma_uint64 total_frames = 0;
    ma_sound_get_length_in_pcm_frames(&g_music[m].sound, &total_frames);
    if (total_frames > 0) {
        ma_uint32 sample_rate;
        ma_sound_get_data_format(&g_music[m].sound, NULL, NULL, &sample_rate, NULL, 0);
        if (sample_rate > 0) {
            length = (float)total_frames / (float)sample_rate;
        }
    }
    // Fallback to cached value
    if (length <= 0) length = g_music[m].total_length;
    return length;
}

float get_music_played(Music m) {
    if (m < 0 || m >= g_music_count || !g_music[m].active) return 0;

    ma_uint64 cursor;
    if (ma_sound_get_cursor_in_pcm_frames(&g_music[m].sound, &cursor) == MA_SUCCESS) {
        ma_uint32 sample_rate;
        ma_sound_get_data_format(&g_music[m].sound, NULL, NULL, &sample_rate, NULL, 0);
        if (sample_rate > 0) {
            return (float)cursor / (float)sample_rate;
        }
    }
    return 0;
}

void get_pcm_buffer(float *out_buffer, int *out_size) {
    if (out_buffer && out_size) {
        *out_size = PCM_BUFFER_SIZE;
        int cursor = g_pcm_cursor;
        for (int i = 0; i < PCM_BUFFER_SIZE; i++) {
            out_buffer[i] = g_pcm_buffer[(cursor + i) % PCM_BUFFER_SIZE];
        }
    }
}

// Simple DFT for visualization (not a fast FFT, but fine for 32-64 bins)
void get_music_fft(Music m, float *out_fft, int size) {
    if (m < 0 || m >= g_music_count || !g_music[m].active || !out_fft || size <= 0) return;

    // Use a window of the PCM buffer
    float pcm[512];
    int pcm_count = 512;
    int cursor = (g_pcm_cursor - pcm_count + PCM_BUFFER_SIZE) % PCM_BUFFER_SIZE;
    for (int i = 0; i < pcm_count; i++) {
        pcm[i] = g_pcm_buffer[(cursor + i) % PCM_BUFFER_SIZE];
        // Apply Hanning window
        float window = 0.5f * (1.0f - cosf(2.0f * 3.14159f * i / (pcm_count - 1)));
        pcm[i] *= window;
    }

    // Crude DFT for requested number of bins
    for (int k = 0; k < size; k++) {
        float re = 0, im = 0;
        float freq = (float)k / (float)size * 0.5f; // Look at low-mid frequencies
        for (int n = 0; n < pcm_count; n++) {
            float angle = 2.0f * 3.14159f * freq * n;
            re += pcm[n] * cosf(angle);
            im += pcm[n] * sinf(angle);
        }
        float mag = sqrtf(re*re + im*im) / pcm_count;
        out_fft[k] = mag * 100.0f; // Scale for visibility
        if (out_fft[k] > 1.0f) out_fft[k] = 0.9f + log10f(out_fft[k]); // Compression
    }
}

// ID3v2 APIC (Cover Art) extractor
Texture load_music_cover(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return -1;

    unsigned char header[10];
    if (fread(header, 1, 10, f) != 10) { fclose(f); return -1; }

    if (memcmp(header, "ID3", 3) != 0) { fclose(f); return -1; }

    // Syncsafe size
    int total_size = ((header[6] & 0x7F) << 21) | ((header[7] & 0x7F) << 14) |
                     ((header[8] & 0x7F) << 7) | (header[9] & 0x7F);

    while (ftell(f) < total_size + 10) {
        unsigned char frame_id[4];
        unsigned char frame_size_bytes[4];
        unsigned char flags[2];
        if (fread(frame_id, 1, 4, f) != 4) break;
        if (fread(frame_size_bytes, 1, 4, f) != 4) break;
        if (fread(flags, 1, 2, f) != 2) break;

        int frame_size = (frame_size_bytes[0] << 24) | (frame_size_bytes[1] << 16) |
                         (frame_size_bytes[2] << 8) | frame_size_bytes[3];
        
        // Handle syncsafe frame size in ID3v2.4
        if (header[3] == 4) {
            frame_size = ((frame_size_bytes[0] & 0x7F) << 21) | ((frame_size_bytes[1] & 0x7F) << 14) |
                         ((frame_size_bytes[2] & 0x7F) << 7) | (frame_size_bytes[3] & 0x7F);
        }

        if (memcmp(frame_id, "APIC", 4) == 0) {
            unsigned char encoding;
            if (fread(&encoding, 1, 1, f) != 1) { /* ignore */ }
            // Skip MIME type
            while (fgetc(f) != 0);
            // Skip Picture type
            fgetc(f);
            // Skip Description
            if (encoding == 0) { while (fgetc(f) != 0); }
            else { while (fgetc(f) != 0 || fgetc(f) != 0); } // UTF-16

            int current_pos = ftell(f);
            int data_start = current_pos;
            (void)data_start;
            
            // Actually, let's just read a big chunk and let stb_image figure it out
            unsigned char *data = (unsigned char *)malloc(frame_size);
            int read_size = fread(data, 1, frame_size, f);
            Texture tex = load_texture_from_memory("jpg", data, read_size);
            free(data);
            fclose(f);
            return tex;
        }
        fseek(f, frame_size, SEEK_CUR);
    }

    fclose(f);
    return -1;
}

// Sound Effects

Sound load_sound(const char *path) {
    if (!g_engine_initialized || g_sound_count >= MAX_SOUND_SLOTS) return -1;

    int idx = g_sound_count;
    // Load fully decoded into memory (not streaming) for low-latency playback
    ma_uint32 flags = MA_SOUND_FLAG_DECODE;
    if (ma_sound_init_from_file(&g_engine, path, flags, NULL, NULL, &g_sounds[idx].sound) != MA_SUCCESS) {
        fprintf(stderr, "[Audio Backend] Failed to load sound: %s\n", path);
        return -1;
    }

    g_sounds[idx].active = 1;
    g_sound_count++;
    return idx;
}

void unload_sound(Sound s) {
    if (s < 0 || s >= g_sound_count || !g_sounds[s].active) return;
    ma_sound_uninit(&g_sounds[s].sound);
    g_sounds[s].active = 0;
}

void play_sound(Sound s) {
    if (s < 0 || s >= g_sound_count || !g_sounds[s].active) return;
    // Rewind to start and play
    ma_sound_seek_to_pcm_frame(&g_sounds[s].sound, 0);
    ma_sound_start(&g_sounds[s].sound);
}
