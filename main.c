#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"
//#include "animation_windows.h" // uncomment this and comment animation.h for windows usage
#include "animation.h"
void case1(Equipe *equipe)
{
    int ajout;
    do
    {
        printf("\t");
        printf("\t");
        printf("\t");
        printf("1. Ajouter un joueur a la fois\n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("2. Ajouter plusieurs joueurs : \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("%s ==> entrez 0 pour retourner au menu principal <== %s\n", green, ANSI_RESET);
        scanf("%d", &ajout);
        getchar();
        switch (ajout)
        {
        case 0:
            clear_screen();
            break;
        case 1:
            ajouter_un_nouveau_joueur(equipe);
            break;
        case 2:
            ajouter_multiple_joueur(equipe);
            break;
        default:
            printf("\t");
            printf("%sChoix invalide !%s\n", red, ANSI_RESET);
            break;
        }
    } while (ajout != 0);
}
void case2(Equipe *equipe)
{

    int affichage;
    do
    {

        printf("\t");
        printf("\t");
        printf("\t");
        printf("1. afficher les joueurs par ordre simple \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("2. afficher les joueurs par ordre poste \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("3. afficher les joueurs par ordre alphabetique \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("4. afficher les joueurs par ordre par age \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("%s ==> entrez 0 pour retourner au menu principal <== %s\n", green, ANSI_RESET);
        scanf("%d", &affichage);
        getchar();
        switch (affichage)
        {
        case 0:
            clear_screen();
            break;
        case 1:
            afficher_les_joueurs_par_id(*equipe);
            break;
        case 2:
            // afficher_equipe(*equipe);
            afficher_les_joueurs_par_postes(*equipe);
            break;
        case 3:
            trier_les_joueurs_alphabetiquement(equipe);
            afficher_equipe(*equipe);
            break;
        case 4:
            trier_les_joueurs_par_age(equipe);
            afficher_equipe(*equipe);
            break;

        default:
            printf("\t");
            printf("%sChoix invalide !%s\n", red, ANSI_RESET);
        }
    } while (affichage != 0);
}

void case3(Equipe *equipe)
{
    int choix3;
    do
    {

        printf("\t");
        printf("\t");
        printf("\t");
        printf("1. Modifier le poste de joueur : \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("2. Modifier l'age de joueur : \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("3. Modifier le nombre de buts marques par le joueur : \n");
        printf("\t");
        printf("\t");
        printf("\t");
        printf("%s ==> entrez 0 pour retourner au menu principal <== %s\n", green, ANSI_RESET);
        scanf("%d", &choix3);
        getchar();

        int id;
        switch (choix3)
        {
        case 0:
            clear_screen();
            break;
        case 1:
        {
            int id;
            printf("\tEntrez l'ID du joueur : ");
            if (scanf("%d", &id) != 1)
            { // Vérifier la saisie
                printf("%sEntrée invalide.%s\n", red, ANSI_RESET);
                while (getchar() != '\n')
                    ; 
                break;
            }
            while (getchar() != '\n')
                ; 

            modifier_un_joueur_poste(id, equipe);
            break;
        }
        case 2:
        {
            int age;
            printf("\t");
            printf("entrez l'id du joueur : \n");
            scanf("%d", &id);
            printf("\t");
            printf("Entrez le nouvel age : \n");
            scanf("%d", &age);
            modifier_un_joueur_age(id, age, equipe);
            break;
        }
        case 3:
        {
            int buts;
            printf("\t");
            printf("entrez l'id du joueur : \n");
            scanf("%d", &id);
            printf("\t");
            printf("Entrez le nouveau nombre de buts : \n");
            scanf("%d", &buts);
            modifier_un_joueur_buts(id, buts, equipe);
            break;
        }
        default:
            printf("\t");
            printf("%sChoix invalide !%s\n", red, ANSI_RESET);
        }
    } while (choix3 != 0);
}

void case4(Equipe *equipe)
{
    int id4;

    printf("\t");
    printf("entrez l'ID du joueur a supprimer : \n");
    printf("\t");
    printf("%s ==> entrez 0 pour retourner au menu principal <== %s\n", green, ANSI_RESET);
    scanf("%d", &id4);

    if (id4 == 0)
    {
        clear_screen();
        return;
    }
    else
    {
        supprimer_joueur(equipe, id4);
    }
}

void case5(Equipe *equipe)
{
    int choix5;
    do
    {
        printf("\t");
        printf("1. rechercher un joueur par ID : \n");
        printf("\t");
        printf("2. rechercher un joueur par NOM : \n");
        printf("\t");
        printf("3. rechercher un joueur par AGE : \n");
        printf("\t");
        printf("4. rechercher un joueur par TRANCHE D'AGE : \n");
        printf("\t");
        printf("5. rechercher un joueur par POSTE : \n");
        printf("\t");
        printf("%s ==> entrez 0 pour retourner au menu principal <== %s\n", green, ANSI_RESET);
        scanf("%d", &choix5);
        getchar();

        switch (choix5)
        {
        case 0:
            clear_screen();
            break;
        case 1:
        {
            int id1_5;
            printf("\t");
            printf("entrez l'ID du joueur a rechercher : \n");
            scanf("%d", &id1_5);
            rechercher_un_joueur(id1_5, *equipe);
            break;
        }
        case 2:
        {
            char nm[MAX_CHAR];
            printf("\t");
            printf("entrez le nom du joueur a rechercher : \n");
            fgets(nm, sizeof(nm), stdin);
            nm[strcspn(nm, "\n")] = 0;
            rechercher_un_joueur_par_nom(nm, *equipe);
            break;
        }
        case 3:
        {
            int age;
            printf("entrez l'age");
            scanf("%d", &age);
            rechercher_joueurs_par_age(equipe, age);
            break;
        }
        case 4:
        {
            rechercher_joueurs_par_tranche_d_age(equipe);
            break;
        }
        case 5:
        {
            char tmp[MAX_CHAR];
            lire_chaine("entrez le poste que vous chercher : \n", tmp, sizeof(tmp));
            rechercher_joueurs_par_poste(equipe, tmp);
            break;
        }
        default:
            printf("\t");
            printf("%sChoix invalide !%s\n", red, ANSI_RESET);
        }
    } while (choix5 != 0);
}

void case6(Equipe *equipe)
{
    int choix6;
    do
    {
        afficher_menu_statistiques();
        if (scanf("%d", &choix6) != 1) {
            // si entrée invalide (lettre, etc.)
            printf("⚠️ Entree invalide, veuillez entrer un nombre !\n");
            while (getchar() != '\n'); // vider buffer
            choix6 = -1; // force à continuer
            continue;
        }

        switch (choix6)
        {
        case 0:
            clear_screen();
            break;
        case 1:
            printf("\tLe nombre total des joueurs dans l'equipe : %d\n", equipe->effective);
            break;
        case 2:
            printf("\tL'age moyen des joueurs : %.2f\n", moyenne(*equipe));
            break;
        case 3:
        {
            int x;
            printf("\tEntrer le nombre des buts X : \n");
            if (scanf("%d", &x) == 1) {
                afficher_joueurs_marque_plus_X_buts(*equipe, x);
            } else {
                printf("⚠️ Entree invalide pour X !\n");
                while (getchar() != '\n'); // vider buffer
            }
            break;
        }
        case 4:
            afficher_meilleur_buteur(*equipe);
            break;
        case 5:
            afficher_le_joueur_le_plus_jeune_et_le_plus_age(*equipe);
            break;
        default:
            printf("\t%sChoix invalide !%s\n", red, ANSI_RESET);
        }

    } while (choix6 != 0); // sortir avec 0
}



int lire_entier() {
    int choix;
    char c;
    while (1) {
        if (scanf("%d%c", &choix, &c) != 2 || c != '\n') {
            printf("\tEntree invalide, veuillez entrer un nombre !\n");
            //vider_buffer();
            while ((c = getchar()) != '\n' && c != EOF);
            //getchar();
        } else {
            return choix;
        }
    }
}
int main()
{   full_run(5);
    clear_screen();
    Equipe equipe = init_equipe(10);

    Joueur joueurs_init[10] = {
        init_joueur(1, "Messi", "Lionel", 10, "attaquant", 25, 73, init_date_1(1, 7, 2004), "TITULAIRE "),
        init_joueur(2, "Xavi", "Hernandez", 6, "milieu", 32, 14, init_date_1(1, 7, 1998), "TITULAIRE "),
        init_joueur(3, "Iniesta", "Andres", 8, "milieu", 28, 8, init_date_1(1, 7, 2002), "TITULAIRE "),
        init_joueur(4, "Puyol", "Carles", 5, "defenseur", 34, 2, init_date_1(1, 7, 1999), "TITULAIRE "),
        init_joueur(5, "Pique", "Gerard", 3, "defenseur", 25, 5, init_date_1(1, 7, 2008), "TITULAIRE "),
        init_joueur(6, "Alves", "Dani", 2, "defenseur", 29, 6, init_date_1(1, 7, 2008), "TITULAIRE "),
        init_joueur(7, "Busquets", "Sergio", 16, "milieu", 24, 3, init_date_1(1, 7, 2008), "TITULAIRE "),
        init_joueur(8, "Pedro", "Rodriguez", 17, "attaquant", 25, 13, init_date_1(1, 7, 2008), "REMPLACANT"),
        init_joueur(9, "Villa", "David", 7, "attaquant", 30, 9, init_date_1(1, 7, 2010), "REMPLACANT"),
        init_joueur(10, "Valdes", "Victor", 1, "gardien", 30, 0, init_date_1(1, 7, 2002), "TITULAIRE ")};

    for (int i = 0; i < 10; i++)
    {
        equipe.joueur[equipe.effective++] = joueurs_init[i];
    }

    // charger_joueurs("joueur.txt",equipe.joueur,equipe.capacite);
    int choix;
    do
    {
        printf("\t");
        printf("\t%s************** MENU ************************%s\n", cyan, ANSI_RESET);
        printf("\t");
        printf("\t%s*       1 .  Ajouter Joueur                *%s\n", blue, ANSI_RESET);
        printf("\t");
        printf("\t%s*       2 .  Afficher Joueur               *%s\n", yellow, ANSI_RESET);
        printf("\t");
        printf("\t%s*       3 .  Modifier Joueur               *%s\n", blue, ANSI_RESET);
        printf("\t");
        printf("\t%s*       4 .  Supprimer Joueur              *%s\n", yellow, ANSI_RESET);
        printf("\t");
        printf("\t%s*       5 .  Rechercher Joueur             *%s\n", blue, ANSI_RESET);
        printf("\t");
        printf("\t%s*       6 .  Statistiques                  *%s\n", yellow, ANSI_RESET);
        printf("\t");
        printf("\t%s*       7 .  Vider le terminal             *%s\n", blue, ANSI_RESET);
        printf("\t");
        printf("\t%s*       8 .  Quitter                       *%s\n", yellow, ANSI_RESET);
        printf("\t");
        printf("\t%s*       Choisir une option :               *%s\n", cyan, ANSI_RESET);
        printf("\t");
        printf("\t%s********************************************%s\n", cyan, ANSI_RESET);
        
        
        choix = lire_entier();
        

        switch (choix)
        {
        case 0:
            clear_screen();
            break;
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
            clear_screen();
            break;
        case 8:
            printf("\t");
            showGoodbyeMessage();
            Sleep(1000);
            system("clear");
            exit(0);
        default:
            printf("\t");
            printf("%sChoix invalide !!!!%s\n", red, ANSI_RESET);
        }

    } while (1);
    return 0;
}
