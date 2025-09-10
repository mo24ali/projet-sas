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
int main()
{
    Equipe equipe = init_equipe(0);
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
            /**Ajouter un joueur : */
            /**
             * Ajouter un nouveau joueur.
                Ajouter plusieurs joueurs en une seule opération.
             */

            break;
        case 2:
            /**
             * Afficher la liste de tous les joueurs :
                Trier les joueurs par ordre alphabétique (Nom).
                Trier les joueurs par âge.
                Afficher les joueurs par poste.

             */
            case2(equipe);
            break;
        case 3:
            /**
            Modifier un joueur :
            Modifier le poste d’un joueur.
            Modifier l’âge d’un joueur.
            Modifier le nombre de buts marqués par un joueur.
             */
            case3(equipe);
            break;
        case 4:
            /**
             * Supprimer un joueur :
            Supprimer un joueur par identifiant.
             */

            case4(equipe);
            break;
        case 5:
            /**
             * Rechercher un joueur :
            Rechercher un joueur par Identifiant.
            Rechercher un joueur par Nom.

             */
            case5(equipe);
            break;
        case 6:
            /**
             * Statistiques :
                Afficher le nombre total de joueurs dans l’équipe.
                Afficher l’âge moyen des joueurs.
                Afficher les joueurs ayant marqué plus de X buts (X introduit par l’utilisateur).
                Afficher le meilleur buteur (joueur avec le maximum de buts).
                Afficher le joueur le plus jeune et le plus âgé.



             */
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