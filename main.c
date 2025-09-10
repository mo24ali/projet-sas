#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"

void case1(Equipe equipe)
{
    int ajout;
    printf("1. Ajouter un joueur a la fois\n");
    printf("2. Ajouter plusieurs joueurs : \n");
    scanf("%d", &ajout);
    switch (ajout)
    {
    case 1:
        ajouter_un_nouveau_joueur(&equipe);
        break;
    case 2:
        ajouter_multiple_joueur(&equipe);
        break;
    }
}
void case2(Equipe equipe)
{
    int affichage;
    printf("1. afficher les joueurs par ordre simple \n");
    printf("2. afficher les joueurs par ordre alphabetique \n");
    printf("3. afficher le sjoueurs par ordre par age \n");
    scanf("%d", &affichage);
    switch (affichage)
    {
    case 1:
        afficher_equipe(equipe);
        break;
    case 2:
        trier_les_joueurs_alphabetiquement(&equipe);
        afficher_equipe(equipe);
        break;
    case 3:
        trier_les_joueurs_par_age(&equipe);
        afficher_equipe(equipe);
        break;
    }
}
void case3(Equipe equipe)
{
    int choix3;
    char pos[MAX_CHAR];
    printf("1. Modifier le poste de joueur : \n");
    printf("2. Modifier l'age de joueur : \n");
    printf("3. Modifier le nombre de buts marqués par le joueur : \n");
    scanf("%d", &choix3);
    switch (choix3)
    {
    case 1:
        int id1;
        char pos[MAX_CHAR];
        printf("entrez l'id du joueur : \n");
        scanf("%d", &id1);
        printf("Entrez la nouvelle position : \n");
        fgets(pos, sizeof(pos), stdin);
        modifier_un_joueur_poste(id1, pos, &equipe);
        break;
    case 2:
        int id2;
        int age;
        printf("entrez l'id du joueur : \n");
        scanf("%d", &id2);
        printf("Entrez le nouvel age : \n");
        scanf("%d", &age);
        modifier_un_joueur_age(id2, age, &equipe);
        break;
    case 3:
        int id3;
        int buts;
        printf("entrez l'id du joueur : \n");
        scanf("%d", &id3);
        printf("Entrez la nouvelle position : \n");
        scanf("%d", &buts);
        modifier_un_joueur_buts(id3, buts, &equipe);
        break;
    }
}

void case4(Equipe equipe)
{
    int id4;
    printf("entrez l'ID de joueur a supprimer : \n");
    scanf("%d", &id4);
    supprimer_joueur(&equipe, id4);
}
void case5(Equipe equipe)
{
    int choix5;
    printf("1. rechercher un joueur par ID : ");
    printf("2. rechercher un joueur par NOM : ");
    scanf("%d", &choix5);
    switch (choix5)
    {
    case 1:
        int id1_5;
        printf("entrez l'ID de joueur a supprimer : \n");
        scanf("%d", &id1_5);
        rechercher_un_joueur(id1_5, equipe);
        break;
    case 2:
        char nm[MAX_CHAR];
        printf("entrez le nom de joueur à rechercher : \n");
        fgets(nm, sizeof(nm), stdin);
        rechercher_un_joueur_par_nom(nm, equipe);
        break;
    default:
        printf("%s choix invalide !! %s\n", red, reset);
    }
}
void case6(Equipe equipe)
{
    int choix6;
    afficher_menu_statistiques();
    scanf("%d", &choix6);
    switch (choix6)
    {
    case 1:
        // Afficher le nombre total de joueurs dans l’équipe.
        printf("le nombre total des joueurs dans l'equipe : %d", equipe.effective);
        break;
    case 2:
        // Afficher l’âge moyen des joueurs.
        float moy = moyenne(equipe);
        break;
    case 3:
        int x;
        // Afficher les joueurs ayant marqué plus de X buts (X introduit par l’utilisateur).
        printf("entrer le nombre des buts X : \n");
        scanf("%d", &x);
        afficher_joueurs_marque_plus_X_buts(equipe, x);
        break;
    case 4:
        // Afficher le meilleur buteur (joueur avec le maximum de buts).
        afficher_meilleur_buteur(equipe);
        break;
    case 5:
        // Afficher le joueur le plus jeune et le plus âgé.
        afficher_le_joueur_le_plus_jeune_et_le_plus_age(equipe);
        break;
    default:
        printf("%s choix invalide !! %s\n", red, reset);
    }
}
int main_1()
{
    Equipe equipe = init_equipe(10);
    Joueur joueurs[10] = {
        {1, 25, 10, 7, {10, 9, 2023}, "Dupont", "Jean", "attaquant", "titulaire"},
        {2, 22, 5, 4, {5, 7, 2022}, "Martin", "Paul", "milieu", "remplacant"},
        {3, 28, 8, 1, {12, 8, 2021}, "Bernard", "Luc", "gardien", "titulaire"},
        {4, 24, 12, 3, {3, 4, 2020}, "Petit", "Marc", "defenseur", "titulaire"},
        {5, 30, 15, 9, {15, 6, 2023}, "Robert", "Louis", "attaquant", "remplacant"},
        {6, 27, 7, 5, {21, 5, 2022}, "Richard", "Alain", "milieu", "titulaire"},
        {7, 23, 4, 8, {30, 11, 2021}, "Durand", "Michel", "defenseur", "remplacant"},
        {8, 26, 3, 10, {1, 1, 2020}, "Leroy", "Henri", "attaquant", "titulaire"},
        {9, 21, 6, 2, {9, 3, 2023}, "Moreau", "Jacques", "gardien", "remplacant"},
        {10, 29, 9, 6, {25, 12, 2022}, "Simon", "Alain", "milieu", "titulaire"}
    };
    for(int i = 0 ; i < 10 ; i++){
        equipe.joueur[i] = joueurs[i];
    }
    int choix;

    do
    {
        printf("\t");
        printf("%s************** MENU *****************%s\n", cyan, reset);
        printf("\t");
        printf("%s*       1 .  Ajouter Joueur         *%s\n", blue, reset);
        printf("\t");
        printf("%s*       2 .  Afficher Joueur        *%s\n", yellow, reset);
        printf("\t");
        printf("%s*       3 .  Modifier Joueur       *%s\n", blue, reset);
        printf("\t");
        printf("%s*       4 .  Supprimer Joueur        *%s\n", yellow, reset);
        printf("\t");
        printf("%s*       5 .  Rechercher Joueur      *%s\n", blue, reset);
        printf("\t");
        printf("%s*       6 .  Statistiques           *%s\n", yellow, reset);
        printf("\t");
        printf("%s*       7 .  Quitter                *%s\n", blue, reset);
        printf("\t");
        printf("%s*       Choisir une option :        *%s\n", cyan, reset);
        printf("\t");
        printf("%s*************************************%s\n", cyan, reset);
        scanf("%d", &choix);
        switch (choix)
        {
        case 1:
            case1(equipe);
            break;
        case 2:
            case2(equipe);
            break;
        case 3:
            case3(equipe);
            break;
        case 4:
            case4(equipe);
            break;
        case 5:
            case5(equipe);
            break;
        case 6:
            case6(equipe);
            break;
        case 7:
            exit(0);
            break;
        default:
            printf("%schoix invalide !!!!\n%s", red, reset);
            break;
        }
    } while (choix >= 1 || choix <= 6);
    return 0;
}
int main(){

    return 0;
}