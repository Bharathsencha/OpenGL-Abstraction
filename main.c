#include "graphics.h"
#include "includes/graphics.h"
#include <stdio.h>

int main(void) {
    // Initialize window
    create_window(800, 600, "FPS and Input Test");

    // Start at 60 FPS
    int current_fps_limit = 60;
    setFps(current_fps_limit);

    // Player properties
    float posX = 400.0f;
    float posY = 300.0f;
    float speed = 300.0f;
    Color playerColor = CYAN;

    printf("Controls:\n");
    printf("- Move: WASD or Arrow Keys\n");
    printf("- Toggle FPS (60/Uncapped): Press 'F'\n");
    printf("- Change Color: Hold SPACE\n");
    printf("- Mouse Position: Click Left Mouse Button\n");
    printf("- Exit: ESC\n");

    while (window_is_open()) {
        float dt = get_delta_time();

        // 1. Toggle FPS Limit
        // Note: is_key_pressed logic might trigger multiple times per second
        // unless you use a "just pressed" flag, but for testing this works:
        if (is_key_down(KEY_F)) {
            if (current_fps_limit == 60) {
                current_fps_limit = 0; // Uncapped
                printf("FPS Limit: Uncapped\n");
            } else {
                current_fps_limit = 60;
                printf("FPS Limit: 60\n");
            }
            setFps(current_fps_limit);
            // Small delay to prevent flickering between states
            for(volatile int i = 0; i < 10000000; i++);
        }

        // 2. Movement Handling (Delta time ensures speed is consistent at any FPS)
        if (is_key_down(KEY_W) || is_key_down(KEY_UP))    posY -= speed * dt;
        if (is_key_down(KEY_S) || is_key_down(KEY_DOWN))  posY += speed * dt;
        if (is_key_down(KEY_A) || is_key_down(KEY_LEFT))  posX -= speed * dt;
        if (is_key_down(KEY_D) || is_key_down(KEY_RIGHT)) posX += speed * dt;

        // 3. State Change (Hold Space)
        playerColor = is_key_down(KEY_SPACE) ? GOLD : CYAN;

        // 4. Mouse Interaction
        if (is_mouse_button_down(MOUSE_LEFT)) {
            float mx, my;
            get_mouse_position(&mx, &my);
            printf("Mouse Position: X: %.2f, Y: %.2f | Delta Time: %.4f\n", mx, my, dt);
        }

        // 5. Exit Shortcut
        if (is_key_down(KEY_ESCAPE)) break;

        // --- Drawing ---------------------------------------------------------- Bruh how to fix this shit 

        // Background Grid
        for(int i = 0; i < 800; i += 100) DrawLine(i, 0, i, 600, 1, DARK_GRAY);
        for(int i = 0; i < 600; i += 100) DrawLine(0, i, 800, i, 1, DARK_GRAY);

        // Player Square
        DrawRectangle(posX - 25, posY - 25, 50, 50, -1, playerColor);

        // Mouse Follower
        float mx, my;
        get_mouse_position(&mx, &my);
        DrawCircle(mx, my, 15, 2, WHITE);

        // Update window (This is where the FPS wait loop happens)
        update_window();
    }

    close_window();
    return 0;
}
