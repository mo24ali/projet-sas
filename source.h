#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ANSI_RESET "\033[0m"
#define black "\033[0;30m"
#define red "\033[0;31m"
#define green "\033[0;32m"
#define yellow "\033[0;33m"
#define blue "\033[0;34m"
#define magenta "\033[0;35m"
#define cyan "\033[0;36m"
#define white "\033[0;37m"
#define bold_black "\033[1;30m"
#define bold_red "\033[1;31m"
#define bold_green "\033[1;32m"
#define bold_yellow "\033[1;33m"
#define bold_blue "\033[1;34m"
#define bold_magenta "\033[1;35m"
#define bold_cyan "\033[1;36m"
#define bold_white "\033[1;37m"

#define MAX_CHAR 50

typedef struct
{
    int j, m, a;
} Date;

typedef struct
{
    int id;
    int age;
    int buts;
    int num_maillot;
    Date dateInscription;
    char nom[MAX_CHAR];
    char prenom[MAX_CHAR];
    char poste[MAX_CHAR];
    char statut[MAX_CHAR];
} Joueur;

typedef struct
{
    Joueur *joueur;
    int effective;
    int capacite;
} Equipe;

Date init_date_1(int j, int m, int a)
{
    Date new_date;
    new_date.j = j;
    new_date.m = m;
    new_date.a = a;
    return new_date;
}
// affiche date
void afficher_date(Date dt)
{
    printf("%d / %d / %d ", dt.j, dt.m, dt.a);
}

int calculer_age(int a)
{
    return 2025 - a;
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
    return j;
}

// read from the file 
/*
void charger_joueurs(char *nomFichier, Joueur tab[], int tailleMax) {
    FILE *f = fopen(nomFichier, "r");
    if (!f) {
        perror("Erreur ouverture fichier");
        return -1;
    }

    int n = 0;
    while (n < tailleMax &&
           fscanf(f, "%d, \"%29[^\"]\", \"%29[^\"]\", %d, \"%19[^\"]\", %d, %d, %d, %d, %d, \"%19[^\"]\"\n",
                  &tab[n].id,
                  tab[n].nom,
                  tab[n].prenom,
                  &tab[n].numero,
                  tab[n].poste,
                  &tab[n].age,
                  &tab[n].buts,
                  &tab[n].date.jour,
                  &tab[n].date.mois,
                  &tab[n].date.annee,
                  tab[n].statut) == 11)
    {
        n++;
    }

    fclose(f);
 //   return n;   // number of players loaded
}

*/

Equipe init_equipe(int capacite)
{
    Equipe e;
    e.effective = 0;
    e.capacite = (capacite > 0) ? capacite : 10;
    e.joueur = malloc(sizeof(Joueur) * e.capacite);
    if (!e.joueur)
    {
        fprintf(stderr, "Erreur allocation memoire pour l'equipe\n");
        exit(EXIT_FAILURE);
    }
    return e;
}

// genere id

int genere_id(Equipe *eq)
{
    int max_id = 0;

    for (int i = 0; i < eq->effective; i++)
    {
        if (eq->joueur[i].id > max_id)
        {
            max_id = eq->joueur[i].id;
        }
    }

    return max_id + 1; 
}

// supprime le '\n' final s'il existe
void trim_newline(char *s)
{
    int len = strlen(s);
    if (len == 0)
        return;
    if (s[len - 1] == '\n')
        s[len - 1] = '\0';
}

// lecture sûre d'une ligne (fgets + trim)
void lire_chaine(char *message, char *buffer, int size)
{
    if (message)
        printf("%s", message);
    if (fgets(buffer, size, stdin) == NULL)
    {
        buffer[0] = '\0';
    }
    else
    {
        trim_newline(buffer);
    }
}

//  AJOUTER JOUEUR
// AJOUTER UN SEUL JOUEUR
void ajouter_un_nouveau_joueur(Equipe *eq)
{
    Joueur j;
    int id = genere_id(eq);
    char nom[MAX_CHAR];
    char prenom[MAX_CHAR];
    char pos[MAX_CHAR];
    char status[MAX_CHAR];
    char postes[4][MAX_CHAR] = {"gardien", "defenseur", "milieu", "attaquant"};
    char statut[2][MAX_CHAR] = {"titulaire","remplacant"};
    int maillot;
    int annee_de_naissance;
    int age;
    int num_buts;
    int choix_pos;
    int choix_status;
    int j_insc, m_insc, a_insc;

    lire_chaine("Entrez le nom :\n", nom, sizeof(nom));
    lire_chaine("Entrez le prenom :\n", prenom, sizeof(prenom));

    // lire entiers - on utilise fgets + sscanf pour éviter les restes dans stdin
    char tmp[64];

    /**
     * 
     * lire chaine affiche le message de demande de input
     * prend comme argument le message, le buffer pour lire l'input et size de input
     * size est donné par sizeof lors de l'execution
     * 
     * lire par fgets et traite la situation de blocage de buffer par le retour a la ligne
     * 
     * 
     * 
     */
    lire_chaine("Entrez le numero de maillot :\n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &maillot) != 1)
        maillot = 0;

    lire_chaine("Entrez l'annee de naissance :\n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &annee_de_naissance) != 1)
        annee_de_naissance = 2000;

    age = calculer_age(annee_de_naissance);

    lire_chaine("Entrez le nombre de buts :\n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &num_buts) != 1)
        num_buts = 0;

    lire_chaine("Entrez le jour de l'inscription :\n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &j_insc) != 1)
        j_insc = 1;

    lire_chaine("Entrez le mois de l'inscription :\n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &m_insc) != 1)
        m_insc = 1;

    lire_chaine("Entrez l'annee d'inscription :\n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &a_insc) != 1)
        a_insc = 2025;

    Date dt = init_date_1(j_insc, m_insc, a_insc);

    lire_chaine("Le joueur est: \n \t 1. titulaire \n \t 2. remplacant \n", status, sizeof(status));
    if(sscanf(tmp,"%d",&choix_status) != 1)
        choix_status = 2;
    switch(choix_status){
        case 1:
            strcpy(status,statut[0]);
            break;
        case 2:
            strcpy(status,statut[1]);
            break;
    }

    // Choix poste
    lire_chaine("1) gardien.  \n 2) defenseur.  \n 3) milieu.  \n 4) attaquant\nEntrez le numero du poste :\n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &choix_pos) != 1)
        choix_pos = 4;
    switch (choix_pos)
    {
    case 1:
        strcpy(pos, postes[0]);
        break;
    case 2:
        strcpy(pos, postes[1]);
        break;
    case 3:
        strcpy(pos, postes[2]);
        break;
    case 4:
        strcpy(pos, postes[3]);
        break;
    default:
        printf("%s LE CHOIX EST INVALIDE MAIS ON VA L'INSERER AUTAUNT QUE ATTAQUANT POUR LE MOMENT ! \n PRIERE DE REGLER LE POSTE ULTIRIEREMENT !\n%s",red,ANSI_RESET);
        strcpy(pos, postes[3]);
        break;
    }

    j = init_joueur(id, nom, prenom, maillot, pos, age, num_buts, dt, status);

    // si besoin, agrandir (ton code faisait ça déjà)
    if (eq->effective >= eq->capacite)
    {
        int newcap = eq->capacite * 2;
        Joueur *tmpj = realloc(eq->joueur, sizeof(Joueur) * newcap);
        if (!tmpj)
        {
            printf("Erreur allocation memoire\n");
            return;
        }
        eq->joueur = tmpj;
        eq->capacite = newcap;
    }

    eq->joueur[eq->effective++] = j;
    printf("Joueur ajoute avec succes ! ID = %d\n", j.id);
}


// AFFICHER JOUEUR
void show_player(Joueur j)
{       
    printf(" %-4s | %-12s | id : %d  | poste : %-12s | num_maillot : %-6d | statut : %-6s | age : %d  | numero de buts : %-12d | Date d'inscription : ",
           j.prenom[0] ? j.prenom : "NULL",
           j.nom[0] ? j.nom : "NULL",
           j.id,
           j.poste[0] ? j.poste : "NULL",
           j.num_maillot,
           j.statut[0] ? j.statut : "NULL",
           j.age,
           j.buts);
    afficher_date(j.dateInscription);
    printf("\n");
}
void show_player_2(Joueur j)
{       
    printf("%-12s | %-12s | %-3d | %-12s | %-7d | %-8s | %-3d | %-5d | ",
           j.prenom[0] ? j.prenom : "NULL",
           j.nom[0] ? j.nom : "NULL",
           j.id,
           j.poste[0] ? j.poste : "NULL",
           j.num_maillot,
           j.statut[0] ? j.statut : "NULL",
           j.age,
           j.buts);
    afficher_date(j.dateInscription);
    printf("\n");
}
// Trier les joueurs par ordre alphabétique (Nom).
void swap(Joueur *a, Joueur *b)
{
    Joueur tmp = *a;
    *a = *b;
    *b = tmp;
}
void trier_les_joueurs_alphabetiquement(Equipe *eq)
{
    int n = eq->effective;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (strcasecmp(eq->joueur[j].nom, eq->joueur[j + 1].nom) < 0)
            {
                swap(&eq->joueur[j], &eq->joueur[j + 1]);
            }
        }
    }
}
// rechercher joueur par index
int rechercher_joueur_index(int id_cher, Equipe *eq)
{
    for (int i = 0; i < eq->effective; i++)
    {
        if (eq->joueur[i].id == id_cher)
        {
            return i;
            break;
        }
    }
    return -1;
}
// Trier les joueurs par âge.
void trier_les_joueurs_par_age(Equipe *eq)
{
    int n = eq->effective;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (eq->joueur[j].age > eq->joueur[j + 1].age)
            {
                swap(&eq->joueur[j], &eq->joueur[j + 1]);
            }
        }
    }
}
// Afficher equipe
void afficher_equipe(Equipe eq)
{
    if (eq.effective == 0)
    {
        printf("Aucun joueur dans l'equipe.\n");
        return;
    }

    printf("\n======================================= LISTE DES JOUEURS =======================================\n");
    printf("%-12s | %-12s | %-3s | %-12s | %-7s | %-8s | %-3s | %-5s | %-12s\n",
           "Prenom", "Nom", "ID", "Poste", "Maillot", "Statut", "Age", "Buts", "Inscription");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < eq.effective; i++)
    {
        show_player_2(eq.joueur[i]);
    }

    printf("===============================================================================================================\n\n");
}

// Afficher les joueurs par poste.
// avant de l'afficher par poste il faut les trier par poste
void trier_les_joueurs_par_poste(Equipe *eq)
{
    int n = eq->effective;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (strcasecmp(eq->joueur[j].poste, eq->joueur[j + 1].poste) > 0)
            {
                swap(&eq->joueur[j], &eq->joueur[j + 1]);
            }
        }
    }
}

void trier_les_joueurs_par_id(Equipe *eq)
{
    int n = eq->effective;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (eq->joueur[j].id > eq->joueur[j+1].id)
            {
                swap(&eq->joueur[j], &eq->joueur[j + 1]);
            }
        }
    }
}
void afficher_les_joueurs_par_id(Equipe eq){
    trier_les_joueurs_par_id(&eq);
    afficher_equipe(eq);
}
void afficher_les_joueurs_par_postes(Equipe eq)
{
    trier_les_joueurs_par_poste(&eq);
    afficher_equipe(eq);
}
// AJOUTER MULTIPLE JOUEUR EN SEULE OPERATION
void ajouter_multiple_joueur(Equipe *eq)
{
    int nb;
    printf("combien de joueur vous voulez : \n");
    scanf("%d", &nb);
    getchar();
    for (int i = 0; i < nb; i++)
    {
        printf("*********** Inserer joueur %d ********* \n", i + 1);
        ajouter_un_nouveau_joueur(eq);
    }
}
// MODIFIER UN JOUEUR
// Modifier le poste d’un joueur.
void modifier_un_joueur_poste(int id, char new_pos[], Equipe *eq)
{
    int index = rechercher_joueur_index(id, eq);
    if (index != -1)
    {
        strcpy(eq->joueur[index].poste, new_pos);
    }
    else
    {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s", red, ANSI_RESET);
    }
}

// Modifier l’âge d’un joueur.
void modifier_un_joueur_age(int id, int new_age, Equipe *eq)
{
    int index = rechercher_joueur_index(id, eq);
    if (index != -1)
    {
        eq->joueur[index].age = new_age;
    }
    else
    {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s", red, ANSI_RESET);
    }
}

// Modifier le nombre de buts marqués par un joueur.
void modifier_un_joueur_buts(int id, int new_buts, Equipe *eq)
{
    int index = rechercher_joueur_index(id, eq);
    if (index != -1)
    {
        eq->joueur[index].buts = new_buts;
    }
    else
    {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s", red, ANSI_RESET);
    }
}
// SUPPRIMER UN JOUEUR

// Supprimer un joueur par identifiant.

void supprimer_joueur(Equipe *eq, int id)
{
    int index = rechercher_joueur_index(id, eq);

    if (index == -1)
    {
        printf("%sJoueur avec ID %d introuvable.%s\n",red, id, ANSI_RESET);
        return;
    }

    // Décaler les joueurs vers la gauche
    for (int i = index; i < eq->effective - 1; i++)
    {
        eq->joueur[i] = eq->joueur[i + 1];
    }

    eq->effective--; 
    printf("Joueur avec ID %d supprimé avec succès.\n", id);
}

// RECHERCHER  UN JOUEUR
void rechercher_un_joueur(int id_cher, Equipe eq)
{
    for (int i = 0; i < eq.effective; i++)
    {
        if (eq.joueur[i].id == id_cher)
        {
            printf("Le joueur exist a la position %d ", i + 1);
            show_player(eq.joueur[i]);
        }
    }
}

void rechercher_un_joueur_par_nom(char nom[], Equipe eq)
{
    int found = 0;
    for (int i = 0; i < eq.effective; i++)
    {
        if (strcasecmp(eq.joueur[i].nom, nom) == 0)
        {
            printf("Le joueur existe a la position %d :\n", i + 1);
            show_player(eq.joueur[i]);
            found = 1;
        }
    }
    if (!found)
    {
        printf("%sLe joueur n'existe pas !!! %s\n", red, ANSI_RESET);
    }
}

void afficher_menu_statistiques()
{
    printf("\n========== MENU STATISTIQUES ==========\n");
    printf("1. Afficher le nombre total de joueurs\n");
    printf("2. Afficher l'age moyen des joueurs\n");
    printf("3. Afficher les joueurs ayant marque plus de X buts\n");
    printf("4. Afficher le meilleur buteur\n");
    printf("5. Afficher le joueur le plus jeune et le plus age\n");
    printf("=======================================\n");
    printf("Entrez votre choix : ");
}

// STATISTIQUES
int maxAge(Equipe eq)
{
    if (eq.effective == 0)
        return -1;
    int max = eq.joueur[0].age;
    for (int i = 1; i < eq.effective; i++)
        if (eq.joueur[i].age > max)
            max = eq.joueur[i].age;
    return max;
}

int minAge(Equipe eq)
{
    if (eq.effective == 0)
        return -1;
    int min = eq.joueur[0].age;
    for (int i = 1; i < eq.effective; i++)
        if (eq.joueur[i].age < min)
            min = eq.joueur[i].age;
    return min;
}

float moyenne(Equipe eq)
{
    if (eq.effective == 0)
        return 0.0f;
    int somme = 0;
    for (int i = 0; i < eq.effective; i++)
        somme += eq.joueur[i].age;
    return (float)somme / (float)eq.effective;
}

int jeune_index(Equipe eq)
{
    if (eq.effective == 0)
        return -1;
    int minA = minAge(eq);
    for (int i = 0; i < eq.effective; i++)
    {
        if (eq.joueur[i].age == minA)
            return i;
    }
    return -1;
}

int age_index(Equipe eq)
{
    if (eq.effective == 0)
        return -1;
    int maxA = maxAge(eq);
    for (int i = 0; i < eq.effective; i++)
    {
        if (eq.joueur[i].age == maxA)
            return i;
    }
    return -1;
}

void afficher_le_joueur_le_plus_jeune_et_le_plus_age(Equipe eq)
{
    int agee = age_index(eq);
    int jeune = jeune_index(eq);
    if (agee == -1 || jeune == -1)
    {
        printf("Equipe vide.\n");
        return;
    }
    printf("Le joueur le plus age est : \n");
    show_player(eq.joueur[agee]);
    printf("\nLe joueur le plus jeune est : \n");
    show_player(eq.joueur[jeune]);
}
int position_max_but(Equipe eq)
{
    if (eq.effective == 0)
        return -1;
    int best_idx = 0;
    int max_but = eq.joueur[0].buts;
    for (int i = 1; i < eq.effective; i++)
    {
        if (eq.joueur[i].buts > max_but)
        {
            max_but = eq.joueur[i].buts;
            best_idx = i;
        }
    }
    return best_idx;
}

void afficher_joueurs_marque_plus_X_buts(Equipe eq, int x)
{
    int found = 0;

    for (int i = 0; i < eq.effective; i++)
    {
        if (eq.joueur[i].buts > x)
        {
            if (!found)
            {
                printf("Liste des joueurs ayant marqué plus de %d buts :\n", x);
            }
            show_player(eq.joueur[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Aucun joueur n'a marqué plus de %d buts.\n", x);
    }
}

void afficher_meilleur_buteur(Equipe eq)
{
    int index = position_max_but(eq);
    if (index == -1)
    {
        printf("Aucun joueur dans l'equipe.\n");
        return;
    }
    show_player(eq.joueur[index]);
}
