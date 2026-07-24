#include "FonctionsListe.h"

typedef struct ElementConv ElementConv;
typedef struct ListeConv ListeConv;

typedef ElementConv* PElementConv;
typedef ListeConv* PListeConv;


struct ElementConv // conv pour converssion
{
	char lettre;
	int* tabNbreBinaire;	
	PElementConv suivant;
};
struct ListeConv
{
    PElementConv premier;
	int tailleTableau; // la taille de tout les tableaux
};

PElementConv Creer_ElementConv(char,int);
PListeConv Creer_ListeConv(char,int);
void Afficher_ListeConv(PListeConv);
void Insertion_ListeConv_Debut(PListeConv,PElementConv);
void Correspondance_Binaire(PArbre,PElementConv);
PListeConv Creer_tableau_conversion_binaire(PListe liste_occurrence,PArbre);
void Retransciption(FILE*,PListeConv,PArbre arbreHuffman);
int Nombre_De_Packet(FILE*,int);
int Puissance(int,int);
int Conversion_En_Decimal(int nombrebinaire[7]);
int Fin_Premiere_Partie(FILE* fichierEntre);
int* Conversion_En_binaire(int);
FILE* Compression(void);




