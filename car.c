// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Bharath
//
// car.c — Neon Retro Racer Demo for BLZ Library
// Ported from car_retro.lu

#include "blz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define LANE_WIDTH 120
#define ROAD_WIDTH (LANE_WIDTH * 3)
#define ROAD_X ((SCREEN_WIDTH - ROAD_WIDTH) / 2)
#define CAR_WIDTH 60
#define CAR_HEIGHT 100

// Colors
const Color C_BG        = {10, 10, 20, 255};
const Color C_GRID      = {40, 0, 80, 255};
const Color C_ROAD      = {20, 20, 25, 255};
const Color C_PLAYER    = {0, 255, 255, 255};
const Color C_ENEMY     = {255, 50, 50, 255};
const Color C_TEXT      = {255, 255, 255, 255};
const Color C_NEON_LINE = {255, 0, 255, 255};
const Color C_SELECT    = {255, 255, 0, 255};
const Color C_NIGHTMARE = {255, 0, 0, 255};

// Game State
typedef enum { STATE_MENU, STATE_PLAY, STATE_GAMEOVER } GameState;
GameState current_state = STATE_MENU;

// Settings
int difficulty = 0; // 0=Easy, 1=Medium, 2=Hard, 3=Nightmare
int target_fps = 60;
int menu_selection = 0;

// Game Variables
float player_x = SCREEN_WIDTH / 2.0f - CAR_WIDTH / 2.0f;
float player_y = SCREEN_HEIGHT - 130.0f;
int player_lane = 1;
float target_x_val = 0;
float speed_base = 400.0f;
float game_speed = 400.0f;
float score = 0.0f;
int high_scores[3] = {0, 0, 0};
float game_time = 0.0f;
float grid_offset = 0.0f;
float line_offset = 0.0f;

// Traffic
#define POOL_SIZE 20
typedef struct {
    float x, y;
    float target_x;
    int lane;
    int active;
} Traffic;

Traffic traffic[POOL_SIZE];
float spawn_timer = 0.0f;

// Assets
Music bg_music;
Texture arrow_left;
Texture arrow_right;

int check_collision_rects(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void reset_game() {
    player_lane = 1;
    player_x = ROAD_X + player_lane * LANE_WIDTH + (LANE_WIDTH - CAR_WIDTH) / 2.0f;
    target_x_val = player_x;

    if (difficulty == 0) speed_base = 400.0f;
    else if (difficulty == 1) speed_base = 600.0f;
    else if (difficulty == 2) speed_base = 800.0f;
    else if (difficulty == 3) speed_base = 1600.0f;

    game_speed = speed_base;
    score = 0.0f;
    spawn_timer = 0.0f;

    for (int i = 0; i < POOL_SIZE; i++) {
        traffic[i].active = 0;
    }
}

void update_high_scores(int new_score) {
    if (new_score > high_scores[0]) {
        high_scores[2] = high_scores[1];
        high_scores[1] = high_scores[0];
        high_scores[0] = new_score;
    } else if (new_score > high_scores[1]) {
        high_scores[2] = high_scores[1];
        high_scores[1] = new_score;
    } else if (new_score > high_scores[2]) {
        high_scores[2] = new_score;
    }
}

void draw_centered_text(const char *text, int y, int size, Color color) {
    int w = measure_text(text, size);
    draw_text(text, (SCREEN_WIDTH - w) / 2, y, size, color);
}

int main() {
    init_window(SCREEN_WIDTH, SCREEN_HEIGHT, "Neon Retro Racer (C)");
    init_audio();
    set_target_fps(60);
    srand(time(NULL));

    bg_music = load_music("assets/kavinsky.mp3");
    play_music(bg_music);

    arrow_left = load_texture("assets/left-arrow.png");
    arrow_right = load_texture("assets/right-arrow.png");

    while (!window_should_close()) {
        float dt = get_frame_time();
        game_time += dt;
        update_music(bg_music);

        grid_offset += (game_speed * 0.5f) * dt;
        if (grid_offset >= 40.0f) grid_offset = 0.0f;

        line_offset += game_speed * dt;
        if (line_offset >= 80.0f) line_offset -= 80.0f;

        if (current_state == STATE_MENU) {
            if (is_key_pressed(KEY_UP) || is_key_pressed(KEY_W)) {
                menu_selection = (menu_selection - 1 + 3) % 3;
            }
            if (is_key_pressed(KEY_DOWN) || is_key_pressed(KEY_S)) {
                menu_selection = (menu_selection + 1) % 3;
            }
            if (is_key_pressed(KEY_ENTER) || is_key_pressed(KEY_SPACE)) {
                reset_game();
                current_state = STATE_PLAY;
            }

            if (menu_selection == 1) { // Difficulty
                if (is_key_pressed(KEY_LEFT) || is_key_pressed(KEY_A)) {
                    difficulty = (difficulty - 1 + 4) % 4;
                }
                if (is_key_pressed(KEY_RIGHT) || is_key_pressed(KEY_D)) {
                    difficulty = (difficulty + 1) % 4;
                }
            }
            if (menu_selection == 2) { // FPS
                if (is_key_pressed(KEY_LEFT) || is_key_pressed(KEY_RIGHT) || is_key_pressed(KEY_A) || is_key_pressed(KEY_D)) {
                    target_fps = (target_fps == 60) ? 30 : 60;
                    set_target_fps(target_fps);
                }
            }
        } else if (current_state == STATE_PLAY) {
            if (is_key_pressed(KEY_ESCAPE)) current_state = STATE_MENU;

            score += (400.0f * 0.01f) * dt;
            float inc = 0.5f;
            if (difficulty == 1) inc = 0.8f;
            else if (difficulty == 2) inc = 1.2f;
            else if (difficulty == 3) inc = 4.0f;

            game_speed = speed_base + (score * inc);
            if (game_speed > 2500.0f) game_speed = 2500.0f;

            if (is_key_pressed(KEY_A) || is_key_pressed(KEY_LEFT)) {
                if (player_lane > 0) player_lane--;
            }
            if (is_key_pressed(KEY_D) || is_key_pressed(KEY_RIGHT)) {
                if (player_lane < 2) player_lane++;
            }

            target_x_val = ROAD_X + player_lane * LANE_WIDTH + (LANE_WIDTH - CAR_WIDTH) / 2.0f;
            player_x += (target_x_val - player_x) * 10.0f * dt;

            spawn_timer -= dt;
            if (spawn_timer <= 0.0f) {
                int cars_to_spawn = 1;
                int chance = (difficulty == 1) ? 20 : (difficulty == 2) ? 40 : (difficulty == 3) ? 60 : 0;
                if (rand() % 100 < chance) cars_to_spawn = 2;

                int first_lane = -1;
                for (int c = 0; c < cars_to_spawn; c++) {
                    int found_idx = -1;
                    for (int i = 0; i < POOL_SIZE; i++) {
                        if (!traffic[i].active) {
                            found_idx = i;
                            break;
                        }
                    }

                    if (found_idx != -1) {
                        int lane = rand() % 3;
                        if (c == 1) while (lane == first_lane) lane = rand() % 3;
                        first_lane = lane;

                        float tx = ROAD_X + lane * LANE_WIDTH + (LANE_WIDTH - CAR_WIDTH) / 2.0f;
                        traffic[found_idx].x = tx;
                        traffic[found_idx].target_x = tx;
                        traffic[found_idx].lane = lane;
                        traffic[found_idx].y = -150.0f;
                        traffic[found_idx].active = 1;
                    }
                }

                float base_spawn_rate = (difficulty == 1) ? 1100.0f : (difficulty == 2) ? 1300.0f : (difficulty == 3) ? 1800.0f : 1000.0f;
                spawn_timer = base_spawn_rate / game_speed;
                if (spawn_timer < 0.25f) spawn_timer = 0.25f;
            }

            for (int i = 0; i < POOL_SIZE; i++) {
                if (traffic[i].active) {
                    traffic[i].y += (game_speed * 0.8f) * dt;
                    traffic[i].x += (traffic[i].target_x - traffic[i].x) * 5.0f * dt;

                    if (fabsf(traffic[i].target_x - traffic[i].x) < 5.0f && traffic[i].y < player_y - 200.0f) {
                        int chance = (difficulty == 0) ? 1 : (difficulty == 1) ? 4 : (difficulty == 2 || difficulty == 3) ? 10 : 0;
                        if (rand() % 1000 < chance) {
                            int new_lane = traffic[i].lane;
                            if (traffic[i].lane < player_lane) new_lane++;
                            else if (traffic[i].lane > player_lane) new_lane--;

                            if (new_lane != traffic[i].lane) {
                                traffic[i].lane = new_lane;
                                traffic[i].target_x = ROAD_X + new_lane * LANE_WIDTH + (LANE_WIDTH - CAR_WIDTH) / 2.0f;
                            }
                        }
                    }

                    if (check_collision_rects(player_x + 5, player_y + 5, CAR_WIDTH - 10, CAR_HEIGHT - 10, traffic[i].x, traffic[i].y, CAR_WIDTH, CAR_HEIGHT)) {
                        if (difficulty == 3) score *= 2.0f;
                        update_high_scores((int)score);
                        current_state = STATE_GAMEOVER;
                    }

                    if (traffic[i].y > SCREEN_HEIGHT + 100) traffic[i].active = 0;
                }
            }
        } else if (current_state == STATE_GAMEOVER) {
            if (is_key_pressed(KEY_ENTER) || is_key_pressed(KEY_SPACE)) {
                reset_game();
                current_state = STATE_PLAY;
            }
            if (is_key_pressed(KEY_ESCAPE)) current_state = STATE_MENU;
        }

        begin_drawing();
        clear_background(C_BG);

        // Grid
        for (float gy = grid_offset; gy < SCREEN_HEIGHT; gy += 40.0f) {
            int alpha = (int)((gy / SCREEN_HEIGHT) * 100);
            draw_line((Vec2){0, gy}, (Vec2){SCREEN_WIDTH, gy}, 1, (Color){C_GRID.r, C_GRID.g, C_GRID.b, alpha});
        }
        draw_line((Vec2){0, SCREEN_HEIGHT}, (Vec2){SCREEN_WIDTH / 2.0f - 50.0f, 0}, 1, (Color){C_GRID.r, C_GRID.g, C_GRID.b, 50});
        draw_line((Vec2){SCREEN_WIDTH, SCREEN_HEIGHT}, (Vec2){SCREEN_WIDTH / 2.0f + 50.0f, 0}, 1, (Color){C_GRID.r, C_GRID.g, C_GRID.b, 50});

        // Road
        draw_rect((Rect){ROAD_X, 0, ROAD_WIDTH, SCREEN_HEIGHT}, C_ROAD);
        for (float ly = -80.0f + line_offset; ly < SCREEN_HEIGHT; ly += 80.0f) {
            draw_rect((Rect){ROAD_X + LANE_WIDTH - 2, ly, 4, 40}, C_NEON_LINE);
            draw_rect((Rect){ROAD_X + LANE_WIDTH * 2 - 2, ly, 4, 40}, C_NEON_LINE);
        }
        draw_rect((Rect){ROAD_X - 5, 0, 5, SCREEN_HEIGHT}, C_NEON_LINE);
        draw_rect((Rect){ROAD_X + ROAD_WIDTH, 0, 5, SCREEN_HEIGHT}, C_NEON_LINE);

        if (current_state == STATE_MENU) {
            draw_centered_text("NEON RACER", 80, 60, (Color){0, 255, 255, 255});
            
            Color start_col = (menu_selection == 0) ? C_SELECT : C_TEXT;
            draw_centered_text("START GAME", 200, 30, start_col);

            const char *diff_str = (difficulty == 1) ? "MEDIUM" : (difficulty == 2) ? "HARD" : (difficulty == 3) ? "NIGHTMARE" : "EASY";
            Color diff_col = (difficulty == 3) ? C_NIGHTMARE : (menu_selection == 1) ? C_SELECT : C_TEXT;
            
            char diff_buf[64];
            sprintf(diff_buf, "DIFFICULTY:  %s  ", diff_str);
            int diff_w = measure_text(diff_buf, 30);
            int diff_x = (SCREEN_WIDTH - diff_w) / 2;
            draw_text(diff_buf, diff_x, 250, 30, diff_col);

            if (menu_selection == 1) {
                if (arrow_left != -1) draw_texture_pro(arrow_left, (Rect){0, 0, 32, 32}, (Rect){diff_x + 180, 250, 30, 30}, (Vec2){0, 0}, 0, COLOR_WHITE);
                if (arrow_right != -1) {
                    char tmp_buf[64];
                    sprintf(tmp_buf, "DIFFICULTY:  %s", diff_str);
                    int str_w = measure_text(tmp_buf, 30);
                    draw_texture_pro(arrow_right, (Rect){0, 0, 32, 32}, (Rect){diff_x + str_w + 10, 250, 30, 30}, (Vec2){0, 0}, 0, COLOR_WHITE);
                }
            }

            Color fps_col = (menu_selection == 2) ? C_SELECT : C_TEXT;
            char fps_buf[64];
            sprintf(fps_buf, "FPS:  %d  ", target_fps);
            int fps_w = measure_text(fps_buf, 30);
            int fps_x = (SCREEN_WIDTH - fps_w) / 2;
            draw_text(fps_buf, fps_x, 300, 30, fps_col);

            if (menu_selection == 2) {
                if (arrow_left != -1) draw_texture_pro(arrow_left, (Rect){0, 0, 32, 32}, (Rect){fps_x + 60, 300, 30, 30}, (Vec2){0, 0}, 0, COLOR_WHITE);
                if (arrow_right != -1) draw_texture_pro(arrow_right, (Rect){0, 0, 32, 32}, (Rect){fps_x + 130, 300, 30, 30}, (Vec2){0, 0}, 0, COLOR_WHITE);
            }

            draw_centered_text("TOP SCORES", 400, 20, C_NEON_LINE);
            for (int i = 0; i < 3; i++) {
                char score_buf[64];
                sprintf(score_buf, "%d. %d", i + 1, high_scores[i]);
                draw_centered_text(score_buf, 430 + i * 30, 20, C_TEXT);
            }
            draw_centered_text("Arrows/WASD to Select", 550, 20, (Color){100, 100, 100, 255});
        } else {
            // Player
            draw_rect_lines((Rect){player_x, player_y, CAR_WIDTH, CAR_HEIGHT}, 2, C_PLAYER);
            draw_rect((Rect){player_x + 10, player_y + 20, CAR_WIDTH - 20, 20}, (Color){0, 255, 255, 50});
            draw_line((Vec2){player_x, player_y + CAR_HEIGHT}, (Vec2){player_x + 10, player_y + CAR_HEIGHT + 10}, 1, C_PLAYER);
            draw_line((Vec2){player_x + CAR_WIDTH, player_y + CAR_HEIGHT}, (Vec2){player_x + CAR_WIDTH - 10, player_y + CAR_HEIGHT + 10}, 1, C_PLAYER);

            // Traffic
            for (int i = 0; i < POOL_SIZE; i++) {
                if (traffic[i].active) {
                    draw_rect_lines((Rect){traffic[i].x, traffic[i].y, CAR_WIDTH, CAR_HEIGHT}, 2, C_ENEMY);
                    draw_rect((Rect){traffic[i].x + 10, traffic[i].y + 60, CAR_WIDTH - 20, 10}, (Color){255, 0, 0, 100});
                }
            }

            char score_buf[64];
            sprintf(score_buf, "SCORE: %d", (int)score);
            draw_text(score_buf, 20, 20, 30, C_TEXT);
            if (current_state == STATE_PLAY) draw_text("ESC: MENU", 20, 60, 20, (Color){150, 150, 150, 255});

            if (current_state == STATE_GAMEOVER) {
                draw_rect((Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Color){0, 0, 0, 200});
                draw_centered_text("CRASHED", 200, 60, C_ENEMY);
                char final_score_buf[64];
                sprintf(final_score_buf, "FINAL SCORE: %d", (int)score);
                draw_centered_text(final_score_buf, 300, 30, C_TEXT);
                draw_centered_text("PRESS ENTER TO RETRY", 400, 20, (Color){200, 200, 200, 255});
                draw_centered_text("PRESS ESC FOR MENU", 440, 20, (Color){150, 150, 150, 255});
            }
        }

        end_drawing();
    }

    unload_texture(arrow_left);
    unload_texture(arrow_right);
    unload_music(bg_music);
    close_audio();
    close_window();

    return 0;
}
