#include "FonctionsListe.h"

PListe Creer_Liste(PArbre nouvel_arbre)
/*

Cette fonction crée une liste avec comme premier élement nouvel_arbre

*/
{   
    PListe liste = MALLOC(Liste);    
    PElement element = MALLOC(Element);


    //On verifie qu'il n y a pas de probleme de memoire.
    if (liste == NULL || element == NULL)
    {
    	printf("Erreur d'allocation de mémoire");
        exit(1);
    }


    element->arbre = nouvel_arbre;
    element->suivant= NULL;
    liste->premier = element;
    
	return liste;
}

int Taille_Liste(PListe liste) 
/*

    Cette fonction indique la taille de la liste.

*/
{
    PElement tmp = liste -> premier;
    int taille = 0;
    while (tmp != NULL)//Tant qu'il reste un element dans la liste taille la  prend taille + 1 
    {
        taille++;
        tmp = tmp->suivant;
    }
    return taille; 
}


void Insertion_Liste_Debut(PListe liste,  PArbre nouvel_arbre)
/*

Cette fonction insert un nouvel arbre au début de la liste

*/
{
    PElement nouvel_element = MALLOC(Element);

    //On verifie qu'il n y a pas de probleme de memoire.
    if (nouvel_element == NULL)
    {
    	printf("Erreur d'allocation de mémoire ou liste NULL");
        exit(1);
    }

    if (liste == NULL) liste = Creer_Liste(nouvel_arbre);
    else//On insere l'element au debut de la liste.
    {
    nouvel_element->arbre = nouvel_arbre; 
    nouvel_element->suivant = liste->premier;
    liste->premier = nouvel_element;
    }
}
void Afficher_Liste(PListe liste)
/*

Cette fonction affiche une liste

*/
{
    PElement actuel_lu = liste->premier;

    //On verifie qu'il n y a pas de probleme de memoire.
    if (liste == NULL)
    {
    	printf("Erreur : Afficher une liste NULL impossible");
        exit(1);
    }
   
    while (actuel_lu != NULL)//Tant qu'il reste encore un element a parcourir dans la liste.
    {
        printf("[%c,%d] -> ", actuel_lu->arbre->lettre,actuel_lu->arbre->occurrence);
        actuel_lu = actuel_lu->suivant;
    }
    printf("NULL\n");
}

PElement Rechercher_Lettre(PListe liste, char lettre_cherche)
/*

Cette fonction renvoie NULL si la lettre n'est pas dans la liste
et l'element où la lettre est presente si elle est dans la liste

*/
{
    PElement tmp = liste->premier;

    while(tmp != NULL)//Tant qu'il reste un caractere dans la liste.
    {
        if(tmp->arbre->lettre == lettre_cherche) return tmp;
        tmp = tmp->suivant;
    }
    return NULL;
}
PListe Creation_Liste_Occurrence(FILE* fichier)
/*

Cette fonction lit un texte et return une liste d'arbres qui contiennent
les lettres et le nombre de fois ou elles sont dans le texte 

*/
{
    char lettrelu;
    PElement Estdansliste;
    PArbre abr = Creer_Arbre(fgetc(fichier),1);
    PListe liste = Creer_Liste(abr);
    
    while ((lettrelu = fgetc(fichier)) != EOF)//Tant qu'il reste un caractere a parcourir dans le fichier.
    {   
        abr = Creer_Arbre(lettrelu,1);
        Estdansliste = Rechercher_Lettre(liste,abr->lettre);
        if (Estdansliste!=NULL) Estdansliste->arbre->occurrence++;//Si le caractere existe on ajoute 1 à son nombre d'occurence. 
        else Insertion_Liste_Debut(liste,abr);//Si le caractere n'existe pas on l'ajoute a la liste.
    }
    fseek(fichier,0,SEEK_SET);//On retrourne au premier charactere du texte.
    return(liste);  
}

void Ajouter_Dans_Ordre_Croissant(PListe liste, PArbre nouvel_arbre)
/*

liste doit être trié dans l'ordre croissant
Ajoute un arbre dans la liste dans un ordre croissant (en fonction des occurrences)

*/

{
    PElement nouvel_element = MALLOC(Element);
    PListe liste_suivant = MALLOC(Liste); 

    //On verifie qu'il n y a pas de probleme de memoire.
    if (nouvel_element == NULL || liste_suivant == NULL)
    {
    	printf("Erreur d'allocation de mémoire");
        exit(1);
    }

	if (liste == NULL) liste = Creer_Liste(nouvel_arbre); // Si la liste est NULL
    
    
	else if (liste->premier->arbre->occurrence>=nouvel_arbre->occurrence) // Si l'occurrence est plus petite que le premier élement

    {
        Insertion_Liste_Debut(liste,nouvel_arbre);
        
    }
    else if (liste->premier->suivant == NULL) // Si on est à la fin
    {
        nouvel_element->arbre = nouvel_arbre;
        nouvel_element->suivant = NULL;
        liste -> premier->suivant = nouvel_element;
        
    }
	else //Sinon 

	{
        liste_suivant -> premier = liste->premier->suivant;
        liste_suivant->premier->suivant = liste->premier->suivant->suivant;

		Ajouter_Dans_Ordre_Croissant(liste_suivant,nouvel_arbre);
        
        liste -> premier -> suivant = liste_suivant->premier;
        
	}

}

void Trie_Liste_Croissant(PListe liste)
/*

Cette fonction trie les fils droit d'un arbre dans l'ordre croissant

*/
{
    PListe nouvel_liste = Creer_Liste(liste->premier->arbre);
    PElement actuel = liste->premier->suivant;

    if (liste != NULL && liste->premier->suivant != NULL) 
    {
        while (actuel != NULL)
        {
           
            Ajouter_Dans_Ordre_Croissant(nouvel_liste,actuel->arbre);
            actuel = actuel->suivant;
           
        }
        liste ->premier = nouvel_liste -> premier;
    }
    
}
void Supprimer_Liste_Debut(PListe liste)
/*

Cette fonction supprime le premier élement d'une liste

*/
{
    PElement copie_liste = liste->premier;

    liste->premier = liste->premier->suivant;
    free(copie_liste);
}