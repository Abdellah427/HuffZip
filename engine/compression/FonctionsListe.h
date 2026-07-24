#ifndef CONSTANTE_Liste
#define CONSTANTE_Liste

#include <stdio.h>
#include <stdlib.h>


#define MALLOC(x) ((x * ) malloc(sizeof(x)))
#define TRUE 1;
#define FALSE 0;


typedef struct Liste Liste;
typedef struct Element Element;

typedef Liste* PListe;
typedef Element* PElement;

#include "FonctionsArbre.h" // Je le mets ici car FonctionsArbre a besoin de la struct liste

struct Element
{
    PArbre arbre;
    PElement suivant;
};

struct Liste
{
    PElement premier;
};

PListe Creer_Liste(PArbre);
int Taille_Liste (PListe);
void Insertion_Liste_Debut(PListe,PArbre);
void Afficher_Liste(PListe);
PElement Rechercher_Lettre(PListe,char);
PListe Creation_Liste_Occurrence(FILE*);
void Ajouter_Dans_Ordre_Croissant(PListe,PArbre);
void Trie_Liste_Croissant(PListe);
void Supprimer_Liste_Debut(PListe);









#endif