#include "abs.h"

int main() {
    create_window(800, 600, "My App");

    while (window_is_open()) {
        // Any drawing code goes here
        
        update_window();
    }

    close_window();
    return 0;
}