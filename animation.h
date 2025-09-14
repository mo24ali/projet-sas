#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // usleep
#include <sys/ioctl.h>  // ioctl, winsize
#include <termios.h>
#include <string.h>

#define ANSI_RESET "\033[0m"
#define cyan "\033[0;36m"
#define bold_cyan "\033[1;36m"
#define M_PI 3.14159265358979323846

#define Sleep(ms) usleep((ms) * 1000)

typedef struct { double x, y, z; } vec3;
typedef struct { double x, y; } vec2;
typedef struct { int ws_row, ws_col; } winsize;

vec2 aspectRatio = {1, 1};

void clearScreen() {
    printf("\033[H\033[J");
}

// ---------- Sphere generator ----------
#define LAT_STEPS 25
#define LON_STEPS 50
vec3 spherePoints[LAT_STEPS * LON_STEPS];

void generateSphere(double radius) {
    int idx = 0;
    for (int i = 0; i < LAT_STEPS; i++) {
        double phi = M_PI * (double)i / (LAT_STEPS - 1) - M_PI / 2;
        for (int j = 0; j < LON_STEPS; j++) {
            double theta = 2 * M_PI * (double)j / (LON_STEPS - 1);
            double x = radius * cos(phi) * cos(theta);
            double y = radius * cos(phi) * sin(theta);
            double z = radius * sin(phi);
            spherePoints[idx++] = (vec3){x, y, z};
        }
    }
}

vec3 rotateVec3(vec3 p, vec3 r) {
    double y1 = p.y * cos(r.x) - p.z * sin(r.x);
    double z1 = p.y * sin(r.x) + p.z * cos(r.x);
    double x1 = p.x;

    double z2 = z1 * cos(r.y) - x1 * sin(r.y);
    double x2 = z1 * sin(r.y) + x1 * cos(r.y);
    double y2 = y1;

    double x3 = x2 * cos(r.z) - y2 * sin(r.z);
    double y3 = x2 * sin(r.z) + y2 * cos(r.z);
    double z3 = z2;

    return (vec3){x3, y3, z3};
}

void drawSphere(winsize w, vec3 rotation, vec2 position, double size) {
    char *screen = malloc(w.ws_col * w.ws_row);
    memset(screen, ' ', w.ws_col * w.ws_row);

    for (int i = 0; i < LAT_STEPS * LON_STEPS; i++) {
        vec3 p = rotateVec3(spherePoints[i], rotation);

        double scale = size / (p.z + 3.0);

        int x = (int)(p.x * scale * w.ws_col / 4 + w.ws_col / 2 + position.x);
        int y = (int)(p.y * scale * w.ws_row / 2 + w.ws_row / 2 + position.y);

        if (x >= 0 && x < w.ws_col && y >= 0 && y < w.ws_row) {
            char c = (p.z > 0) ? 'O' : '.';
            screen[y * w.ws_col + x] = c;
        }
    }

    printf("\033[H"); 
    for (int i = 0; i < w.ws_row; i++) {
        fwrite(screen + i * w.ws_col, 1, w.ws_col, stdout);
        putchar('\n');
    }

    free(screen);
}

// ---------- Helpers ----------
void updateAspectRatio(winsize *w) {
    int size_x = w->ws_col;
    int size_y = w->ws_row * 2;
    if (size_x > size_y) {
        aspectRatio.x = (double)size_x / size_y;
        aspectRatio.y = 1;
    } else {
        aspectRatio.x = 1;
        aspectRatio.y = (double)size_y / size_x;
    }
}

void getConsoleSize(winsize *w) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    w->ws_col = ws.ws_col;
    w->ws_row = ws.ws_row;
}

void showWelcomeMessage() {
    const char *welcome[] = {
        " __        __   _                            _          ",
        " \\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___    ",
        "  \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\ ",
        "   \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) |   ",
        "    \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/ ",
        "",
        "                WELCOME TO THE APP                      \n"
        "       FOOTBALL CLUB MANAGER code by Ali Cherraoui \n                   "
        "      SAS 2025                   "};

    int lines = sizeof(welcome) / sizeof(welcome[0]);
    printf("\033[1m");
    for (int i = 0; i < lines; i++) {
        printf("   %s\n", welcome[i]);
        Sleep(150);
    }
    printf("%s", ANSI_RESET);
}

void showGoodbyeMessage() {
    const char *goodbye[] = {
        "         _____                  _      ",
        "     /\\          |  __ \\               (_)     ",
        "    /  \\  _   _  | |__) |_____   _____  _ _ __ ",
        "   / /\\ \\| | | | |  _  // _ \\ \\ / / _ \\| | '__|",
        "  / ____ \\ |_| | | | \\ \\  __/\\ V / (_) | | |   ",
        " /_/    \\_\\__,_| |_|  \\_\\___| \\_/ \\___/|_|_|   ",
        "                                               ",
        "                                               "};

    int lines = sizeof(goodbye) / sizeof(goodbye[0]);
    printf("%s", cyan);
    for (int i = 0; i < lines; i++) {
        printf("%s   %s\n", goodbye[i], bold_cyan);
        Sleep(150);
    }
    printf("%s", ANSI_RESET);
}

// ---------- Main animation with bouncing ----------
void full_run(int duration_seconds) {
    winsize w;
    vec3 rotation = {0.0, 0.0, 0.0};
    double size = 1.0;

    // ball position and velocity
    double posX = 0, posY = 0;
    double velX = 2.0, velY = 1.0;

    generateSphere(1.0);

    int frames = duration_seconds * 25;
    for (int f = 0; f < frames; f++) {
        getConsoleSize(&w);
        updateAspectRatio(&w);
        clearScreen();

        drawSphere(w, rotation, (vec2){posX, posY}, size);

        rotation.x += 0.03;
        rotation.y += 0.02;

        // update position
        posX += velX;
        posY += velY;

        // bounce against borders
        int maxX = w.ws_col / 3;
        int maxY = w.ws_row / 3;
        if (posX > maxX || posX < -maxX) velX = -velX;
        if (posY > maxY || posY < -maxY) velY = -velY;

        Sleep(40);
    }

    Sleep(500);
    clearScreen();
    showWelcomeMessage();

    Sleep(4000);
    clearScreen();

    printf("\e[?25h"); // show cursor
}


