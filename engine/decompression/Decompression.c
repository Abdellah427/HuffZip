#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

#define MALLOC(x) ((x * ) malloc(sizeof(x)))
#define ARBRENULL NULL
#define TRUE 1
#define FALSE 0
#define ARBRENULL NULL
typedef struct Arbre Arbre;
typedef Arbre* PArbre;

typedef struct Liste Liste;
typedef struct Element Element;

typedef Liste* PListe;
typedef Element* PElement;

struct Arbre
{
    char lettre;
    int occurrence;
    PArbre fd;
    PArbre fg;
};

struct Element
{
    PArbre arbre;
    PElement suivant;
};

struct Liste
{
    PElement premier;
};

PListe Creer_Liste(PArbre nouvel_arbre)
/*

Cette fonction crée une liste avec comme premier élement nouvel_arbre

*/
{
    PListe liste = MALLOC(Liste);
    PElement element = MALLOC(Element);

    if (liste == NULL || element == NULL)
    {
        printf("Erreur d'allocation de mémoire");
        exit(1);
    }
    element->arbre = nouvel_arbre;
    element->suivant = NULL;
    liste->premier = element;

    return liste;
}

//////////////////////////Prototype///////////////////////////////// 


PArbre Creer_Arbre(char nouvel_lettre, int nouvel_occurrence);
void Afficher_Liste(PListe liste);
void Insertion_Liste_Debut(PListe liste, PArbre nouvel_arbre);
void Ajouter_Dans_Ordre_Croissant(PListe liste, PArbre nouvel_arbre);
void Trie_Liste_Croissant(PListe liste);
void Supprimer_Liste_Debut(PListe liste);
int Taille_Liste(PListe liste);
void Afficher_Parcours_Prefixe(PArbre arbre);

int* Conversion_En_binaire(int nombredecimal);
int finPremierePartie(FILE* fichierEntre);
void Decompression_Binaire(FILE*);

int traductionEnNombre(FILE* fichierEntre);
PArbre Creer_Arbre_Huffman(PListe liste);
void Decompresse(void);





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

void Afficher_Liste(PListe liste)
/*

Cette fonction affiche une liste

*/
{
    if (liste == NULL)
    {
        printf("Erreur : Afficher une liste NULL impossible");
        exit(1);
    }
    PElement actuel_lu = liste->premier;
    while (actuel_lu != NULL)
    {
        printf("[%c,%d] -> ", actuel_lu->arbre->lettre, actuel_lu->arbre->occurrence);
        actuel_lu = actuel_lu->suivant;
    }
    printf("NULL\n");
}

void Insertion_Liste_Debut(PListe liste, PArbre nouvel_arbre)
/*

Cette fonction insert un nouvel arbre au début de la liste

*/
{
    PElement nouvel_element = MALLOC(Element);

    if (nouvel_element == NULL)
    {
        printf("Erreur d'allocation de mémoire ou liste NULL");
        exit(1);
    }
    if (liste == NULL) liste = Creer_Liste(nouvel_arbre);

    else
    {
        nouvel_element->arbre = nouvel_arbre;
        nouvel_element->suivant = liste->premier;
        liste->premier = nouvel_element;
    }
}

//-----creer ArbreHuffman
void Ajouter_Dans_Ordre_Croissant(PListe liste, PArbre nouvel_arbre)
/*

liste doit être trié dans l'ordre croissant
Ajoute un arbre dans la liste dans un ordre croissant (en fonction des occurrences)

*/

{
    PElement nouvel_element = MALLOC(Element);
    PListe liste_suivant = MALLOC(Liste);
    if (nouvel_element == NULL || liste_suivant == NULL)
    {
        printf("Erreur d'allocation de mémoire");
        exit(1);
    }

    if (liste == NULL) liste = Creer_Liste(nouvel_arbre); // Si la liste est NULL


    else if (liste->premier->arbre->occurrence >= nouvel_arbre->occurrence) // Si l'occurrence est plus petite que le premier élement

    {
        Insertion_Liste_Debut(liste, nouvel_arbre);

    }
    else if (liste->premier->suivant == NULL) // Si on est à la fin
    {
        nouvel_element->arbre = nouvel_arbre;
        nouvel_element->suivant = NULL;
        liste->premier->suivant = nouvel_element;

    }
    else //Sinon 

    {
        liste_suivant->premier = liste->premier->suivant;
        liste_suivant->premier->suivant = liste->premier->suivant->suivant;

        Ajouter_Dans_Ordre_Croissant(liste_suivant, nouvel_arbre);

        liste->premier->suivant = liste_suivant->premier;

    }

}

void Trie_Liste_Croissant(PListe liste)
/*

Cette fonction trie les fils droit d'un arbre dans l'ordre croissant

*/
{
    if (liste != NULL && liste->premier->suivant != NULL)
    {
        PListe nouvel_liste = Creer_Liste(liste->premier->arbre);
        PElement actuel = liste->premier->suivant;

        while (actuel != NULL)
        {

            Ajouter_Dans_Ordre_Croissant(nouvel_liste, actuel->arbre);
            actuel = actuel->suivant;

        }
        liste->premier = nouvel_liste->premier;
    }

}

int Taille_Liste(PListe liste)
{
    PElement tmp = liste->premier;
    int i = 0;
    while (tmp != NULL)
    {
        i++;
        tmp = tmp->suivant;
    }
    return i;
}

void Supprimer_Liste_Debut(PListe liste)
{
/*

Cette fonction supprime le premier élement d'une liste

*/

    PElement copie_liste = liste->premier;
    liste->premier = liste->premier->suivant;
    free(copie_liste);
}

PArbre Creer_Arbre_Huffman(PListe liste)
{
/*

Cette fonction créée l'arbre de Huffman à partir d'une liste
qui contient toutes les lettres du texte à compresser (liste_occurrence)
Rq : La liste d'entrée doit être trié dans l'ordre croissant

*/

    PElement premier = MALLOC(Element);
    int somme_min;
    PArbre arbre_min = Creer_Arbre(0, 0);

    Trie_Liste_Croissant(liste);

    if (liste == NULL) // Si liste NULL erreur
    {
        printf("Erreur : liste NULL");
        exit(1);
    }

    while (Taille_Liste(liste) != 1)
    {

        premier = liste->premier;
        somme_min = premier->arbre->occurrence + premier->suivant->arbre->occurrence;

        arbre_min = Creer_Arbre(-1, somme_min);// Je mets -1 car aucun caractère ne peut correspondre à -1 dans un texte

        arbre_min->fg = premier->arbre; // On cree un arbre qui a pour fg et fd les lettres les moins présentent dans le texte
        arbre_min->fd = premier->suivant->arbre;
        if (Taille_Liste(liste) == 2) return arbre_min;

        Supprimer_Liste_Debut(liste);// On supprime les deux premier élements
        Supprimer_Liste_Debut(liste);


        Ajouter_Dans_Ordre_Croissant(liste, arbre_min);
    }


    return premier->arbre;

}

//---------------------------------------------------------------------

int* Conversion_En_binaire(int nombredecimal)// cette fonction transforme un nombre decimal entre 0 et 127 !!!
{
    int* tab = malloc(7 * sizeof(int));
    int i;
    if (nombredecimal == 128) nombredecimal = 26;
    if (tab == NULL)
    {
        printf("Erreur d'allocation de memoire");
        exit(1);
    }

    for (i = 0; i<7 ; i++)
    {
        tab[6-i] = nombredecimal%2;
        nombredecimal/=2;
    }

    
    return tab;
}

int finPremierePartie(FILE* fichierEntre) 
{
    char lettre = fgetc(fichierEntre);
    char lettre2 = fgetc(fichierEntre);
    char lettre3 = fgetc(fichierEntre);

    int i;
    for(i=0;i<3;i++) fseek(fichierEntre, -1, SEEK_CUR);
    
    if (lettre == '/' && lettre2 == '.' && lettre3 == '/') return TRUE;
    return FALSE;
}

void Decompression_Binaire(FILE* fichier_compresse)
{
    FILE* fichier_decompresse_binaire = fopen("Decompression Etape1.txt","wb"); // fichier temporaire (binaire)
    FILE* fichierTrans = fopen("fichierTrans.txt","wb");
    char lettre_lu;
    int position_fin;
    int nombre_bits_valide;
    int* tab;
    int debut_binaire;
    int i;
    int j;
    int nCharactereAjoute;
    int nBloc;

    if (fichier_decompresse_binaire == NULL || fichierTrans == NULL)
    {
        printf("Erreur d'ouverture du fichier");
        exit(1);
    }
    fseek(fichier_compresse,0,SEEK_END);
    position_fin = ftell(fichier_compresse);
    fseek(fichier_compresse,0,SEEK_SET);
    
    //On recupere le nombre de 0 en surplus a la fin du fichier
    nCharactereAjoute = fgetc(fichier_compresse) - 48;//Traduction en decimal

    // On place le curseur apres /./ qui parque le debut du fichier ne binaire
    i = 0;
    while  (!finPremierePartie(fichier_compresse))
    {
        fgetc(fichier_compresse);
        i++;
    }
    
    debut_binaire = i+3;
    fseek(fichier_compresse,0,SEEK_SET);//On retourne au premier caractere du fichier.

    //On copie le debut du fichier compresse
    for(i = 0; i<=debut_binaire;i++)
    {
        lettre_lu = fgetc(fichier_compresse);
        fprintf(fichier_decompresse_binaire,"%c",lettre_lu);
    }
    
    nBloc = position_fin - debut_binaire - 1;

    // on decompresse //On met les caracter de la seconde partie sous forme binaire
    for (i=0; i<nBloc;i++)
    {
        tab = Conversion_En_binaire(fgetc(fichier_compresse));
        for (int i = 0; i<7; i++) fprintf(fichier_decompresse_binaire,"%d",tab[i]);
    }
  
    //
    fclose(fichier_decompresse_binaire);
    fichier_decompresse_binaire = fopen("Decompression Etape1.txt","rb");
    
    //On ecrit le fichier qui sera utilise pour la decompression final
    fgetc(fichier_decompresse_binaire);//On ne veut pas le premier caractere
    for(i = 1; i<=debut_binaire;i++)//On copie la premiere partie
    {
        lettre_lu = fgetc(fichier_decompresse_binaire);
        fprintf(fichierTrans,"%c",lettre_lu);
    }
    
    //On copie le binaire en retirant les 0 en trops
    for(i = 0; i < nBloc - 1; i++){
        for(j = 0; j < 7; j++) {
            lettre_lu = fgetc(fichier_decompresse_binaire);
            fprintf(fichierTrans, "%c", lettre_lu);
        }
    }

    for(i = 0; i < 7 - nCharactereAjoute; i++){
        lettre_lu = fgetc(fichier_decompresse_binaire);
        fprintf(fichierTrans, "%c", lettre_lu);
    }

    fclose(fichier_decompresse_binaire);
    fclose(fichierTrans);
    remove("Decompression Etape1.txt");
}
//---------------------------------------------------------------------
int traductionEnNombre(FILE* fichierEntre) 
{
    int nombre;
    char lettre;
    char lettre2;
    int fin = FALSE;

    lettre = fgetc(fichierEntre);
    nombre = lettre - 48;

    while(!fin){
        lettre = fgetc(fichierEntre);
        lettre2 = fgetc(fichierEntre);
        fseek(fichierEntre, -1, SEEK_CUR);

        if(lettre == 47 && lettre2 == 47) break; //fin = TRUE;
        
        nombre = nombre * 10 + lettre - 48;
    }

    fgetc(fichierEntre);
    return nombre;
}

void Afficher_Parcours_Prefixe(PArbre arbre)
{
    if (arbre != NULL)
    {
    
        Afficher_Parcours_Prefixe(arbre->fg);
        Afficher_Parcours_Prefixe(arbre->fd);
    }
}

void Decompresse(void)
{
    FILE* fichierEntre = NULL;
    FILE* fichierSortie = NULL;
    PArbre arbre;
    PArbre arbreHuffman;
    PListe liste_occurrence;
    PElement element = MALLOC(Element);
    int tailleFinal;
    //Creation abre
    char charactere;
    int occurence;
    //Traduction
    char lettre;
    int lettreTrouve = FALSE;
    //verification
    int nombreCarFichier = 0;

    fichierEntre =  fopen("fichierTrans.txt", "rb");

    fichierSortie = fopen("Fichier Decompresse.txt", "wb");

    
    if (fichierSortie == NULL || fichierEntre == NULL)
    {
        printf("Erreur d'ouverture du fichier");
        exit(1);
    }

    //Decompression de la taille du fichier
    tailleFinal = traductionEnNombre(fichierEntre);

    //decompression liste caractere et occurence;
    charactere = fgetc(fichierEntre);

    occurence = traductionEnNombre(fichierEntre);
    //printf("[%c-%d]\n", charactere, occurence);
    arbre = Creer_Arbre(charactere, occurence);
    liste_occurrence = Creer_Liste(arbre);

    //
    while (!finPremierePartie(fichierEntre)) {
        charactere = fgetc(fichierEntre);
        
        occurence = traductionEnNombre(fichierEntre);
        arbre = Creer_Arbre(charactere, occurence);

        Insertion_Liste_Debut(liste_occurrence, arbre);
       

    }
    
    arbreHuffman = Creer_Arbre_Huffman(liste_occurrence);

    //

    int i;
    for(i=0; i<4;i++) {
        lettre = fgetc(fichierEntre);
    }

    while (lettre != EOF) {

        lettreTrouve = FALSE;
        arbre = arbreHuffman;
        
        while (!lettreTrouve) {
            
            if (lettre == '0') arbre = arbre->fg;
            else arbre = arbre->fd;//lettre == '1'
          
            if (!arbre->fg && !arbre->fd) {
                fprintf(fichierSortie, "%c", arbre->lettre);
                lettreTrouve = TRUE;
            }
            else lettre = fgetc(fichierEntre);
            
            
        }
        lettre = fgetc(fichierEntre);
    }
    

    //verification
    fclose(fichierSortie);
    fichierSortie = fopen("Fichier Decompresse.txt", "rb");

    while ((lettre = fgetc(fichierSortie)) != EOF) nombreCarFichier += 1;
    if (nombreCarFichier == tailleFinal) printf("Decompression effectue avec succes\n");
    else printf("Erreur de decompression");
  
    fclose(fichierEntre);
    fclose(fichierSortie);
    remove("fichierTrans.txt");
   
}
//---------------------------------------------------------------------
int main(void) 
{
    FILE* fichier_compresse = fopen("Fichier Compresse.txt","rb");

    if (fichier_compresse == NULL)
    {
        printf("Erreur d'ouverture du fichier\n");
        exit(1);
    }
    printf("Decompression en cours ...\n");
    Decompression_Binaire(fichier_compresse);
    Decompresse();
    fclose(fichier_compresse);
	return 0;
}
