#include "FonctionsCompression.h"

PElementConv Creer_ElementConv(char nouvel_lettre,int taille_tableau)
/*

	Cette fonction creer un nouvel element contenant le caractere et son equivalence binaire sous la forme d'un tableau.

*/
{
	PElementConv nouvel_elmt = MALLOC(ElementConv);
	int *tab = malloc(taille_tableau * sizeof(int));

	//On verifie qu'il n'y a pas de probleme de memoire
	if (nouvel_elmt == NULL || tab == NULL)
	{
		printf("Erreur d'allocation de memoire");
		exit(1);
	}
	for (int i=0;i<taille_tableau;i++)
	{
		tab[i] = -1;//-1 est la valeur par de faut du tableau.
	}
	nouvel_elmt -> lettre = nouvel_lettre;
	
	nouvel_elmt -> tabNbreBinaire = tab;
	nouvel_elmt -> suivant = NULL;
	return nouvel_elmt;
}

PListeConv Creer_ListeConv(char nouvel_lettre,int taille_tableau)
/*

	Cette fonction creer la liste qui contiendre les elements elementConv necaissaire a la conversion des caracterer en binaire.

*/
{
	PListeConv liste = MALLOC(ListeConv);

	//On verifie qu'il n'y a pas de probleme de memoire
	if (liste == NULL)
	{
		printf("Erreur d'allocation de memoire");
		exit(1);
	}

	liste->premier = Creer_ElementConv(nouvel_lettre,taille_tableau);//On ceer le premier element de la liste.
	liste->tailleTableau = taille_tableau;

	return liste;
}

void Afficher_ListeConv(PListeConv liste)
/*

Cette fonction affiche une listeconv

*/
{
	PElementConv actuel_lu = liste->premier;

	//On verifie qu'il n'y a pas de probleme de memoire.
    if (liste == NULL)
    {
    	printf("Erreur : Afficher une liste NULL impossible");
        exit(1);
    }
   
    while (actuel_lu != NULL ) {//Tant qu'il reste un element dans la liste.

        printf("[%c,(", actuel_lu->lettre);//On ecrit le caractere contenu dans l'element
		for (int i=0; i<liste->tailleTableau-1; i++)//On ecrit son nombre binaire equivalent contenu dans l'element sous la forme d'un tableau.
		{
			printf("%d,", actuel_lu->tabNbreBinaire[i]);
		}
		printf("%d)]->", actuel_lu->tabNbreBinaire[liste->tailleTableau-1]);
		
        actuel_lu = actuel_lu->suivant;
    }
    printf("NULL\n");
}

void Insertion_ListeConv_Debut(PListeConv liste,  PElementConv nouvel_elmt)
/*

Cette fonction insert un nouvel arbre au début de la liste

*/
{
	//On verifie qu'il n'y a pas de probleme de memoire
    if (liste == NULL) 
	{
		printf("Erreur car liste NULL");
		exit(1);
	}
    else 
	{
	nouvel_elmt->suivant = liste->premier;
    liste->premier = nouvel_elmt;
	
	}
}


void Correspondance_Binaire(PArbre arbre,PElementConv elmt) 
/*

	Cette fonction remplie le tableau contenant le nombre binaire correspondant a un caractere.

*/
{
	char lettre_cherche = elmt->lettre;
	int* tab = elmt->tabNbreBinaire;
	int i =0;
	while (arbre->lettre != lettre_cherche)
	{
		if(Est_Dans_Arbre(arbre->fd,lettre_cherche))
		{
			tab[i]=1;
			arbre = arbre->fd;
		}
		
		else if(Est_Dans_Arbre(arbre->fg,lettre_cherche))
		{
			tab[i]=0;
			arbre = arbre->fg;
		}
		i++;
		
	}
	
}
PListeConv Creer_tableau_conversion_binaire(PListe liste_occurrence, PArbre arbre) 

{
	PElement tmp;
	PListeConv listeConv ;
	PElementConv elmtConv;
	int taille_arbre = Taille(arbre);
	int* tab;

	//On verifie qu'il n'y a pas de probleme de memoire
	if (liste_occurrence == NULL)
	{
		printf("Erreur car liste NULL");
		exit(1);
	}
	
	tmp = liste_occurrence->premier;
	listeConv = Creer_ListeConv(tmp->arbre->lettre,Taille(arbre));
	Correspondance_Binaire(arbre,listeConv->premier);
	
	while ((tmp = tmp->suivant) != NULL)
	{
		elmtConv = Creer_ElementConv(tmp->arbre->lettre,taille_arbre);
		Correspondance_Binaire(arbre,elmtConv);
		Insertion_ListeConv_Debut(listeConv,elmtConv);
	
	}
	return listeConv;
	
}


void Retransciption(FILE* fichierEntre,PListeConv liste, PArbre arbreHuffman)
{
    FILE* fichierSortie = fopen("CompressionEtape1.txt","w");//fichier temporaire
    PElementConv element = MALLOC(ElementConv);
	int taille_tableau = liste->tailleTableau;
    char lettre_lu;
    int i;
	PListe liste_occurrence;
	PElement elementOccurence = MALLOC(Element);


	//On verifie qu'il n'y a pas de probleme de memoire
    if (fichierSortie == NULL || element == NULL || elementOccurence == NULL)
    {
        printf("Erreur d'ouverture du fichier ou allocation de memoire");
        exit(1);
    }

    
    ////Ecrit taille fichier
    i = 0;
    while ((lettre_lu = fgetc(fichierEntre)) != EOF) i += 1;//Tant qu'il reste un caractere a parcourir dans le texte.
    fprintf(fichierSortie, "%d//", i);//On ecrit la taille du texte suivi d'un char null afin de signifier que l'on passe a l'etape.
    fseek(fichierEntre, 0, SEEK_SET);//On retourn au premier caractere du texte.

    /////Ecrit les caractere et leur nombre d'occurence
	//On recreer la liste contenant les caracteres et leurs nombres d'occurences car nous l'avons modifier avant afin de creer l'arbre Huffman
    liste_occurrence = Creation_Liste_Occurrence(fichierEntre);
    Trie_Liste_Croissant(liste_occurrence);
    elementOccurence = liste_occurrence->premier;

	
	//On ecrit chaque caractere et leur nombre d'occurence contenu dans la liste suivi d'un char null afin de signifier que l'on passe a l'etape.
    while (elementOccurence) {
		
       	fprintf(fichierSortie, "%c%d//", elementOccurence->arbre->lettre, elementOccurence->arbre->occurrence);
		
        elementOccurence = elementOccurence->suivant;
    }

    //Permet de marquer la transition
    fprintf(fichierSortie, "/./");

    //Creee la seconde partie du fichier compresse soit la retranscription en binaire.
    while ((lettre_lu = fgetc(fichierEntre)) != EOF)//Tant qu'il reste un element a parcourir.
	{
        element = liste->premier;
        while (element && lettre_lu != element->lettre) element = element->suivant;//On parcours la liste jusqu'a trouve le caractere correspondant.
        if (element) 
		{
            i = 0;
            while (i < taille_tableau && element->tabNbreBinaire[i] != -1)//On ecrit l'equivalent binaire du caractere dans le fichier.
			{
                fprintf(fichierSortie, "%d", element->tabNbreBinaire[i]);
                i++;
            }
        }
    }
	fseek(fichierSortie,0,SEEK_SET);
    fclose(fichierSortie);
}

//--------------------------------------------------

int Nombre_De__Bits(FILE* fichier, int debut_binaire)
/*

	Donne le nombre de packet de 7 octets dans un fichier retranscription en binaire.

*/
{	
	int nombre_de_binaire = 0;
	
	fseek(fichier,debut_binaire,SEEK_SET);//On va au debut du binaire
	while (fgetc(fichier) != EOF) nombre_de_binaire++;

	fseek(fichier,0,SEEK_SET);//On retourne au premier caractere du texte.
	return nombre_de_binaire;
}

int Puissance(int a, int b) 
/*

	 Calcul a puissance b

*/
{
	if(b == 0) return 1;
	int a_copie = a;
	for (int i=1; i<b;i++) a = a * a_copie;
	return a;
}

int Conversion_En_Decimal(int nombrebinaire[7])
/*

	Transforme un nombre binaire de 7 bits contenu dans un tableau en décimal

*/
{
	int nombredecimal = 0;
	for (int i = 0; i < 7; i++)
	{
		nombredecimal += nombrebinaire[i] * Puissance(2,6-i);
	}
	if (nombredecimal == 26) nombredecimal = 128;
	return nombredecimal;
}

int* Conversion_En_binaire(int nombredecimal)
/*

	Cette fonction transforme un nombre decimal entre 0 et 127.

*/
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

int Fin_Premiere_Partie(FILE* fichierEntre) 
/*

	Cette fonction est un booleen qui indique si on a atteint la fin de la premiere.

*/
{
    char lettre = fgetc(fichierEntre);
    char lettre2 = fgetc(fichierEntre);
	char lettre3 = fgetc(fichierEntre);

	int i;
	for(i=0;i<3;i++) fseek(fichierEntre, -1, SEEK_CUR);//On retourne au caractere precedent.
   
    if (lettre == '/' && lettre2 == '.' && lettre3 =='/') return TRUE;

    return FALSE;
}

FILE* Compression(void) // Prend en entre le fichier en binaire   ! idée : convertir en base 10 puis a = base 10
/*

	Cette fonction converti la seconde partie du fichier compression en caractere. 

*/
{
	FILE* fichier = fopen("CompressionEtape1.txt","r");
	FILE*  fichierTrans = fopen("Fichier Compresse.txt","w");
	FILE*  fichier_compresse = fopen("fichierCompresse0.txt", "w");
	char lettre_lu;
	int nombre_Bits;
	
	int tab[7];
	int i = 0;
	int j = 0;
	int debut_binaire;
	int nCharManquant;

	//On verifie qu'il n'y a pas de probleme de memoire
    if (fichier == NULL || fichier_compresse == NULL || fichierTrans == NULL)
    {
        printf("Erreur d'ouverture du fichier");
        exit(1);
    }

	// on recopie le texte jusqu'au '//'
	while  (!Fin_Premiere_Partie(fichier))// On place le curseur apres // qui parque le debut du fichier ne binaire
	
	{
		fgetc(fichier);
		i++;
	}
	debut_binaire = i+3;
	fseek(fichier,0,SEEK_SET);//On retourne au premier caractere du fichier.

	nombre_Bits = Nombre_De__Bits(fichier, debut_binaire);

	for(i = 0; i<=debut_binaire-1;i++)
	{
		lettre_lu = fgetc(fichier);
		fprintf(fichier_compresse,"%c",lettre_lu);
	}
	

	// on reecrit les packets de 7 avec leurs correspondants en lettre dans le tableau ascii
	for (i=0; i<nombre_Bits/7; i++)
	{


		for (j=0; j<7; j++) 
		{
			tab[j] = fgetc(fichier) - '0';
		}

		fprintf(fichier_compresse,"%c",Conversion_En_Decimal(tab));

	}

	// on écrit le dernier (complete par des 0)

	nCharManquant = 7 - (nombre_Bits%7);
	for (j=0; j<7; j++) tab[j] = 0;
	for (j=0; j<nCharManquant; j++) 
	{
		tab[j] = fgetc(fichier) - '0';
	}
	
	fprintf(fichier_compresse,"%c",Conversion_En_Decimal(tab));

	//On ecrit au debut du fichier combien de 0 on ete ajoute

	fprintf(fichierTrans, "%d", nCharManquant);

	fclose(fichier_compresse);
	fichier_compresse = fopen("fichierCompresse0.txt","r");
	
	lettre_lu = fgetc(fichier_compresse);
	while(lettre_lu != EOF){
		fprintf(fichierTrans, "%c", lettre_lu);
		lettre_lu = fgetc(fichier_compresse);
	}

	fclose(fichier);
	fclose(fichier_compresse);
	fseek(fichierTrans,0,SEEK_SET);//On retourne au premier caractere du fichier.
	remove("CompressionEtape1.txt");
	remove("fichierCompresse0.txt");

	return fichierTrans;
}

