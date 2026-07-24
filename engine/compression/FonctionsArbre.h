#ifndef CONSTANTE_Arbre
#define CONSTANTE_Arbre

#define ARBRENULL NULL
typedef struct Arbre Arbre;
typedef Arbre* PArbre;

struct Arbre
{
    char lettre;
    int occurrence;
    PArbre fd;
    PArbre fg;
};

#include "FonctionsListe.h" // Je le mets car FontionsListe.h a besoin de la struct arbre 

PArbre Creer_Arbre(char,int);
PArbre Insertion_Arbre_Debut(PArbre,char,int);
void Afficher_Arbre_fd(PArbre);
PArbre Creer_Arbre_Huffman(FILE*);// Cette fonction modifie la liste
void Afficher_Parcours_Prefixe(PArbre); 
int Est_Dans_Arbre(PArbre,char);
int Est_Feuille(PArbre);
int max(int,int);
int Hauteur(PArbre);
int Taille(PArbre);


#endif