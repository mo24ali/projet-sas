#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 40

int main() {
    float A = 0, B = 0; // angles pour rotation
    int k;
    float z[WIDTH * HEIGHT];
    char b[WIDTH * HEIGHT];

    // Position du ballon
    float ballX = WIDTH/2 + 20;
    float ballY = HEIGHT/2;

    printf("\x1b[2J"); // clear screen

    while(1) {
        memset(b, ' ', WIDTH*HEIGHT);
        memset(z, 0, sizeof(z));

        float legPhase = sin(A*6) * 0.5;  // oscillation jambes
        float armPhase = cos(A*6) * 0.5;  // oscillation bras

        // Corps humanoïde
        for(float i=-1; i<=1; i+=0.05){       // hauteur
            for(float j=0; j<6.28; j+=0.1){   // rotation
                // Coordonnées de base
                float x = i;  
                float y = 0.3 * sin(j*2 + armPhase);  // bras
                float zpos = 3 + 0.3 * cos(j*2 + legPhase); // jambes

                float ooz = 1 / zpos;
                int xp = (int)(WIDTH/2 + 20 + 30 * ooz * (x*cos(B) - y*sin(B)));
                int yp = (int)(HEIGHT/2 + 15 * ooz * (x*sin(B) + y*cos(B)));
                int idx = xp + WIDTH*yp;

                if(idx >= 0 && idx < WIDTH*HEIGHT){
                    if(ooz > z[idx]){
                        z[idx] = ooz;
                        // dessiner différentes parties
                        if(i > 0.7) b[idx] = 'O';       // tête
                        else if(i > 0.2) b[idx] = '|';  // haut du tronc
                        else if(i > -0.2) b[idx] = '|'; // bas du tronc
                        else if(i > -0.7) b[idx] = '/'; // jambes
                        else b[idx] = '/';               // pieds
                    }
                }
            }
        }

        // Afficher le ballon
        int ballIdx = (int)ballX + WIDTH*(int)ballY;
        if(ballIdx >=0 && ballIdx < WIDTH*HEIGHT) b[ballIdx] = 'o';

        // Affichage
        printf("\x1b[H");
        for(k=0; k<WIDTH*HEIGHT; k++){
            putchar(k%WIDTH ? b[k] : 10);
        }

        // Incrément rotation
        A += 0.05;
        B += 0.03;
        usleep(25000);
    }

    return 0;
}
