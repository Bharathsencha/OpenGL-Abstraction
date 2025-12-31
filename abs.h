#ifndef ABS_H
#define ABS_H

#include "colors.h"

// Window management
void create_window(int width, int height, const char* title);

// Returns 1 if window is open, 0 if it should close
int window_is_open(void);

// Handles buffer swapping, polling events, and screen clearing
void update_window(void);

// Clean up
void close_window(void);

// Drawing functions
// thickness: negative = filled, positive = outline thickness
void DrawRectangle(float x, float y, float width, float height, float thickness, Color color);
void DrawCircle(float x, float y, float radius, float thickness, Color color);
void DrawLine(float x1, float y1, float x2, float y2, float thickness, Color color);

// Text rendering (placeholder for now)
void DrawText(const char* text, float x, float y, Color color);

#endif