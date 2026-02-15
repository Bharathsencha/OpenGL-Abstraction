#include "graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <time.h> // Required for nanosleep or similar if you want to be CPU efficien

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Hidden internal state
static GLFWwindow* window = NULL;
static int window_width = 800;
static int window_height = 600;
static double last_time = 0.0;
static float delta_time = 0.0f;
static double target_frame_time = 0.0; // 0.0 means uncapped

void create_window(int width, int height, const char* title) {
    if (!glfwInit()) return;

    window_width = width;
    window_height = height;

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Set up 2D coordinate system with (0,0) at top-left
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable transparency support
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Black background by default
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initialize timing
    last_time = glfwGetTime();
}

int window_is_open(void) {
    if (!window) return 0;
    return !glfwWindowShouldClose(window);
}

void setFps(int fps) {
    if (fps > 0) {
        target_frame_time = 1.0 / fps;
    } else {
        target_frame_time = 0.0; // Unlimited
    }
}

void update_window(void) {
    if (!window) return;

    // 1. Cap FPS logic
    if (target_frame_time > 0.0) {
        // Busy-wait loop (simplest for high-precision timing in graphics)
        while (glfwGetTime() - last_time < target_frame_time) {
            // Wait until enough time has passed
        }
    }

    // 2. Calculate delta time
    double current_time = glfwGetTime();
    delta_time = (float)(current_time - last_time);
    last_time = current_time;

    // 3. Swap buffers to show the new frame
    glfwSwapBuffers(window);

    // 4. Process keyboard/mouse events
    glfwPollEvents();

    // 5. Clear the screen (ready for next draw)
    glClear(GL_COLOR_BUFFER_BIT);
}

void close_window(void) {
    // Clean up
    glfwTerminate();
}

// Input handling
int is_key_pressed(int key) {
    if (!window) return 0;
    return glfwGetKey(window, key) == GLFW_PRESS;
}

int is_key_down(int key) {
    if (!window) return 0;
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void get_mouse_position(float* x, float* y) {
    if (!window) return;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (x) *x = (float)xpos;
    if (y) *y = (float)ypos;
}

int is_mouse_button_down(int button) {
    if (!window) return 0;
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

// Timing
float get_delta_time(void) {
    return delta_time;
}

float get_time(void) {
    return (float)glfwGetTime();
}

// Helper to set OpenGL color from Color struct
static void set_gl_color(Color color) {
    glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}

// DrawRectangle: negative thickness = filled, positive = outline
void DrawRectangle(float x, float y, float width, float height, float thickness, Color color) {
    set_gl_color(color);

    if (thickness < 0) {
        // Filled rectangle
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    } else {
        // Outlined rectangle
        glLineWidth(thickness);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }
}

// DrawCircle: negative thickness = filled, positive = outline
void DrawCircle(float x, float y, float radius, float thickness, Color color) {
    set_gl_color(color);

    int segments = 50;

    if (thickness < 0) {
        // Filled circle
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y); // Center

        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * M_PI * i / segments;
            float px = x + radius * cosf(angle);
            float py = y + radius * sinf(angle);
            glVertex2f(px, py);
        }
        glEnd();
    } else {
        // Outlined circle
        glLineWidth(thickness);
        glBegin(GL_LINE_LOOP);

        for (int i = 0; i < segments; i++) {
            float angle = 2.0f * M_PI * i / segments;
            float px = x + radius * cosf(angle);
            float py = y + radius * sinf(angle);
            glVertex2f(px, py);
        }
        glEnd();
    }
}

// DrawLine: thickness determines line width
void DrawLine(float x1, float y1, float x2, float y2, float thickness, Color color) {
    set_gl_color(color);
    glLineWidth(thickness);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

// DrawText: placeholder for now
void DrawText(const char* text, float x, float y, Color color) {
    // Placeholder - proper text rendering requires font loading and rasterization
    printf("DrawText: '%s' at (%.0f, %.0f) - not yet implemented\n", text, x, y);
}
