// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Bharath
//
// blz.h — Single header for BLZ Graphics & Audio Library

#ifndef BLZ_H
#define BLZ_H

#include <stdbool.h>
#include <stddef.h>

// Types

typedef struct { unsigned char r, g, b, a; } Color;
typedef struct { float x, y, w, h; }         Rect;
typedef struct { float x, y; }               Vec2;

typedef struct {
    Vec2 offset;
    Vec2 target;
    float rotation;
    float zoom;
} Camera2D;

// Opaque handles
typedef int Texture;
typedef int Font;
typedef int RenderTex;
typedef int Image;
typedef int Music;
typedef int Sound;

// Predefined colors
#define COLOR_WHITE    (Color){255, 255, 255, 255}
#define COLOR_BLACK    (Color){0, 0, 0, 255}
#define COLOR_RED      (Color){230, 41, 55, 255}
#define COLOR_GREEN    (Color){0, 228, 48, 255}
#define COLOR_BLUE     (Color){0, 121, 241, 255}
#define COLOR_GOLD     (Color){255, 203, 0, 255}
#define COLOR_DARKGRAY (Color){80, 80, 80, 255}
#define COLOR_LIGHTGRAY (Color){200, 200, 200, 255}
#define COLOR_GRAY     (Color){130, 130, 130, 255}
#define COLOR_SKYBLUE  (Color){102, 191, 255, 255}
#define COLOR_MAROON   (Color){190, 33, 55, 255}

// Key codes (matching GLFW values)
#define KEY_SPACE           32
#define KEY_ESCAPE          256
#define KEY_ENTER           257
#define KEY_TAB             258
#define KEY_BACKSPACE       259
#define KEY_RIGHT           262
#define KEY_LEFT            263
#define KEY_DOWN            264
#define KEY_UP              265

// A-Z keys (ASCII)
#define KEY_A               65
#define KEY_B               66
#define KEY_C               67
#define KEY_D               68
#define KEY_E               69
#define KEY_F               70
#define KEY_G               71
#define KEY_H               72
#define KEY_I               73
#define KEY_J               74
#define KEY_K               75
#define KEY_L               76
#define KEY_M               77
#define KEY_N               78
#define KEY_O               79
#define KEY_P               80
#define KEY_Q               81
#define KEY_R               82
#define KEY_S               83
#define KEY_T               84
#define KEY_U               85
#define KEY_V               86
#define KEY_W               87
#define KEY_X               88
#define KEY_Y               89
#define KEY_Z               90

// Mouse buttons
#define MOUSE_LEFT          0
#define MOUSE_RIGHT         1
#define MOUSE_MIDDLE        2

// Window & System

int   init_window(int w, int h, const char *title);
int   window_should_close(void);
void  close_window(void);
void  set_target_fps(int fps);
float get_frame_time(void);
void  set_window_opacity(float opacity);

// Drawing

void  begin_drawing(void);
void  end_drawing(void);
void  clear_background(Color color);

// 2D Shapes

void  draw_rect(Rect rect, Color color);
void  draw_rect_lines(Rect rect, float thick, Color color);
void  draw_rect_rounded(Rect rect, float radius, int segments, Color color);
void  draw_rect_rounded_lines(Rect rect, float radius, int segments, Color color);
void  draw_rect_gradient_v(int x, int y, int w, int h, Color top, Color bottom);
void  draw_rect_gradient_ex(Rect rect, Color tl, Color bl, Color br, Color tr);
void  draw_circle(int cx, int cy, float radius, Color color);
void  draw_line(Vec2 start, Vec2 end, float thick, Color color);
void  draw_rect_at(int x, int y, int w, int h, Color color);

// Textures

Texture load_texture(const char *path);
void    draw_texture(Texture tex, int x, int y, Color tint);
void    draw_texture_pro(Texture tex, Rect src, Rect dest, Vec2 origin, float rot, Color tint);
int     get_texture_width(Texture tex);
int     get_texture_height(Texture tex);
void    unload_texture(Texture tex);
Texture load_texture_from_memory(const char *ext, const unsigned char *data, int size);

// Fonts

Font    load_font(const char *path, int size);
void    draw_text_ex(Font font, const char *text, Vec2 pos, float size, float spacing, Color color);
void    draw_text(const char *text, int x, int y, int size, Color color);
int     measure_text(const char *text, int size);
Vec2    measure_text_ex(Font font, const char *text, float size, float spacing);

// Input

Vec2    get_mouse_position(void);
float   get_mouse_wheel(void);
int     is_key_down(int key);
int     is_key_pressed(int key);
int     is_mouse_button_pressed(int button);
int     is_mouse_button_down(int button);
int     check_collision_point_rec(Vec2 point, Rect rect);

// Camera 2D

void    begin_mode_2d(Camera2D cam);
void    end_mode_2d(void);

// Render Textures (FBO)

RenderTex load_render_texture(int w, int h);
void      begin_texture_mode(RenderTex rt);
void      end_texture_mode(void);
void      draw_render_texture(RenderTex rt, int x, int y);
void      unload_render_texture(RenderTex rt);

// Image Manipulation

Image    load_image(const char *path);
void     image_rotate_cw(Image img);
Texture  load_texture_from_image(Image img);
void     unload_image(Image img);

// Audio System

int      init_audio(void);
void     close_audio(void);

// Music

Music    load_music(const char *path);
void     unload_music(Music m);
void     play_music(Music m);
void     stop_music(Music m);
void     pause_music(Music m);
void     resume_music(Music m);
void     update_music(Music m);
void     seek_music(Music m, float position);
float    get_music_length(Music m);
float    get_music_played(Music m);
void     get_music_fft(Music m, float *out_fft, int size);
Texture  load_music_cover(const char *path);

// PCM & FFT

void     get_pcm_buffer(float *out_buffer, int *out_size);

// Sounds

Sound    load_sound(const char *path);
void     unload_sound(Sound s);
void     play_sound(Sound s);

// Utilities

Color    color_from_hsv(float h, float s, float v);
Color    color_from_hsl(float h, float s, float l);
void     take_screenshot(const char *path);

#endif // BLZ_H
