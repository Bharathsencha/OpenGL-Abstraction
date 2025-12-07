#include "abs.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

// Hidden internal state
static GLFWwindow* window = NULL;

void create_window(int width, int height, const char* title) {
    if (!glfwInit()) return;

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window);
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
    // You can change the color here: glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void close_window(void) {
    glfwTerminate();
}