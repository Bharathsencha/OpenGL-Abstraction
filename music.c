// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Bharath



#include "blz.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define WIDTH 900
#define HEIGHT 700

const char* songs[] = {
    "music/songs/Virtual Love - Tanin Jazz ( English translation lyrics)-(130k).mp3",
    "music/songs/TheFatRat - Jackpot (Jackpot EP Track 1)-(134k).mp3",
    "music/songs/TheFatRat - Rise Up-(133k).mp3",
    "music/songs/TheFatRat & Anjulie - Close To The Sun-(136k).mp3",
    "music/songs/TheFatRat & Cecilia Gault - Escaping Gravity [Chapter 3]-(134k).mp3",
    "music/songs/The Pointer Sisters - Hot Together (Official Audio)-(130k).mp3",
    "music/songs/Hozier - Too Sweet (Official Video)-(131k).mp3",
    "music/songs/Alphaville - Forever Young (Official Music Video)-(129k).mp3",
    "music/songs/Max Raabe - We Will Rock You-(134k).mp3",
    "music/songs/Sunflower - Spider-Man_ Into the Spider-Verse_spotdown.org.mp3",
    "music/songs/Tears For Fears - Everybody Wants To Rule The World (Official Music Video)-(132k).mp3",
    "music/songs/The Score - Unstoppable (Lyric Video)-(129k).mp3",
    "music/songs/Tip Toe.mp3",
    "music/songs/yung kai - blue (official audio)-(141k).mp3"
};

const char* song_names[] = {
    "Virtual Love - Tanin Jazz",
    "Jackpot - TheFatRat",
    "Rise Up - TheFatRat",
    "Close To The Sun - TheFatRat & Anjulie",
    "Escaping Gravity - TheFatRat & Cecilia Gault",
    "Hot Together - The Pointer Sisters",
    "Too Sweet - Hozier",
    "Forever Young - Alphaville",
    "We Will Rock You - Max Raabe",
    "Sunflower - Spider-Man",
    "Everybody Wants To Rule The World - Tears For Fears",
    "Unstoppable - The Score",
    "Tip Toe",
    "Blue - yung kai"
};

#define SONG_COUNT (sizeof(songs) / sizeof(songs[0]))

int main() {
    if (init_window(WIDTH, HEIGHT, "BLZ Retro Music Player") != 0) return 1;
    init_audio();

    // Assets
    Font font_l = load_font("music/assets/ShadeBlue-2OozX.ttf", 60);
    Font font_s = load_font("music/assets/ShadeBlue-2OozX.ttf", 30);
    
    Texture tex_play  = load_texture("music/assets/icons8-play-64.png");
    Texture tex_pause = load_texture("music/assets/icons8-pause-50.png");
    Texture tex_next  = load_texture("music/assets/icons8-forward-50_arrow.png");

    int current_idx = 0;
    Music music = load_music(songs[current_idx]);
    play_music(music);
    Texture cover = load_music_cover(songs[current_idx]);

    int is_playing = 1;
    float time = 0.0f;
    float fft[64];

    while (!window_should_close()) {
        Vec2 mouse = get_mouse_position();
        update_music(music);
        float dt = get_frame_time();
        time += dt;

        // Auto-next track
        float len = get_music_length(music);
        float played = get_music_played(music);
        if (len > 0 && played >= len - 0.1f) {
            unload_music(music);
            if (cover >= 0) unload_texture(cover);
            current_idx = (current_idx + 1) % SONG_COUNT;
            music = load_music(songs[current_idx]);
            play_music(music);
            cover = load_music_cover(songs[current_idx]);
        }

        begin_drawing();
        clear_background(COLOR_BLACK);

        // Background Gradient
        float h_bl = 270.0f + sinf(time * 0.4f) * 20.0f;
        float h_tr = 200.0f + cosf(time * 0.6f) * 20.0f;
        float lp = 0.35f + sinf(time * 0.8f) * 0.05f;
        draw_rect_gradient_ex((Rect){0, 0, WIDTH, HEIGHT},
                             color_from_hsl((h_bl+h_tr)/2.0f, 0.7f, lp-0.05f),
                             color_from_hsl(h_bl, 0.8f, lp),
                             color_from_hsl((h_bl+h_tr)/2.0f, 0.7f, lp-0.05f),
                             color_from_hsl(h_tr, 0.8f, lp));

        // Dark overlay
        draw_rect((Rect){0, 0, WIDTH, HEIGHT}, (Color){0, 0, 0, 120});

        // Album Art
        int art_size = 220;
        int art_x = 40, art_y = 30;
        if (cover >= 0) {
            int cw = get_texture_width(cover);
            int ch = get_texture_height(cover);
            float scale = (cw > ch) ? (float)art_size/cw : (float)art_size/ch;
            int dw = cw * scale;
            int dh = ch * scale;
            draw_texture_pro(cover, (Rect){0, 0, cw, ch}, 
                             (Rect){art_x + (art_size-dw)/2, art_y + (art_size-dh)/2, dw, dh},
                             (Vec2){0,0}, 0, COLOR_WHITE);
        } else {
            draw_rect((Rect){art_x, art_y, art_size, art_size}, (Color){40, 40, 40, 200});
            draw_text("NO COVER", art_x + 60, art_y + 100, 20, COLOR_GRAY);
        }
        draw_rect_lines((Rect){art_x, art_y, art_size, art_size}, 2, (Color){255, 255, 255, 40});

        // Song Info
        int info_x = art_x + art_size + 30;
        int info_y = art_y + 70;
        draw_text_ex(font_l, song_names[current_idx], (Vec2){info_x, info_y}, 36, 2, COLOR_WHITE);
        
        char track_info[32];
        sprintf(track_info, "%d / %d", current_idx + 1, (int)SONG_COUNT);
        draw_text_ex(font_s, track_info, (Vec2){info_x, info_y + 45}, 25, 2, (Color){180, 180, 220, 200});
        draw_text_ex(font_s, is_playing ? "NOW PLAYING" : "PAUSED", (Vec2){info_x, info_y + 85}, 25, 2, (Color){0, 230, 255, 200});

        // Visualizer
        int viz_top = 280, viz_height = 220;
        draw_rect((Rect){20, viz_top - 5, WIDTH - 40, viz_height + 10}, (Color){0, 0, 0, 80});
        
        get_music_fft(music, fft, 64);
        int num_bars = 64;
        float bar_w = (float)(WIDTH - 60) / num_bars;
        for (int i = 0; i < num_bars; i++) {
            float h = fft[i] * viz_height;
            if (h < 4) h = 4;
            if (h > viz_height) h = viz_height;
            
            Color col = color_from_hsl(180.0f + (i * 160.0f / num_bars), 1.0f, 0.55f);
            draw_rect((Rect){30 + i * bar_w, viz_top + viz_height - h, bar_w - 1, h}, col);
        }

        // Progress Bar
        int bar_y = viz_top + viz_height + 30;
        int bar_w_px = WIDTH - 60;
        Rect bar_rect = {30, bar_y, bar_w_px, 8};
        
        draw_rect(bar_rect, (Color){255, 255, 255, 30});
        
        if (len > 0) {
            float progress = played / len;
            draw_rect((Rect){30, bar_y, bar_w_px * progress, 8}, (Color){0, 220, 255, 255});
            draw_circle(30 + bar_w_px * progress, bar_y + 4, 7, COLOR_WHITE);
            
            // Seek interaction
            if (is_mouse_button_down(MOUSE_LEFT)) {
                if (check_collision_point_rec(mouse, (Rect){bar_rect.x, bar_rect.y - 10, bar_rect.w, bar_rect.h + 20})) {
                    float pct = (mouse.x - bar_rect.x) / bar_rect.w;
                    if (pct < 0) pct = 0;
                    if (pct > 1) pct = 1;
                    seek_music(music, len * pct);
                }
            }
            
            char time_cur[16], time_tot[16];
            sprintf(time_cur, "%0d:%02d", (int)played/60, (int)played%60);
            sprintf(time_tot, "%0d:%02d", (int)len/60, (int)len%60);
            draw_text_ex(font_s, time_cur, (Vec2){30, bar_y + 15}, 25, 2, COLOR_LIGHTGRAY);
            draw_text_ex(font_s, time_tot, (Vec2){30 + bar_w_px - 50, bar_y + 15}, 25, 2, COLOR_LIGHTGRAY);
        }

        // Controls
        int ctrl_y = bar_y + 60;
        int btn_size = 50;
        int mid_x = WIDTH / 2;

        // Play/Pause
        Texture cur_tex = is_playing ? tex_pause : tex_play;
        Rect play_rect = {mid_x - btn_size/2, ctrl_y, btn_size, btn_size};
        draw_texture_pro(cur_tex, (Rect){0, 0, get_texture_width(cur_tex), get_texture_height(cur_tex)},
                         play_rect, (Vec2){0,0}, 0, COLOR_WHITE);
        if (is_mouse_button_pressed(MOUSE_LEFT) && check_collision_point_rec(mouse, play_rect)) {
            is_playing = !is_playing;
            if (is_playing) resume_music(music); else pause_music(music);
        }

        // Next/Prev
        Rect next_rect = {mid_x + 60, ctrl_y + 5, 40, 40};
        Rect prev_rect = {mid_x - 100, ctrl_y + 5, 40, 40};
        draw_texture_pro(tex_next, (Rect){0, 0, get_texture_width(tex_next), get_texture_height(tex_next)},
                         next_rect, (Vec2){0,0}, 0, COLOR_WHITE);
        draw_texture_pro(tex_next, (Rect){0, 0, -get_texture_width(tex_next), get_texture_height(tex_next)},
                         prev_rect, (Vec2){0,0}, 0, COLOR_WHITE);

        if (is_mouse_button_pressed(MOUSE_LEFT)) {
            int change = 0;
            if (check_collision_point_rec(mouse, next_rect)) change = 1;
            if (check_collision_point_rec(mouse, prev_rect)) change = -1;
            if (change != 0) {
                unload_music(music);
                if (cover >= 0) unload_texture(cover);
                current_idx = (current_idx + change + SONG_COUNT) % SONG_COUNT;
                music = load_music(songs[current_idx]);
                play_music(music);
                cover = load_music_cover(songs[current_idx]);
                is_playing = 1;
            }
        }

        end_drawing();
    }

    close_audio();
    close_window();
    return 0;
}
