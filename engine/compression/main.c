#include "FonctionsArbre.h"
#include "FonctionsListe.h"
#include "FonctionsCompression.h"


int main()
{
    
    PListe liste_occurrence;
    PArbre arbre_huffman;
    PListeConv liste_conversion_binaire;
    int taille_tableau;
 
    FILE* fichier = NULL;
    FILE* fichier_compresse = NULL;
    fichier = fopen("Fichier a compresser.txt", "r");
 
    if (fichier == NULL)
    {
         printf("Erreur d'ouverture du fichier");
         exit(1);
    }
    
    // liste d'occurrence
    liste_occurrence = Creation_Liste_Occurrence(fichier);
    Trie_Liste_Croissant(liste_occurrence);

    // arbre de Huffman
    arbre_huffman = Creer_Arbre_Huffman(fichier);
    taille_tableau = Taille(arbre_huffman);
    
    // liste de conversion lettre - binaire

    liste_conversion_binaire = Creer_tableau_conversion_binaire(liste_occurrence,arbre_huffman);
    
    // crée le fichier Retranscrire 

    Retransciption(fichier,liste_conversion_binaire,arbre_huffman);
    fclose(fichier);

    // crée le fichier compression

    fichier_compresse = Compression();
    fclose(fichier_compresse);

    remove("fichierTrans.txt");
    
    return 0;

}

