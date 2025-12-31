#include "abs.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Hidden internal state
static GLFWwindow* window = NULL;
static int window_width = 800;
static int window_height = 600;

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
}

int window_is_open(void) {
    if (!window) return 0;
    return !glfwWindowShouldClose(window);
}

void update_window(void) {
    // Swap buffers to show the new frame
    glfwSwapBuffers(window);
    
    // Process keyboard/mouse events
    glfwPollEvents();

    // Clear the screen (ready for next draw)
    glClear(GL_COLOR_BUFFER_BIT);
}

void close_window(void) {
    // Clean up
    glfwTerminate();
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