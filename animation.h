// main.c  -- version Linux / Ubuntu
#define _POSIX_C_SOURCE 200112L
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ANSI_RESET "\033[0m"

struct vec3 { double x, y, z; };
struct vec2 { double x, y; };

struct cube {
    struct vec3 rotation;
    struct vec2 position;
    double size;
};

struct convexHull {
    struct vec2 *points;
    int size;
};

/* small terminal-size container (to avoid name conflict with sys winsize) */
struct term_size { int ws_row; int ws_col; };

void clearScreen() { printf("\033[H\033[J"); fflush(stdout); }

int color = 31;
struct vec2 aspectRatio = {1, 1};

double crossProduct(struct vec2 A, struct vec2 B, struct vec2 C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

void sortPoints(struct vec2 *points, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (points[j].x < points[i].x ||
                (points[j].x == points[i].x && points[j].y < points[i].y)) {
                struct vec2 tmp = points[i];
                points[i] = points[j];
                points[j] = tmp;
            }
        }
    }
}

struct convexHull *computeConvexHull(struct vec2 *points, size_t size) {
    if (size == 0) return NULL;
    sortPoints(points, size);
    struct vec2 *hull = malloc(sizeof(struct vec2) * (size * 2));
    int k = 0;
    for (size_t i = 0; i < size; i++) {
        while (k >= 2 && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) k--;
        hull[k++] = points[i];
    }
    for (int i = (int)size - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) k--;
        hull[k++] = points[i];
    }
    struct convexHull *ch = malloc(sizeof(struct convexHull));
    ch->points = hull;
    ch->size = k;
    return ch;
}

int isInsideConvexHull(struct vec2 P, struct convexHull *ch) {
    if (!ch || ch->size < 3) return 0;
    for (int i = 1; i < ch->size; i++) {
        double cross = crossProduct(ch->points[i - 1], ch->points[i], P);
        if (cross < 0) return 0;
    }
    return 1;
}

void getBounds(struct vec2 *points, size_t size, struct vec2 *minBound, struct vec2 *maxBound) {
    if (size == 0) return;
    *minBound = points[0];
    *maxBound = points[0];
    for (size_t i = 1; i < size; i++) {
        if (points[i].x < minBound->x) minBound->x = points[i].x;
        if (points[i].y < minBound->y) minBound->y = points[i].y;
        if (points[i].x > maxBound->x) maxBound->x = points[i].x;
        if (points[i].y > maxBound->y) maxBound->y = points[i].y;
    }
}

void drawBall(struct vec2 center, double radius, struct term_size *w) {
    int cols = w->ws_col;
    int rows = w->ws_row;
    double sx = aspectRatio.x, sy = aspectRatio.y;
    printf("\033[H");
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            double nx = (double)x / cols;
            double ny = (double)y / rows;
            double dx = (nx - center.x) / (radius);
            double dy = (ny - center.y) / (radius);
            if (dx * dx / (sx * sx) + dy * dy / (sy * sy) <= 1.0) {
                printf("\033[37mO");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void drawSoccerBall(struct cube *ball, struct term_size *w) {
    int cols = w->ws_col, rows = w->ws_row;
    double r = ball->size, cx = ball->position.x, cy = ball->position.y;
    printf("\033[H");
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            double nx = (double)x / cols;
            double ny = (double)y / rows;
            double dx = (nx - cx) / r;
            double dy = (ny - cy) / r;
            if (dx*dx + dy*dy <= 1.0) {
                double stripe = fmod(fabs(dx*5 + dy*5 + ball->rotation.z), 1.0);
                if (stripe < 0.15) printf("\033[30m#");
                else printf("\033[37mO");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/* draw cube faces projected */
void draw(struct vec2 faces[3][4], char chars[], struct term_size *w) {
    struct vec2 minBound, maxBound;
    getBounds((struct vec2 *)faces, 12, &minBound, &maxBound);
    struct convexHull *hulls[3];
    for (int i = 0; i < 3; i++) {
        /* copy since computeConvexHull sorts the input */
        struct vec2 temp[4];
        for (int j = 0; j < 4; j++) temp[j] = faces[i][j];
        hulls[i] = computeConvexHull(temp, 4);
    }

    int x0 = (int)floor(minBound.x / aspectRatio.x * w->ws_col);
    int y0 = (int)floor(minBound.y / aspectRatio.y * w->ws_row);
    int size_x = (int)ceil(maxBound.x / aspectRatio.x * w->ws_col);
    int size_y = (int)ceil(maxBound.y / aspectRatio.y * w->ws_row);

    if (size_x > w->ws_col) size_x = w->ws_col;
    if (size_y > w->ws_row) size_y = w->ws_row;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;

    if (size_x <= x0 || size_y <= y0) {
        // nothing visible
        printf("\033[H");
        printf(" ");
        for (int i = 0; i < 3; i++) {
            if (hulls[i]) { free(hulls[i]->points); free(hulls[i]); }
        }
        return;
    }

    /* VLA of strings (each cell up to 8 chars to hold escape sequences) */
    char (*output)[size_x][8] = malloc(sizeof(char) * (size_y - y0) * size_x * 8);
    if (!output) {
        for (int i = 0; i < 3; i++) {
            if (hulls[i]) { free(hulls[i]->points); free(hulls[i]); }
        }
        return;
    }
    memset(output, 0, sizeof(char) * (size_y - y0) * size_x * 8);

    for (int i = y0; i < size_y; i++) {
        for (int j = x0; j < size_x; j++) {
            struct vec2 P = {(double)j * aspectRatio.x / w->ws_col,
                             (double)i * aspectRatio.y / w->ws_row};
            int inside = 0;
            for (int c = 0; c < 3; c++) {
                if (isInsideConvexHull(P, hulls[c])) { inside = 1; break; }
            }
            if (inside) {
                int pattern = ((int)(P.x * 20) + (int)(P.y * 10)) % 6;
                if (pattern == 0 || pattern == 3) {
                    snprintf(output[i - y0][j - x0], 8, "\033[30m@");
                } else {
                    snprintf(output[i - y0][j - x0], 8, "\033[37mO");
                }
            } else {
                snprintf(output[i - y0][j - x0], 8, " ");
            }
        }
    }

    printf("\e[?25l\033[1m"); // hide cursor + bold
    for (int i = 0; i < (size_y - y0); i++) {
        printf("\033[%d;%dH", i + y0 + 1, x0 + 1); // 1-based cursor position
        for (int j = 0; j < (size_x - x0); j++) {
            printf("%s", output[i][j]);
        }
        printf("\n");
    }
    printf(ANSI_RESET);

    free(output);
    for (int i = 0; i < 3; i++) {
        if (hulls[i]) { free(hulls[i]->points); free(hulls[i]); }
    }
}

/* update position + rotation */
void updateBall(struct cube *ball, struct vec2 *velocity, struct vec3 *rotational_velocity) {
    ball->position.x += velocity->x;
    ball->position.y += velocity->y;
    ball->rotation.x += rotational_velocity->x;
    ball->rotation.y += rotational_velocity->y;
    ball->rotation.z += rotational_velocity->z;
    if (ball->rotation.x > 2 * M_PI) ball->rotation.x -= 2 * M_PI;
    if (ball->rotation.y > 2 * M_PI) ball->rotation.y -= 2 * M_PI;
    if (ball->rotation.z > 2 * M_PI) ball->rotation.z -= 2 * M_PI;
}

void rotate_vecs(struct vec3 *points, size_t size, struct vec3 rotation) {
    for (size_t i = 0; i < size; i++) {
        double x = points[i].x, y = points[i].y, z = points[i].z;
        double x1 = x;
        double y1 = y * cos(rotation.x) - z * sin(rotation.x);
        double z1 = y * sin(rotation.x) + z * cos(rotation.x);
        double x2 = x1 * cos(rotation.y) + z1 * sin(rotation.y);
        double y2 = y1;
        double z2 = -x1 * sin(rotation.y) + z1 * cos(rotation.y);
        double x3 = x2 * cos(rotation.z) - y2 * sin(rotation.z);
        double y3 = x2 * sin(rotation.z) + y2 * cos(rotation.z);
        double z3 = z2;
        points[i].x = x3; points[i].y = y3; points[i].z = z3;
    }
}

void project(struct vec3 *points, struct vec2 *projection, size_t size,
             struct vec2 position, double cube_size) {
    for (size_t i = 0; i < size; i++) {
        projection[i].x = (points[i].x * cube_size / 2.0) + position.x;
        projection[i].y = (points[i].y * cube_size / 2.0) + position.y;
    }
}

void updateFaces(struct cube *cube, struct vec2 projections[3][4]) {
    struct vec3 faces[6][4] = {
        {{1,1,1},{1,-1,1},{1,-1,-1},{1,1,-1}},
        {{-1,1,1},{-1,-1,1},{-1,-1,-1},{-1,1,-1}},
        {{1,1,1},{1,1,-1},{-1,1,-1},{-1,1,1}},
        {{1,-1,1},{1,-1,-1},{-1,-1,-1},{-1,-1,1}},
        {{1,1,1},{1,-1,1},{-1,-1,1},{-1,1,1}},
        {{1,1,-1},{1,-1,-1},{-1,-1,-1},{-1,1,-1}},
    };
    struct vec3 normals[6] = {
        {1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
    };

    // rotate normals (work on a temp copy to preserve originals)
    struct vec3 tmp_normals[6];
    memcpy(tmp_normals, normals, sizeof(normals));
    rotate_vecs(tmp_normals, 6, cube->rotation);

    int k = 0;
    for (int i = 0; i < 6; i++) {
        if (tmp_normals[i].z < 0) continue;
        // rotate face copy
        struct vec3 face_copy[4];
        for (int j = 0; j < 4; j++) face_copy[j] = faces[i][j];
        rotate_vecs(face_copy, 4, cube->rotation);
        project(face_copy, projections[k++], 4, cube->position, cube->size);
        if (k == 3) break;
    }
}

void updateColor() {
    color = (color + 1) % 5 + 31;
}

void handleEdgeCollision(struct vec2 faces[3][4], struct vec2 *velocity,
                         struct vec3 *rotational_velocity) {
    struct vec2 minBound, maxBound;
    getBounds((struct vec2 *)faces, 12, &minBound, &maxBound);
    if (minBound.x < 0 && velocity->x < 0) {
        velocity->x = fabs(velocity->x);
        rotational_velocity->y *= -1;
        updateColor();
    }
    if (maxBound.x > aspectRatio.x && velocity->x > 0) {
        velocity->x = -fabs(velocity->x);
        rotational_velocity->y *= -1;
        updateColor();
    }
    if (minBound.y < 0 && velocity->y < 0) {
        velocity->y = fabs(velocity->y);
        rotational_velocity->x *= -1;
        updateColor();
    }
    if (maxBound.y > aspectRatio.y && velocity->y > 0) {
        velocity->y = -fabs(velocity->y);
        rotational_velocity->x *= -1;
        updateColor();
    }
}

void updateAspectRatio(struct term_size *w) {
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

void getConsoleSize(struct term_size *t) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        t->ws_col = 80;
        t->ws_row = 24;
    } else {
        t->ws_col = ws.ws_col;
        t->ws_row = ws.ws_row;
    }
}

void msleep(unsigned int ms) { usleep(ms * 1000); }

void showWelcomeMessage() {
    const char *welcome[] = {
        " __        __   _                            _          ",
        " \\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___    ",
        "  \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\ ",
        "   \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) |   ",
        "    \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/ ",
        "",
        "                WELCOME TO THE APP                      ",
        "       FOOTBALL CLUB MANAGER code by Ali Cherraoui      ",
        "                         SAS 2025                       "
    };
    int lines = sizeof(welcome) / sizeof(welcome[0]);
    printf("\033[1m"); fflush(stdout);
    for (int i = 0; i < lines; i++) {
        printf("   %s\n", welcome[i]); fflush(stdout);
        msleep(150);
    }
    printf(ANSI_RESET);
    fflush(stdout);
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
        "                                               "
    };
    int lines = sizeof(goodbye) / sizeof(goodbye[0]);
    printf("\033[1m"); fflush(stdout);
    for (int i = 0; i < lines; i++) {
        printf("   %s\n", goodbye[i]); fflush(stdout);
        msleep(150);
    }
    printf(ANSI_RESET); fflush(stdout);
}

void full_run(int duration_seconds) {
    struct cube cube = {{0.1, 0.2, 0.3}, {0.5, 0.5}, 0.3};
    struct vec2 faces[3][4];
    char chars[3] = {'#','*','~'};
    struct vec2 velocity = {0.006, 0.004};
    struct vec3 rotational_velocity = {0.02, 0.02, 0.01};
    struct term_size w;
    int frames = duration_seconds * 25;

    for (int f = 0; f < frames; f++) {
        getConsoleSize(&w);
        updateAspectRatio(&w);
        updateFaces(&cube, faces);
        clearScreen();
        draw(faces, chars, &w);
        handleEdgeCollision(faces, &velocity, &rotational_velocity);
        updateBall(&cube, &velocity, &rotational_velocity);
        msleep(40); // ~25 FPS
    }

    msleep(500);
    clearScreen();
    showWelcomeMessage();
    msleep(1500);
    clearScreen();
    printf("\e[?25h"); fflush(stdout); // show cursor
}

void main_animation(int duration) {
    full_run(duration);
    showGoodbyeMessage();
    
}
