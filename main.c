#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"
#include "animation.h"

void case1(Equipe *equipe)
{
    int ajout;
    printf("1. Ajouter un joueur a la fois\n");
    printf("2. Ajouter plusieurs joueurs : \n");
    scanf("%d", &ajout);
    getchar(); // pour consommer le '\n' résiduel
    switch (ajout)
    {
    case 1:
        ajouter_un_nouveau_joueur(equipe);
        break;
    case 2:
        ajouter_multiple_joueur(equipe);
        break;
    default:
        printf("%sChoix invalide !%s\n", red, reset);
    }
}

void case2(Equipe *equipe)
{   

    int affichage;
    printf("1. afficher les joueurs par ordre simple \n");
    printf("2. afficher les joueurs par ordre alphabetique \n");
    printf("3. afficher les joueurs par ordre par age \n");
    scanf("%d", &affichage);
    getchar();
    switch (affichage)
    {
    case 1:
        afficher_equipe(*equipe);
        break;
    case 2:
        trier_les_joueurs_alphabetiquement(equipe);
        afficher_equipe(*equipe);
        break;
    case 3:
        trier_les_joueurs_par_age(equipe);
        afficher_equipe(*equipe);
        break;
    default:
        printf("%sChoix invalide !%s\n", red, reset);
    }
}

void case3(Equipe *equipe)
{
    int choix3;
    printf("1. Modifier le poste de joueur : \n");
    printf("2. Modifier l'age de joueur : \n");
    printf("3. Modifier le nombre de buts marqués par le joueur : \n");
    scanf("%d", &choix3);
    getchar();

    int id;
    switch (choix3)
    {
    case 1:
    {
        char pos[MAX_CHAR];
        printf("entrez l'id du joueur : \n");
        scanf("%d", &id);
        getchar();
        printf("Entrez la nouvelle position : \n");
        fgets(pos, sizeof(pos), stdin);
        pos[strcspn(pos, "\n")] = 0; // enlever le '\n'
        modifier_un_joueur_poste(id, pos, equipe);
        break;
    }
    case 2:
    {
        int age;
        printf("entrez l'id du joueur : \n");
        scanf("%d", &id);
        printf("Entrez le nouvel age : \n");
        scanf("%d", &age);
        modifier_un_joueur_age(id, age, equipe);
        break;
    }
    case 3:
    {
        int buts;
        printf("entrez l'id du joueur : \n");
        scanf("%d", &id);
        printf("Entrez le nouveau nombre de buts : \n");
        scanf("%d", &buts);
        modifier_un_joueur_buts(id, buts, equipe);
        break;
    }
    default:
        printf("%sChoix invalide !%s\n", red, reset);
    }
}

void case4(Equipe *equipe)
{
    int id4;
    printf("entrez l'ID du joueur a supprimer : \n");
    scanf("%d", &id4);
    supprimer_joueur(equipe, id4);
}

void case5(Equipe *equipe)
{
    int choix5;
    printf("1. rechercher un joueur par ID : \n");
    printf("2. rechercher un joueur par NOM : \n");
    scanf("%d", &choix5);
    getchar();

    switch (choix5)
    {
    case 1:
    {
        int id1_5;
        printf("entrez l'ID du joueur à rechercher : \n");
        scanf("%d", &id1_5);
        rechercher_un_joueur(id1_5, *equipe);
        break;
    }
    case 2:
    {
        char nm[MAX_CHAR];
        printf("entrez le nom du joueur à rechercher : \n");
        fgets(nm, sizeof(nm), stdin);
        nm[strcspn(nm, "\n")] = 0;
        rechercher_un_joueur_par_nom(nm, *equipe);
        break;
    }
    default:
        printf("%sChoix invalide !%s\n", red, reset);
    }
}

void case6(Equipe *equipe)
{
    int choix6;
    afficher_menu_statistiques();
    scanf("%d", &choix6);
    getchar();

    switch (choix6)
    {
    case 1:
        printf("Le nombre total des joueurs dans l'equipe : %d\n", equipe->effective);
        break;
    case 2:
        printf("L'age moyen des joueurs : %.2f\n", moyenne(*equipe));
        break;
    case 3:
    {
        int x;
        printf("Entrer le nombre des buts X : \n");
        scanf("%d", &x);
        afficher_joueurs_marque_plus_X_buts(*equipe, x);
        break;
    }
    case 4:
        afficher_meilleur_buteur(*equipe);
        break;
    case 5:
        afficher_le_joueur_le_plus_jeune_et_le_plus_age(*equipe);
        break;
    default:
        printf("%sChoix invalide !%s\n", red, reset);
    }
}
void clear_screen() {
    #ifdef _WIN32
        system("cls"); 
    #else
        system("clear");
    #endif
}
// ====== MAIN ======
int main()
{   full_run(5);
    // Initialiser l'équipe avec capacité de 10 joueurs
    Equipe equipe = init_equipe(10);

    // Créer quelques joueurs initiaux
    Joueur joueurs_init[5] = {
        init_joueur(1, "Dupont", "Jean", 7, "attaquant", 25, 10, init_date_1(10,9,2023), "titulaire"),
        init_joueur(2, "Martin", "Paul", 4, "milieu", 22, 5, init_date_1(5,7,2022), "remplacant"),
        init_joueur(3, "Bernard", "Luc", 1, "gardien", 28, 8, init_date_1(12,8,2021), "titulaire"),
        init_joueur(4, "Petit", "Marc", 3, "defenseur", 24, 12, init_date_1(3,4,2020), "titulaire"),
        init_joueur(5, "Robert", "Louis", 9, "attaquant", 30, 15, init_date_1(15,6,2023), "remplacant")
    };

    // Ajouter ces joueurs à l'équipe
    for(int i = 0; i < 5; i++){
        equipe.joueur[equipe.effective++] = joueurs_init[i];
    }

    int choix;
    do
    {
        printf("\t%s************** MENU *****************%s\n", cyan, reset);
        printf("\t%s*       1 .  Ajouter Joueur         *%s\n", blue, reset);
        printf("\t%s*       2 .  Afficher Joueur        *%s\n", yellow, reset);
        printf("\t%s*       3 .  Modifier Joueur        *%s\n", blue, reset);
        printf("\t%s*       4 .  Supprimer Joueur       *%s\n", yellow, reset);
        printf("\t%s*       5 .  Rechercher Joueur      *%s\n", blue, reset);
        printf("\t%s*       6 .  Statistiques           *%s\n", yellow, reset);
        printf("\t%s*       7 .  Quitter                *%s\n", blue, reset);
        printf("\t%s*       8 .  Vider le terminal      *%s\n", yellow, reset);
        printf("\t%s*       Choisir une option :        *%s\n", cyan, reset);
        printf("\t%s*************************************%s\n", cyan, reset);

        scanf("%d", &choix);
        getchar();

        switch (choix)
        {
        case 1:
            case1(&equipe);
            break;
        case 2:
            case2(&equipe);
            break;
        case 3:
            case3(&equipe);
            break;
        case 4:
            case4(&equipe);
            break;
        case 5:
            case5(&equipe);
            break;
        case 6:
            case6(&equipe);
            break;
        case 7:
            printf("Au revoir !\n");
            exit(0);
        case 8:
            clear_screen();
            break;
        default:
            printf("%sChoix invalide !!!!%s\n", red, reset);
        }
        
    } while (1);
    //clear_screen();
    return 0;
}
