#ifndef ABS_H
#define ABS_H

// Opens the window
void create_window(int width, int height, const char* title);

// Returns 1 if window is open, 0 if it should close
int window_is_open(void);

// Handles buffer swapping, polling events, and screen clearing
void update_window(void);

// Clean up
void close_window(void);

#endif