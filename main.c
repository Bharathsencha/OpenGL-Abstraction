#include "abs.h"

int main() {
    create_window(1920, 1080, "Filled vs Outlined Demo");

    while (window_is_open()) {
        
        update_window();
    }

    close_window();
    return 0;
}