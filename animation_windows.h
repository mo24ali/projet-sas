#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


#define M_PI 3.14159265358979323846

struct vec3 {
    double x;
    double y;
    double z;
};

struct vec2 {
    double x;
    double y;
};

struct cube {
    struct vec3 rotation;
    struct vec2 position;
    double size;
};

struct convexHull {
    struct vec2 *points;
    int size;
};

struct winsize {
    int ws_row;
    int ws_col;
};

void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void clearScreen() {
    printf("\033[H\033[J");
}

int color = 31;
struct vec2 aspectRatio = {1, 1};

double crossProduct(struct vec2 A, struct vec2 B, struct vec2 C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

void sortPoints(struct vec2 *points, size_t size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (points[j].x < points[i].x ||
                (points[j].x == points[i].x && points[j].y < points[i].y)) {
                struct vec2 temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
        }
    }
}

struct convexHull *computeConvexHull(struct vec2 *points, size_t size) {
    sortPoints(points, size);
    struct vec2 *hull = malloc(sizeof(struct vec2) * size * 2);
    int k = 0;
    for (int i = 0; i < size; i++) {
        while (k >= 2 && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }
    for (int i = size - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }
    struct convexHull *ch = malloc(sizeof(struct convexHull));
    ch->points = hull;
    ch->size = k;
    return ch;
}

int isInsideConvexHull(struct vec2 P, struct convexHull *ch) {
    for (int i = 1; i < ch->size; i++) {
        double cross = crossProduct(ch->points[i - 1], ch->points[i], P);
        if (cross < 0) {
            return 0;
        }
    }
    return 1;
}

void getBounds(struct vec2 *points, size_t size, struct vec2 *minBound, struct vec2 *maxBound) {
    *minBound = points[0];
    *maxBound = points[0];
    for (size_t i = 1; i < size; i++) {
        if (points[i].x < minBound->x) minBound->x = points[i].x;
        if (points[i].y < minBound->y) minBound->y = points[i].y;
        if (points[i].x > maxBound->x) maxBound->x = points[i].x;
        if (points[i].y > maxBound->y) maxBound->y = points[i].y;
    }
}

void draw(struct vec2 faces[3][4], char chars[], struct winsize *w) {
    struct vec2 minBound, maxBound;
    getBounds((struct vec2 *)faces, 12, &minBound, &maxBound);
    struct convexHull *hulls[3];
    for (int i = 0; i < 3; i++) {
        hulls[i] = computeConvexHull(faces[i], 4);
    }

    int x0 = floor(minBound.x / aspectRatio.x * w->ws_col);
    int y0 = floor(minBound.y / aspectRatio.y * w->ws_row);
    int size_x = ceil(maxBound.x / aspectRatio.x * w->ws_col);
    int size_y = ceil(maxBound.y / aspectRatio.y * w->ws_row);

    if (size_x > w->ws_col) size_x = w->ws_col;
    if (size_y > w->ws_row) size_y = w->ws_row;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;

    char output[size_y][size_x][7];

    for (int i = y0; i < size_y; i++) {
        for (int j = x0; j < size_x; j++) {
            struct vec2 P = {(double)j * aspectRatio.x / w->ws_col,
                             (double)i * aspectRatio.y / w->ws_row};
            int inside = 0;
            for (int c = 0; c < 3; c++) {
                if (isInsideConvexHull(P, hulls[c])) {
                    inside = 1;
                    break;
                }
            }
            if (inside) {
                int pattern = ((int)(P.x * 20) + (int)(P.y * 10)) % 6;
                if (pattern == 0 || pattern == 3) {
                    sprintf(output[i - y0][j - x0], "\033[30m@");
                } else {
                    sprintf(output[i - y0][j - x0], "\033[37mO");
                }
            } else {
                sprintf(output[i - y0][j - x0], " ");
            }
        }
    }

    printf("\e[?25l\033[1m");
    for (int i = 0; i < (size_y - y0); i++) {
        printf("\033[%d;%dH", i + y0, x0);
        for (int j = 0; j < (size_x - x0); j++) {
            printf("%s", output[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < 3; i++) {
        free(hulls[i]->points);
        free(hulls[i]);
    }
}

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

void rotate(struct vec3 *points, size_t size, struct vec3 rotation) {
    for (int i = 0; i < size; i++) {
        double x = points[i].x;
        double y = points[i].y;
        double z = points[i].z;
        double x1 = x;
        double y1 = y * cos(rotation.x) - z * sin(rotation.x);
        double z1 = y * sin(rotation.x) + z * cos(rotation.x);
        double x2 = x1 * cos(rotation.y) + z1 * sin(rotation.y);
        double y2 = y1;
        double z2 = -x1 * sin(rotation.y) + z1 * cos(rotation.y);
        double x3 = x2 * cos(rotation.z) - y2 * sin(rotation.z);
        double y3 = x2 * sin(rotation.z) + y2 * cos(rotation.z);
        double z3 = z2;
        points[i].x = x3;
        points[i].y = y3;
        points[i].z = z3;
    }
}

void project(struct vec3 *points, struct vec2 *projection, size_t size,
             struct vec2 position, double cube_size) {
    for (int i = 0; i < size; i++) {
        projection[i].x = (points[i].x * cube_size / 2) + position.x;
        projection[i].y = (points[i].y * cube_size / 2) + position.y;
    }
}

void updateFaces(struct cube *cube, struct vec2 projections[3][4]) {
    struct vec3 faces[6][4] = {
        {{1, 1, 1}, {1, -1, 1}, {1, -1, -1}, {1, 1, -1}},
        {{-1, 1, 1}, {-1, -1, 1}, {-1, -1, -1}, {-1, 1, -1}},
        {{1, 1, 1}, {1, 1, -1}, {-1, 1, -1}, {-1, 1, 1}},
        {{1, -1, 1}, {1, -1, -1}, {-1, -1, -1}, {-1, -1, 1}},
        {{1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, {-1, 1, 1}},
        {{1, 1, -1}, {1, -1, -1}, {-1, -1, -1}, {-1, 1, -1}},
    };

    struct vec3 normals[6] = {
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
        {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
    };

    rotate(normals, 6, cube->rotation);
    int k = 0;
    for (int i = 0; i < 6; i++) {
        if (normals[i].z < 0) continue;
        rotate(faces[i], 4, cube->rotation);
        project(faces[i], projections[k++], 4, cube->position, cube->size);
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

void updateAspectRatio(struct winsize *w) {
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

void getConsoleSize(struct winsize *w) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    w->ws_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    w->ws_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
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
        "      SAS 2025                   "
    };

    int lines = sizeof(welcome) / sizeof(welcome[0]);

    printf("\033[1m"); // Enable bold
    
    for (int i = 0; i < lines; i++) {
        printf("   %s\n", welcome[i]);
        Sleep(150);  // small delay between lines for fade-in effect
    }

    printf("%s",ANSI_RESET); // Reset formatting
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

    printf("\033[1m"); // Enable bold
    
    for (int i = 0; i < lines; i++) {
        printf("   %s\n", goodbye[i]);
        Sleep(150);  // small delay between lines for fade-in effect
    }

    printf("%s",ANSI_RESET); // Reset formatting
}


void full_run(int duration_seconds) {
    struct cube cube = {{0.1, 0.2, 0.3}, {0.5, 0.5}, 0.3};
    struct vec2 faces[3][4];
    char chars[3] = {'#', '*', '~'};
    struct vec2 velocity = {0.01, 0.01};
    struct vec3 rotational_velocity = {0.05, 0.05, 0};
    struct winsize w;
    int frames = duration_seconds * 25;

    enableVirtualTerminalProcessing();

    for (int f = 0; f < frames; f++) {
        getConsoleSize(&w);
        updateAspectRatio(&w);
        updateFaces(&cube, faces);
        clearScreen();
        draw(faces, chars, &w);
        handleEdgeCollision(faces, &velocity, &rotational_velocity);
        updateBall(&cube, &velocity, &rotational_velocity);
        Sleep(15); 
    }

    Sleep(500);      
    clearScreen();   
    showWelcomeMessage();

    Sleep(4000);      
    clearScreen();    

    printf("\e[?25h"); 
}
