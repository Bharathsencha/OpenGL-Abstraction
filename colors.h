#ifndef COLORS_H
#define COLORS_H

// Color structure
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

// Create custom colors
static inline Color rgb(int r, int g, int b) {
    return (Color){r, g, b, 255};
}

static inline Color rgba(int r, int g, int b, int a) {
    return (Color){r, g, b, a};
}

// Basic colors
#define RED           (Color){255, 0, 0, 255}
#define GREEN         (Color){0, 255, 0, 255}
#define BLUE          (Color){0, 0, 255, 255}
#define YELLOW        (Color){255, 255, 0, 255}
#define ORANGE        (Color){255, 165, 0, 255}
#define PURPLE        (Color){128, 0, 128, 255}
#define CYAN          (Color){0, 255, 255, 255}
#define MAGENTA       (Color){255, 0, 255, 255}
#define WHITE         (Color){255, 255, 255, 255}
#define BLACK         (Color){0, 0, 0, 255}
#define GRAY          (Color){128, 128, 128, 255}
#define DARK_GRAY     (Color){64, 64, 64, 255}
#define LIGHT_GRAY    (Color){192, 192, 192, 255}

// Extended colors
#define PINK          (Color){255, 192, 203, 255}
#define BROWN         (Color){165, 42, 42, 255}
#define GOLD          (Color){255, 215, 0, 255}
#define SILVER        (Color){192, 192, 192, 255}
#define NAVY          (Color){0, 0, 128, 255}
#define TEAL          (Color){0, 128, 128, 255}
#define LIME          (Color){0, 255, 0, 255}
#define MAROON        (Color){128, 0, 0, 255}
#define OLIVE         (Color){128, 128, 0, 255}
#define INDIGO        (Color){75, 0, 130, 255}
#define VIOLET        (Color){238, 130, 238, 255}

// Shades
#define DARK_RED      (Color){139, 0, 0, 255}
#define DARK_GREEN    (Color){0, 100, 0, 255}
#define DARK_BLUE     (Color){0, 0, 139, 255}
#define LIGHT_RED     (Color){255, 102, 102, 255}
#define LIGHT_GREEN   (Color){144, 238, 144, 255}
#define LIGHT_BLUE    (Color){173, 216, 230, 255}

// Transparent
#define TRANSPARENT   (Color){0, 0, 0, 0}

#endif