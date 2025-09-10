#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "source.h"
int main(){
    int i = 0;
    //Date dt = init_date_1(10,9,2025);
    Joueur j = init_joueur(i,"3li","da",24,"attaquant",22,11,init_date_1(10,9,2025),"titulaire");
    //Equipe eq = init_equipe(0);
    show_player(j);
    return 0;
}