#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define reset "\033[0m"
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

Equipe init_equipe(int eff)
{
    Equipe e;
    e.effective = eff;
    e.joueur = (Joueur *)malloc(sizeof(Joueur) * e.effective);
    return e;
}

// genere id
int genere_id()
{
    int id;
}

void afficher_menu()
{
    printf("1. Ajouter un joueur\n");
    printf("2. Ajouter plusieurs joueurs\n");
    printf("3. Afficher tous les joueurs\n");
    printf("4. Afficher joueurs par poste\n");
    printf("5. Modifier un joueur\n");
    printf("6. Supprimer un joueur\n");
    printf("7. Rechercher un joueur\n");
    printf("8. Statistiques\n");
    printf("9. Quitter\n");
}
//  AJOUTER JOUEUR
// AJOUTER UN SEUL JOUEUR
void ajouter_un_nouveau_joueur(Equipe *eq)
{
    Joueur j;
    char nom[MAX_CHAR];
    char prenom[MAX_CHAR];
    char pos[MAX_CHAR];
    char status[MAX_CHAR];
    char postes[4][MAX_CHAR] = {"gardien", "defenseur", "milieu", "attaquant"};
    int maillot;
    int annee_de_naissance;
    int age;
    int num_buts;
    int choix_pos;
    int j_insc, m_insc, a_insc;

    printf("entrez le nom : \n");
    fgets(nom, sizeof(nom), stdin);
    printf("entrez le prenom : \n");
    fgets(prenom, sizeof(prenom), stdin);
    printf("entrez le status : \n");
    fgets(status, sizeof(status), stdin);
    printf("entrez le numero de maillot\n");
    scanf("%d", &maillot);
    printf("entrez l'annee de  naissance:\n");
    scanf("%d", &annee_de_naissance);
    age = calculer_age(annee_de_naissance);
    printf("entrez le numero des buts");
    scanf("%d", &num_buts);
    printf("Entrez le jour de l'inscription : \n");
    scanf("%d", &j_insc);
    printf("Entrez le mois de l'inscription : \n");
    scanf("%d", &m_insc);
    printf("Entrez l'annee d'inscription : \n");
    scanf("%d", &a_insc);
    Date dt = init_date_1(j_insc, m_insc, a_insc);
    printf("Le joueur est titulaire ou remplacant ? \n");
    fgets(status, sizeof(status), stdin);
    printf("1. gardien . \n 2. defeseur . \n 3. millieu . \n 4. attaquant");
    printf("entrez la position du joueur : \n");
    scanf("%d", &choix_pos);
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
        printf("choix invalisde");
        break;
    }

    j = init_joueur(0, nom, prenom, maillot, pos, age, num_buts, dt, status);

    if (eq->effective >= eq->capacite)
    {
        eq->capacite *= 2;
        eq->joueur = realloc(eq->joueur, sizeof(Joueur) * eq->capacite);
        if (!eq->joueur)
        {
            printf("Erreur allocation memoire\n");
            exit(EXIT_FAILURE);
        }
    }

    eq->joueur[eq->effective++] = j;

    printf("Joueur ajoute avec succes ! ID = %d\n", j.id);
}
// AFFICHER JOUEUR
void show_player(Joueur j)
{
    printf(" => Le joueur : %s %s \n => poste : %s \n => num_maillot : %d \n => statut : %s \n => age : %d \n => numero de buts : %d \n => Date d'inscription : ", j.prenom, j.nom, j.poste, j.num_maillot, j.statut, j.age, j.buts);
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
            if (strcmp(eq->joueur[j].nom, eq->joueur[j + 1].nom)> 0)
            {
                swap(&eq->joueur[j], &eq->joueur[j + 1]);
            }
        }
    }
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
    for (int i = 0; i < eq.effective; i++)
    {
        printf("%s : ", eq.joueur[i].poste);
        show_player(eq.joueur[i]);
    }
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
            if (strcmp(eq->joueur[j].poste, eq->joueur[j + 1].poste) > 0)
            {
                swap(&eq->joueur[j], &eq->joueur[j + 1]);
            }
        }
    }
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
    for (int i = 0; i < nb; i++)
    {
        printf("*********** Inserver joueur %d ********* \n", i + 1);
        ajouter_un_nouveau_joueur(eq);
    }
}
// MODIFIER UN JOUEUR

// Modifier le poste d’un joueur.
void modifier_un_joueur_poste(Joueur j, char new_pos[], Equipe *eq)
{
    int index = rechercher_joueur_index(j.id, eq);
    if (index != -1)
    {
        strcpy(eq->joueur[index].poste, new_pos);
    }
    else
    {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s",red,reset);
    }
}

// Modifier l’âge d’un joueur.
void modifier_un_joueur_age(Joueur j, int new_age, Equipe *eq)
{
    int index = rechercher_joueur_index(j.id, eq);
    if (index != -1)
    {
        eq->joueur[index].age = new_age;
    }
    else
    {
        printf("%sLE JOUEUR N'EXISTE PAS !!!%s",red,reset);
    }
}
// Modifier le nombre de buts marqués par un joueur.
void modifier_un_joueur_buts(Joueur j, int new_buts, Equipe *eq)
{
    int index = rechercher_joueur_index(j.id, eq);
    if (index != -1)
    {
        eq->joueur[index].buts = new_buts;
    }
    else
    {
        printf("LE JOUEUR N'EXISTE PAS !!!");
    }
}
// SUPPRIMER UN JOUEUR

// Supprimer un joueur par identifiant.
void supprimer_joueur(Equipe *eq, int id)
{
    int index = rechercher_joueur_index(id, eq);
    for (int i = index; i < eq->effective - 1; i++)
    {
        eq->joueur[i] = eq->joueur[i + 1];
    }
    --eq->effective;
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
// STATISTIQUES
int maxAge(Equipe eq)
{
    int max = 0;
    for (int i = 0; i < eq.effective; i++)
    {
        if (max < eq.joueur[i].age)
            max = eq.joueur[i].age;
    }
    return max;
}
int minAge(Equipe eq)
{
    int min = 0;
    for (int i = 0; i < eq.effective; i++)
    {
        if (min > eq.joueur[i].age)
            min = eq.joueur[i].age;
    }
    return min;
}

float moyenne(Equipe eq)
{
    int somme = 0;
    for (int i = 0; i < eq.effective; i++)
    {
        somme += eq.joueur[i].age;
    }
    return (float)somme / eq.effective;
}
int position_max_but(Equipe eq)
{
    int max_id;
    int max_but = 0;
    int n = eq.effective;
    for (int i = 0; i < n; i++)
    {
        if (max_but < eq.joueur[i].buts)
            max_but = eq.joueur[i].buts;
        max_id = eq.joueur[i].id;
    }
    return max_id;
}
void afficher_joueurs_marque_plus_X_buts(Equipe eq, int x)
{
    if (eq.effective == 0)
    {
        printf("Y'a pas de joueurs : \n");
    }
    else
    {
        int *tabInd;
        int count = 0;
        tabInd = (int *)malloc(sizeof(int) * count);
        for (int i = 0; i < eq.effective; i++)
        {
            if (eq.joueur[i].buts == x)
                tabInd[count++] = i;
        }
        if (count == 0)
        {
            printf("Pas de joueurs qui ont marquer %d buts ! \n", x);
        }
        else
        {
            printf("Listes des joueurs qui ont marquer %d buts : \n");
            for (int i = 0; i < count; i++)
            {
                show_player(eq.joueur[tabInd[i]]);
            }
        }
    }
}
void afficher_meilleur_buteur(Equipe eq)
{
    int index = position_max_but(eq);
    show_player(eq.joueur[index]);
}
int jeune_index(Equipe eq)
{
    int size = eq.effective;
    int index_jeune;
    int minA = minAge(eq);
    for(int i =0 ; i < size ; i++){
        if(eq.joueur[i].age == minA){
            index_jeune = i;
        }
    }
    return index_jeune;
}
int age_index(Equipe eq)
{
    int size = eq.effective;
    int index_age;
    int maxA = maxAge(eq);
    for(int i =0 ; i < size ; i++){
        if(eq.joueur[i].age == maxA){
            index_age = i;
        }
    }
    return index_age;
}

void afficher_le_joueur_le_plus_jeune_et_le_plus_age(Equipe eq)
{
    int agee = age_index(eq);
    int jeune = jeune_index(eq);
    printf("Le joueur le plus age est : \n");
    show_player(eq.joueur[agee]);
    printf("\n");
    printf("Le joueur le plus Jeune est : \n");
    show_player(eq.joueur[jeune]);
}
void afficher_statistiques(Equipe eq)
{
    int x;
    // Afficher le nombre total de joueurs dans l’équipe.
    printf("le nombre total des joueurs dans l'equipe : %d", eq.effective);
    // Afficher l’âge moyen des joueurs.
    float moy = moyenne(eq);
    printf("la moyenne de l'age de l'equipe est : %f \n", moy);
    // Afficher les joueurs ayant marqué plus de X buts (X introduit par l’utilisateur).
    printf("entrer le nombre des buts X : \n");
    scanf("%d", &x);
    afficher_joueurs_marque_plus_X_buts(eq, x);
    // Afficher le meilleur buteur (joueur avec le maximum de buts).
    afficher_meilleur_buteur(eq);
    // Afficher le joueur le plus jeune et le plus âgé.
    afficher_le_joueur_le_plus_jeune_et_le_plus_age(eq);
}

int main_1()
{

    return 0;
}