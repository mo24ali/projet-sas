#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_CHAR 50

/**
 * @brief
 *

Un joueur est défini par :
Un id unique: généré automatiquement pour identifier chaque joueur (évite les doublons).
Son nom et prenom : strings pour stocker l’identité.
numeroMaillot : un entier pour identifier le joueur sur le terrain.
poste : permet de distinguer les rôles (gardien, défenseur, milieu, attaquant).
age : utile pour les tris et statistiques.
buts : nombre de buts marqués par le joueur.
dateInscription (bonus) : fixée à l’ajout du joueur, non modifiable.
statut (bonus) : indique si le joueur est "titulaire" ou "remplaçant".

 *
 */
typedef struct
{
    int j, m, a;
} Date;

typedef struct
{
    int id;
    char nom[MAX_CHAR];
    char prenom[MAX_CHAR];
    int num_maillot;
    char poste[MAX_CHAR];
    int age;
    int buts;
    Date dateInscription;
    char statut[MAX_CHAR];
} Joueur;

typedef struct
{
    Joueur *joueur;
    int effective;
} Equipe;

Date init_date_1(int j, int m, int a)
{
    Date new_date;
    new_date.j = j;
    new_date.m = m;
    new_date.a = a;
    return new_date;
}

Joueur init_joueur(int i, char nm[], char pnm[], int maillot, char pos[], int ag, int bt, Date dt, char st[])
{
    Joueur j;
    j.id = i;
    strcpy(j.nom, nm);
    strcpy(j.prenom, pnm);
    strcpy(j.poste, pos);
    strcpy(j.statut, st);
    j.num_maillot = maillot;
    j.age = ag;
    j.buts = bt;
    j.dateInscription = init_date_1(dt.j, dt.m, dt.a);
}

Equipe init_equipe(int eff)
{
    Equipe e;
    e.effective = eff;
    e.joueur = (Joueur *)malloc(sizeof(Joueur) * e.effective);
    return e;
}

//  AJOUTER JOUEUR
// AJOUTER UN SEUL JOUEUR
void ajouter_un_nouveau_joueur(Equipe eq, Joueur j)
{
    if (eq.effective == 0)
    {
        eq.joueur[eq.effective++] = j;
    }
    else
    {
        for (int i = 0; i < eq.effective; i++)
        {
            
        }
    }
}
// AFFICHER JOUEUR
    // Trier les joueurs par ordre alphabétique (Nom).
    // Trier les joueurs par âge.
    //Afficher les joueurs par poste.

// AJOUTER MULTIPLE JOUEUR EN SEULE OPERATION
// MODIFIER UN JOUEUR
    //Modifier le poste d’un joueur.
    //Modifier l’âge d’un joueur.
    //Modifier le nombre de buts marqués par un joueur.

// SUPPRIMER UN JOUEUR
    //Supprimer un joueur par identifiant.

// RECHERCHER  UN JOUEUR
// STATISTIQUES
    //Afficher le nombre total de joueurs dans l’équipe.
    //Afficher l’âge moyen des joueurs.
    //Afficher les joueurs ayant marqué plus de X buts (X introduit par l’utilisateur).
    //Afficher le meilleur buteur (joueur avec le maximum de buts).
    //Afficher le joueur le plus jeune et le plus âgé.

int main()
{

    return 0;
}