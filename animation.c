#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 40

int main() {
    float A = 0, B = 0;
    float z[WIDTH*HEIGHT];
    char b[WIDTH*HEIGHT];

    float playerX = -10, playerY = 0;
    float ballX = 20, ballY = 0;

    printf("\x1b[2J"); // clear screen

    while(1) {
        memset(b, ' ', WIDTH*HEIGHT);
        memset(z, 0, sizeof(z));

        // Joueur suit le ballon
        playerX += (ballX - playerX) * 0.1;
        playerY += (ballY - playerY) * 0.1;

        float legPhase = sin(A*6) * 0.5;
        float armPhase = cos(A*6) * 0.5;

        // Corps du joueur style 3D donut
        for(float i=-1; i<=1; i+=0.1){
            for(float j=-1; j<=1; j+=0.1){
                for(float k=-1; k<=1; k+=0.1){
                    float x = j;
                    float y = i + 0.3*sin(A*6 + j);
                    float zpos = k + 3 + 0.3*cos(A*6 + i);

                    x += playerX;
                    y += playerY;

                    float ooz = 1 / zpos;

                    int xp = (int)(WIDTH/2 + (WIDTH/4) * ooz * (x*cos(B) - y*sin(B)));
                    int yp = (int)(HEIGHT/2 + (HEIGHT/4) * ooz * (x*sin(B) + y*cos(B)));

                    int idx = xp + WIDTH*yp;
                    if(idx >=0 && idx<WIDTH*HEIGHT) {
                        if(ooz > z[idx]) {
                            z[idx] = ooz;
                            if(i>0.7) b[idx] = 'O';
                            else if(i>0.2) b[idx] = '|';
                            else if(i>-0.2) b[idx] = '|';
                            else if(i>-0.7) b[idx] = '/';
                            else b[idx] = '/';
                        }
                    }
                }
            }
        }

        // Ballon
        int ballIdx = (int)(WIDTH/2 + (WIDTH/4)*ballX/30) + WIDTH*(int)(HEIGHT/2 + (HEIGHT/4)*ballY/15);
        if(ballIdx>=0 && ballIdx<WIDTH*HEIGHT) b[ballIdx] = 'o';

        // Affichage
        printf("\x1b[H");
        for(int k=0; k<WIDTH*HEIGHT; k++)
            putchar(k%WIDTH ? b[k] : 10);

        A += 0.05;
        B += 0.03;
        usleep(25000);
    }
    return 0;
}
