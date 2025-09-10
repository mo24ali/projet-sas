#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"
int main()

{
    int choix;

    do
    {
        printf("\t");
        printf("%s************** MENU *****************%s\n", cyan, reset);
        printf("\t");
        printf("%s*       1 .  Ajouter Joueur        *%s\n", blue, reset);
        printf("\t");
        printf("%s*       2 .  Modifier Joueur       *%s\n", yellow, reset);
        printf("\t");
        printf("%s*       3 .  Supprimer Joueur      *%s\n", blue, reset);
        printf("\t");
        printf("%s*       4 .  Afficher Joueur       *%s\n", yellow, reset);
        printf("\t");
        printf("%s*       5 .  Rechercher Joueur     *%s\n", blue, reset);
        printf("\t");
        printf("%s*       6 .  Statistiques                *%s\n", yellow, reset);
        printf("\t");
        printf("%s*       7 .  Quitter                *%s\n", yellow, reset);
        printf("\t");
        printf("%s*       Choisir une option :        *%s\n", cyan, reset);
        printf("\t");
        printf("%s*************************************%s\n", cyan, reset);
        scanf("%d", &choix);
        switch (choix)
        {
        case 1:
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
            break;
        case 3:
            /**
             *

            Modifier un joueur :
            Modifier le poste d’un joueur.
            Modifier l’âge d’un joueur.
            Modifier le nombre de buts marqués par un joueur.



             */
            break;
        case 4:
            /**
             * Supprimer un joueur :
            Supprimer un joueur par identifiant.

             */
            break;
        case 5:
            /**
             * Rechercher un joueur :
            Rechercher un joueur par Identifiant.
            Rechercher un joueur par Nom.

             */
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