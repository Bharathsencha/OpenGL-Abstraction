// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Bharath
//
// main.c — Basic demo for the BLZ Graphics & Audio Library

#include "blz.h"
#include <stdio.h>
#include <math.h>

int main() {
    // Initialize window
    if (init_window(800, 600, "BLZ Standalone Library Demo") != 0) {
        return 1;
    }

    // Initialize audio
    init_audio();

    printf("BLZ Library Initialized\n");

    float time = 0;

    // Main loop
    while (!window_should_close()) {
        begin_drawing();
        clear_background(COLOR_BLACK);

        time += get_frame_time();

        // Draw some shapes
        draw_rect((Rect){100, 100, 200, 150}, COLOR_RED);
        draw_circle(400, 300, 50, COLOR_BLUE);
        draw_line((Vec2){100, 500}, (Vec2){700, 500}, 5.0f, COLOR_GREEN);

        // Animated rectangle
        float x = 400 + cosf(time) * 200;
        draw_rect((Rect){x - 25, 400, 50, 50}, COLOR_GOLD);

        // Draw text
        draw_text("Welcome to BLZ!", 300, 50, 30, COLOR_WHITE);
        draw_text("Standalone C Library", 320, 85, 20, COLOR_SKYBLUE);

        end_drawing();
    }

    // Cleanup
    close_audio();
    close_window();

    printf("BLZ Library Closed\n");

    return 0;
}
