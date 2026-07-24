#include "FonctionsArbre.h"


PArbre Creer_Arbre(char nouvel_lettre, int nouvel_occurrence)
/*

Cette fonction crée un Arbre avec comme lettre égale à nouvel_lettre
et comme occurrence égale à nouvel_occurrence.
Les deux fils droit et gauche sont NULL

*/
{
    
    PArbre nouvel_arbre = MALLOC(Arbre);

    if (nouvel_arbre == ARBRENULL)
    {
    	printf("Erreur d'allocation de mémoire");
        exit(1);
    }

    nouvel_arbre->lettre = nouvel_lettre;
    nouvel_arbre->occurrence = nouvel_occurrence;
    nouvel_arbre->fd = NULL;
    nouvel_arbre->fg = NULL;

    return nouvel_arbre;

}

PArbre Insertion_Arbre_Debut(PArbre abr,char nouvel_lettre,int nouvel_occurrence)
/*

Cette fonction insert un nouvel arbre qui a pour lettre égale à nouvel_lettre
et a pour occurrence égale à nouvel_occurrence au début.
Si abr est NULL on renvoie le nouvel arbre.

*/
{
    PArbre arbre_temporaire;
    PArbre nouvel_arbre = Creer_Arbre(nouvel_lettre,nouvel_occurrence);
    nouvel_arbre->fd = abr;
    return nouvel_arbre;
}

void Afficher_Arbre_fd(PArbre abr)
/*

Cette fonction affiche partiellement un arbre, seulement tout les fils droit.

*/
{
    if (abr != ARBRENULL) 
    {
        PArbre actuel_lu = abr;
        while (actuel_lu != ARBRENULL)
        {
            printf("[%c,%d] -> ", actuel_lu->lettre,actuel_lu->occurrence);
            actuel_lu = actuel_lu->fd;
        }
    }
    printf("NULL\n");
}

PArbre Creer_Arbre_Huffman(FILE* fichier)
/*

Cette fonction créée l'arbre de Huffman à partir d'une liste
qui contient toutes les lettres du texte à compresser (liste_occurrence).

*/
{
    PListe liste;
    PElement premier = MALLOC (Element);
    int somme_min; 
    PArbre arbre_min = Creer_Arbre(0,0);//On creee la racine.

    liste = Creation_Liste_Occurrence(fichier);
    Trie_Liste_Croissant(liste);

    if(liste == NULL) // Si liste NULL erreur
    {
        printf("Erreur : liste NULL");
        exit(1);
    }
    
    while (Taille_Liste(liste) != 1)
    {
        
        premier = liste->premier;
        somme_min = premier->arbre->occurrence + premier->suivant->arbre->occurrence;

        arbre_min = Creer_Arbre(-1,somme_min);// Je mets -1 car aucun caractère ne peut correspondre à -1 dans un texte.

        arbre_min->fg = premier->arbre; // On cree un arbre qui a pour fg et fd les lettres les moins présentent dans le texte.
        arbre_min->fd = premier->suivant->arbre;
        if (Taille_Liste(liste) == 2) return arbre_min;
        
        Supprimer_Liste_Debut(liste);// On supprime les deux premier elements.
        Supprimer_Liste_Debut(liste);
        
        
        Ajouter_Dans_Ordre_Croissant(liste,arbre_min);
    }


    return premier->arbre;
    
}


void Afficher_Parcours_Prefixe(PArbre arbre) 
/*

    Parcours l'arbre de maniere prefixe et affiche le contenue de chacun noeud. 

*/
{
    if (arbre != NULL) 
    { 
        printf("[%c,%d] ", arbre->lettre, arbre->occurrence);
        Afficher_Parcours_Prefixe(arbre->fg);
        Afficher_Parcours_Prefixe(arbre->fd); 
    } 
}

int Est_Dans_Arbre(PArbre arbre, char lettre_recherche)
/*

    On parcours l'arbre de maniere recursive. Retourne TRUE si un noeud de l'arbre contient le caractere recherche.

*/
{
    if (arbre != NULL) 
    { 
        if(arbre->lettre == lettre_recherche) return TRUE;
        if(Est_Dans_Arbre(arbre->fg,lettre_recherche)) return TRUE;
        if(Est_Dans_Arbre(arbre->fd,lettre_recherche)) return TRUE; 
    } 
    return FALSE;
}
int Est_Feuille(PArbre arbre)
/*

    Cette fonction verifie si le noeud est une feuille.

*/
{
    if (arbre->fd == NULL && arbre->fg == NULL ) return TRUE;
    return FALSE;
}

int max(int a, int b)
{
	return (a<b ? b : a);
}

int Hauteur(PArbre arbre) 
/*

    Fonction recursive indiquant la hauteur de l'arbre.

*/
{
	if (!arbre) return -1;
	if (!arbre->fg && !arbre->fd) return 0;
	return 1 + max(Hauteur(arbre->fg),Hauteur(arbre->fd));
}

int Taille(PArbre a) 
/*

    Fonction recursive indiquant la taille de l'arbre.

*/
{
    if(a == NULL || Est_Feuille(a)) return 0;
    return 1 + max(Taille(a->fg),Taille(a->fd)); 
}


