#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

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

int calculer_age(int a) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900 - a;
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

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
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

// utilité
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
const char postes[4][MAX_CHAR] = {"gardien", "defenseur", "milieu", "attaquant"};
const char statut[2][MAX_CHAR] = {"TITULAIRE", "REMPLACANT"};
// AJOUTER UN SEUL JOUEUR
// Vérifie si un numéro de maillot existe déjà dans l'équipe
int maillot_existe(Equipe *eq, int numero) {
    for (int i = 0; i < eq->effective; i++) {
        if (eq->joueur[i].num_maillot == numero) {
            return 1; // trouvé
        }
    }
    return 0; // libre
}

void ajouter_un_nouveau_joueur(Equipe *eq)
{
    Joueur j;
    int id = genere_id(eq);
    char nom[MAX_CHAR];
    char prenom[MAX_CHAR];
    char pos[MAX_CHAR];
    char status[MAX_CHAR];
    int maillot;
    int annee_de_naissance;
    int age;
    int num_buts;
    int choix_pos;
    int choix_status;
    int j_insc, m_insc, a_insc;
    char tmp[64];

    lire_chaine("Entrez le nom :\n", nom, sizeof(nom));
    lire_chaine("Entrez le prenom :\n", prenom, sizeof(prenom));

    // maillot unique
    do {
        lire_chaine("Entrez le numero de maillot :\n", tmp, sizeof(tmp));
        if (sscanf(tmp, "%d", &maillot) != 1) {
            printf("Entree invalide, veuillez entrer un nombre !\n");
            maillot = -1;
        } else if (maillot_existe(eq, maillot)) {
            printf("Le numero de maillot %d est deja pris, choisissez un autre.\n", maillot);
            maillot = -1;
        }
    } while (maillot < 0);

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

    // choix de status
    lire_chaine("Le joueur est: \n \t 1. titulaire \n \t 2. remplacant \n", tmp, sizeof(tmp));
    if (sscanf(tmp, "%d", &choix_status) != 1)
        choix_status = 2;
    switch (choix_status)
    {
    case 1:
        strcpy(status, statut[0]);
        break;
    case 2:
        strcpy(status, statut[1]);
        break;
    default:
        strcpy(status, statut[1]);
    }

    //Choix poste
    lire_chaine("\t \n 1) gardien.  \n 2) defenseur.  \n 3) milieu.  \n 4) attaquant\nEntrez le numero du poste :\n", tmp, sizeof(tmp));
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
        printf("%s LE CHOIX EST INVALIDE, ON VA L'INSERER COMME ATTAQUANT ! \n PRIERE DE REGLER LE POSTE ULTÉRIEUREMENT !\n%s", red, ANSI_RESET);
        strcpy(pos, postes[3]);
        break;
    }

    // Création du joueur
    j = init_joueur(id, nom, prenom, maillot, pos, age, num_buts, dt, status);

    // Vérifier la capacité
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
    printf("✅ Joueur ajoute avec succes ! ID = %d, Maillot = %d\n", j.id, j.num_maillot);
}

// AFFICHER JOUEUR
void show_player(Joueur j)
{
    printf("+-----------------------------------------------------------------------------------------+\n");
    printf("| %-12s | %-12s | %-3s | %-12s | %-6s | %-8s | %-3s | %-5s | %-15s |\n",
           "Prenom", "Nom", "ID", "Poste", "Maillot", "Statut", "Age", "Buts", "Inscription");
    printf("+-----------------------------------------------------------------------------------------+\n");

    printf("| %-12s | %-12s | %-3d | %-12s | %-6d | %-8s | %-3d | %-5d | %02d/%02d/%04d       |\n",
           j.prenom[0] ? j.prenom : "NULL",
           j.nom[0] ? j.nom : "NULL",
           j.id,
           j.poste[0] ? j.poste : "NULL",
           j.num_maillot,
           j.statut[0] ? j.statut : "NULL",
           j.age,
           j.buts,
           j.dateInscription.j,
           j.dateInscription.m,
           j.dateInscription.a);

    printf("+-----------------------------------------------------------------------------------------+\n");
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
// bonus
void star_de_lequipe(Equipe eq)
{
    int i = position_max_but(eq);
    printf("le STAR DE L'EQUIPE EST : \n");
    show_player(eq.joueur[i]);
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
// rechercher Joueurs (bonus)
void rechercher_joueurs_par_poste(Equipe *eq, const char poste[])
{
    if (eq->effective == 0)
    {
        printf("Aucun joueur dans l'equipe.\n");
        return;
    }

    int found = 0;
    printf("Liste des joueurs au poste '%s' :\n", poste);
    for (int i = 0; i < eq->effective; i++)
    {
        if (strcasecmp(eq->joueur[i].poste, poste) == 0)
        {
            show_player(eq->joueur[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Aucun joueur trouve pour le poste '%s'.\n", poste);
    }
}

void rechercher_joueurs_par_age(Equipe *eq, int age)
{
    if (eq->effective == 0)
    {
        printf("Aucun joueur dans l'equipe.\n");
        return;
    }

    int found = 0;
    printf("Liste des joueurs ayant l'age %d :\n", age);
    for (int i = 0; i < eq->effective; i++)
    {
        if (eq->joueur[i].age == age)
        {
            show_player(eq->joueur[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Aucun joueur trouve avec l'age %d.\n", age);
    }
}

void rechercher_joueurs_par_tranche_d_age(Equipe *eq)
{
    if (eq->effective == 0)
    {
        printf("Aucun joueur dans l'equipe.\n");
        return;
    }

    int minAge = 0, maxAge = 0;
    char buffer[32];

    printf("Entrez le minimum d'age : ");
    if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%d", &minAge) != 1)
    {
        printf("Entree invalide. Valeur par defaut 0 utilisee. \n");
        minAge = 0;
    }

    printf("Entrez le maximum d'age : ");
    if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%d", &maxAge) != 1)
    {
        printf("Entree invalide. Valeur par defaut 100 utilse. \n");
        maxAge = 100;
    }

    if (minAge > maxAge)
    {
        printf("Le minimum d'age est superieur au maximum. Inversion automatique.\n");
        int tmp = minAge;
        minAge = maxAge;
        maxAge = tmp;
    }

    int found = 0;
    printf("Liste des joueurs ayant l'age entre %d et %d :\n", minAge, maxAge);
    for (int i = 0; i < eq->effective; i++)
    {
        if (eq->joueur[i].age >= minAge && eq->joueur[i].age <= maxAge)
        {
            show_player(eq->joueur[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Aucun joueur trouve dans cette tranche d'age.\n");
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
    star_de_lequipe(eq);
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
            if (eq->joueur[j].id > eq->joueur[j + 1].id)
            {
                swap(&eq->joueur[j], &eq->joueur[j + 1]);
            }
        }
    }
}
void afficher_les_joueurs_par_id(Equipe eq)
{
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
void modifier_un_joueur_poste(int id, Equipe *eq)
{   int NB_POSTES = sizeof(postes)/MAX_CHAR;
    int index = rechercher_joueur_index(id, eq);
    if (index == -1) {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s\n", red, ANSI_RESET);
        return;
    }

    
    printf("=== Choisissez un nouveau poste ===\n");
    for (int i = 0; i < NB_POSTES; i++) {
        printf("%d) %s\n", i + 1, postes[i]);
    }
    printf("Votre choix : \n");

    int choix;

    if (scanf("%d", &choix) != 1) {
        printf("%sEntrée invalide.%s\n", red, ANSI_RESET);
        while (getchar() != '\n');  
        return;
    }

    if (choix < 1 || choix > NB_POSTES) {
        printf("%sLe choix est invalide !!!%s\n", red, ANSI_RESET);
        return;
    }

    strcpy(eq->joueur[index].poste, postes[choix - 1]);
    printf("%sPoste du joueur modifie avec succes.%s\n", green, ANSI_RESET);
}

// Modifier l’âge d’un joueur.
void modifier_un_joueur_age(int id, int nv_age, Equipe *eq)
{
    // Vérifier d'abord si l'ID existe
    int index = rechercher_joueur_index(id, eq);
    if (index == -1) {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s\n", red, ANSI_RESET);
        return;
    }

    // Vérifier la validité de l'âge
    if (nv_age < 18 || nv_age > 45) {
        printf("%sAge invalide : %d (doit etre entre 18 et 45).%s\n",
               red, nv_age, ANSI_RESET);
        return;
    }

    // Mise à jour si tout est correct
    eq->joueur[index].age = nv_age;
    printf("%sÂge du joueur modifie avec succes.%s\n", green, ANSI_RESET);
}
// Modifier le nombre de buts marques par un joueur.
void modifier_un_joueur_buts(int id, int new_buts, Equipe *eq)
{
    int index = rechercher_joueur_index(id, eq);
    if (index == -1) {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s\n", red, ANSI_RESET);
        return;
    }

    if (new_buts < 0) {
        printf("%sLe nombre de buts ne peut pas etre negatif (%d).%s\n",
               red, new_buts, ANSI_RESET);
        return;
    }

    eq->joueur[index].buts = new_buts;
    printf("%sNombre de buts modifie avec succes.%s\n", green, ANSI_RESET);
}
// SUPPRIMER UN JOUEUR

// Supprimer un joueur par identifiant.

void supprimer_joueur(Equipe *eq, int id)
{
    int index = rechercher_joueur_index(id, eq);

    if (index == -1)
    {
        printf("%sJoueur avec ID %d introuvable.%s\n", red, id, ANSI_RESET);
        return;
    }

    // Décaler les joueurs vers la gauche
    for (int i = index; i < eq->effective - 1; i++)
    {
        eq->joueur[i] = eq->joueur[i + 1];
    }

    eq->effective--;
    printf("Joueur avec ID %d supprime avec succes.\n", id);
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
    printf("%s ==> entrez 0 pour retourner au menu principal <== %s\n", green, ANSI_RESET);
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

void afficher_joueurs_marque_plus_X_buts(Equipe eq, int x)
{
    int found = 0;

    for (int i = 0; i < eq.effective; i++)
    {
        if (eq.joueur[i].buts > x)
        {
            if (!found)
            {
                printf("Liste des joueurs ayant marque plus de %d buts :\n", x);
            }
            show_player(eq.joueur[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Aucun joueur n'a marque plus de %d buts.\n", x);
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
