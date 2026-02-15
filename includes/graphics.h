#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "colors.h"

// Window management
void create_window(int width, int height, const char* title);

void setFps(int fps); // Set the target frames per second

// Returns 1 if window is open, 0 if it should close
int window_is_open(void);

// Handles buffer swapping, polling events, and screen clearing
void update_window(void);

// Clean up
void close_window(void);

// Input handling
int is_key_pressed(int key);
int is_key_down(int key);
void get_mouse_position(float* x, float* y);
int is_mouse_button_down(int button);

// Timing
float get_delta_time(void);
float get_time(void);

// Drawing functions
// thickness: negative = filled, positive = outline thickness
void DrawRectangle(float x, float y, float width, float height, float thickness, Color color);
void DrawCircle(float x, float y, float radius, float thickness, Color color);
void DrawLine(float x1, float y1, float x2, float y2, float thickness, Color color);

// Text rendering (placeholder for now)
void DrawText(const char* text, float x, float y, Color color);

// Printable keys
#define KEY_SPACE           32
#define KEY_APOSTROPHE      39
#define KEY_COMMA           44
#define KEY_MINUS           45
#define KEY_PERIOD          46
#define KEY_SLASH           47

// Number keys (top row)
#define KEY_0               48
#define KEY_1               49
#define KEY_2               50
#define KEY_3               51
#define KEY_4               52
#define KEY_5               53
#define KEY_6               54
#define KEY_7               55
#define KEY_8               56
#define KEY_9               57

#define KEY_SEMICOLON       59
#define KEY_EQUAL           61

// Letter keys
#define KEY_A               65
#define KEY_B               66
#define KEY_C               67
#define KEY_D               68
#define KEY_E               69
#define KEY_F               70
#define KEY_G               71
#define KEY_H               72
#define KEY_I               73
#define KEY_J               74
#define KEY_K               75
#define KEY_L               76
#define KEY_M               77
#define KEY_N               78
#define KEY_O               79
#define KEY_P               80
#define KEY_Q               81
#define KEY_R               82
#define KEY_S               83
#define KEY_T               84
#define KEY_U               85
#define KEY_V               86
#define KEY_W               87
#define KEY_X               88
#define KEY_Y               89
#define KEY_Z               90

#define KEY_LEFT_BRACKET    91
#define KEY_BACKSLASH       92
#define KEY_RIGHT_BRACKET   93
#define KEY_GRAVE_ACCENT    96

// Function keys
#define KEY_ESCAPE          256
#define KEY_ENTER           257
#define KEY_TAB             258
#define KEY_BACKSPACE       259
#define KEY_INSERT          260
#define KEY_DELETE          261
#define KEY_RIGHT           262
#define KEY_LEFT            263
#define KEY_DOWN            264
#define KEY_UP              265
#define KEY_PAGE_UP         266
#define KEY_PAGE_DOWN       267
#define KEY_HOME            268
#define KEY_END             269
#define KEY_CAPS_LOCK       280
#define KEY_SCROLL_LOCK     281
#define KEY_NUM_LOCK        282
#define KEY_PRINT_SCREEN    283
#define KEY_PAUSE           284

// F1-F12
#define KEY_F1              290
#define KEY_F2              291
#define KEY_F3              292
#define KEY_F4              293
#define KEY_F5              294
#define KEY_F6              295
#define KEY_F7              296
#define KEY_F8              297
#define KEY_F9              298
#define KEY_F10             299
#define KEY_F11             300
#define KEY_F12             301

// F13-F25
#define KEY_F13             302
#define KEY_F14             303
#define KEY_F15             304
#define KEY_F16             305
#define KEY_F17             306
#define KEY_F18             307
#define KEY_F19             308
#define KEY_F20             309
#define KEY_F21             310
#define KEY_F22             311
#define KEY_F23             312
#define KEY_F24             313
#define KEY_F25             314

// Keypad numbers
#define KEY_KP_0            320
#define KEY_KP_1            321
#define KEY_KP_2            322
#define KEY_KP_3            323
#define KEY_KP_4            324
#define KEY_KP_5            325
#define KEY_KP_6            326
#define KEY_KP_7            327
#define KEY_KP_8            328
#define KEY_KP_9            329

// Keypad operators
#define KEY_KP_DECIMAL      330
#define KEY_KP_DIVIDE       331
#define KEY_KP_MULTIPLY     332
#define KEY_KP_SUBTRACT     333
#define KEY_KP_ADD          334 // Ctrl + c Ctrl + v at it's finest
#define KEY_KP_ENTER        335
#define KEY_KP_EQUAL        336

// Modifier keys
#define KEY_LEFT_SHIFT      340
#define KEY_LEFT_CONTROL    341
#define KEY_LEFT_ALT        342
#define KEY_LEFT_SUPER      343
#define KEY_RIGHT_SHIFT     344
#define KEY_RIGHT_CONTROL   345
#define KEY_RIGHT_ALT       346
#define KEY_RIGHT_SUPER     347
#define KEY_MENU            348

// Mouse buttons
#define MOUSE_LEFT          0
#define MOUSE_RIGHT         1
#define MOUSE_MIDDLE        2
#define MOUSE_BUTTON_4      3
#define MOUSE_BUTTON_5      4
#define MOUSE_BUTTON_6      5
#define MOUSE_BUTTON_7      6
#define MOUSE_BUTTON_8      7

#endif

/*
 * ============================================================================
 * BIG BEAUTIFUL RANT
 * ============================================================================
 *
 * I spent THREE FUCKING HOURS yesterday trying to find documentation on how to
 * do something that should've taken 5 goddamn minutes. Their docs are like a
 * treasure map written in invisible ink on a napkin that's been through the
 * wash. Sure, they have "docs," but good fucking luck finding a straightforward
 * example that actually compiles without sacrificing three rubber ducks to the
 * linker gods. Every function has 47 overloads, and the one I need is always
 * the one that's "deprecated since Qt 4.8" or "available since Qt 6.2" when
 * I'm stuck on Qt 5.15 because I can't be bothered to upgrade and break
 * everything again.
 *
 * And .pro files? WHAT THE ACTUAL FUCK IS A .PRO FILE? Why do I need this
 * proprietary build system that looks like it was designed by someone who saw
 * a Makefile once in a fever dream? I just want to use CMake like a normal
 * fucking person. Oh wait, I CAN use CMake with Qt, but then I still need to
 * learn qmake syntax anyway because half the examples only show .pro files and
 * the CMake Qt integration is about as intuitive as performing surgery with
 * oven mitts. QT += widgets? TARGET = MyApp? What is this shit, a config file
 * or a ransom note?
 *
 * And MOC. Oh god, MOC. The fucking Meta Object Compiler. "Just add Q_OBJECT
 * to your class!" Yeah, sure, let me just remember to run a fucking
 * preprocessor on my code because apparently C++ wasn't complicated enough
 * already. Half the time I forget and spend 20 minutes wondering why my
 * signals aren't working. You know what SDL does? NOTHING. No preprocessor.
 * No magic macros. It just fucking works. GLFW? Same thing. Just works. Even
 * the goddamn raw Win32 API is less painful than this preprocessor hell.
 *
 * Speaking of signals and slots - "Oh but they're so elegant!" Fuck that
 * noise. You know what's elegant? A regular fucking callback function that
 * doesn't require MAGIC MACROS and a separate compilation step. I've wasted
 * so many hours debugging why my button does nothing, only to find out I
 * forgot to #include "moc_myclass.cpp" or some other arcane bullshit. It's
 * 2026, why am I dealing with this prehistoric garbage?
 *
 * You know what's fucking wild? I've written apps using raw Win32 API in C.
 * Just straight C, no C++, no frameworks. And that shit was SIMPLER than Qt.
 * Yeah, I had to write a window procedure and handle WM_PAINT messages manually,
 * but at least I knew what the fuck was happening. No hidden MOC magic. No
 * mysterious vtable errors. Just straightforward "here's a message, handle it"
 * logic. When Win32 API from 1995 is easier to understand than your "modern"
 * framework, you've fucked up somewhere.
 *
 * And the fucking includes .. Is it #include <QtWidgets/QWidget> or #include
 * <QWidget>? WHO THE FUCK KNOWS? Different tutorials show different things.
 * Different Qt versions expect different things. The compiler error doesn't
 * help because it just screams about missing files instead of telling me which
 * goddamn include format to use. SDL? #include "SDL.h". Done. One fucking line.
 * GLFW? #include "GLFW/glfw3.h". Consistent. Predictable. Not Qt though. Qt
 * has to be special.
 *
 * Want to draw something custom? Cool, just inherit from QWidget and override
 * paintEvent(). Oh, but make sure you call the base class version. Or don't?
 * The docs are vague about it. And don't forget to set up a QPainter with the
 * right settings. And remember to call update() to trigger a repaint. And make
 * sure you're not blocking the event loop. SDL_RenderClear(), SDL_RenderPresent().
 * TWO FUCKING FUNCTIONS. That's it. That's all it takes to draw in SDL. But no,
 * Qt needs an entire event-driven architecture just to put a pixel on screen.
 *
 * Threading? Oh boy, threading in Qt. The "proper" way now is supposedly to
 * create a QObject and moveToThread() it to a QThread instance, but good luck
 * finding consistent documentation on it. Stack Overflow has 50 different
 * answers and half of them are "deprecated, don't do this anymore." And god
 * help you if you try to update the GUI from your thread. You can't! You have
 * to use signals and slots to communicate back to the main thread. You know
 * how you do threading in literally any other framework? std::thread. That's
 * it. That's the entire fucking thing. But Qt has to reinvent the wheel and
 * make it square.
 *
 * Qt Creator. Let's talk about Qt Creator. That fucking IDE is constantly
 * "indexing" my project. Why? What is it indexing? I have 5 source files.
 * FIVE. And it takes 10 minutes to index them. And then it hangs. And then
 * the autocomplete doesn't work. And then it crashes. And I'm back to using
 * VSCode or Vim because at least those don't pretend to be helpful while
 * actually just wasting my time.
 *
 * The designer-generated ui_*.h files. These fucking abominations live in your
 * build directory and get regenerated every time you breathe on your .ui file.
 * Want to customize something? Too bad! Your changes get overwritten. The
 * "proper" way is to use some convoluted inheritance pattern or manually edit
 * the XML. Yes, XML. In 2026. We're still editing fucking XML files to design
 * user interfaces. At least HTML has the decency to be human-readable.
 *
 * Want to use Qt? Cool, just download 4GB of libraries. I tried to distribute
 * my app last week - my 2MB executable now needs 200MB of DLLs. Static linking?
 * Sure, if I want to spend 6 hours recompiling Qt from source and then figure
 * out the LGPL relinking requirements that may or may not apply to my use case.
 *
 * Linker errors. Holy fuck, the linker errors. "undefined reference to
 * vtable for MyClass". What the fuck does that even mean? Oh, I forgot to
 * run MOC. Or I forgot Q_OBJECT. Or I forgot to add the file to my .pro file.
 * Or Mercury is in retrograde. Who fucking knows? The error message certainly
 * doesn't help. GLFW gives you actual useful error messages. Win32 API gives
 * you cryptic but at least googleable error codes. Qt gives you abstract
 * poetry about vtables and you're supposed to divine the meaning like some
 * kind of fucking oracle.
 *
 * Qt4? Qt5? Qt6? I've been through all of them and each major version breaks
 * shit in creative new ways. QString::null is gone! QList is now an alias for
 * QVector! Wait, no, QVector is gone in Qt6! What the fuck are they doing over
 * there? I'm tired of rewriting my code every few years. SDL has been backwards
 * compatible for decades. DECADES. Qt can't even maintain compatibility between
 * minor versions half the time.
 *
 * And don't give me that "but Qt is good for enterprise applications" bullshit.
 * You know what enterprises have? MONEY. Money to hire developers who get paid
 * to suffer through this nightmare. Money to buy support contracts. Money to
 * waste on 6-hour build times. For everyone else? Students? Indie devs? People
 * who just want to make a simple game or learn graphics programming? Qt is
 * OVERKILL and OVERCOMPLICATED as fuck. It's a sledgehammer when you need a
 * regular hammer, except the sledgehammer also requires a degree in medieval
 * weaponry to operate and comes with 200 pages of safety instructions in a
 * language you don't speak.
 *
 *
 * YOU'RE WELCOME, future me.
 * (Yes, I know it's "you're." I'm too angry to care right now.)
 *
 * welp idk .. I am still mad ig.
 * ============================================================================
 */