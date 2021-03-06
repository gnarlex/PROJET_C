/***********************************************************Imports des bibliothèques**********************************************************/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/***********************************************************Définitions des couleurs d'affichage************************************************/
#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define REDCLIGNOTANT  "\x1B[5;31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"
#define BLACK  "\x1B[30m"

/***********************************************************Variables globales******************************************************************/

// Dimensions de la map
#define LONGUEURMAP 37
#define LARGEURMAP 145

// Dimensions des tanks joueurs et ennemis
#define LONGUEURTANK 3
#define LARGEURTANK 5

#define PETITASCII 15 // Pour les petits caractères ASCII OEM
#define GRANDASCII 20 // Pour les grands caractères ASCII OEM

#define NBOBUSALLOWED 50 // Nombre d'obus autorisé à apparaitre sur la map

#define NBTANKTOTALFACILE 20 // Nombre total de tanks ennemis
#define NBTANKWEAKFACILE 8 // Nombre total de tanks ennemis faibles
#define NBTANKMEDIUMFACILE 7 // Nombre total de tanks ennemis moyens
#define NBTANKSTRONGFACILE 5 // Nombre total de tanks ennemis forts

#define NBCOUPSABIMENT 3 // Nombre de coups pour les blindages faibles se détruisent, et les blindages moyens et forts rétrogradent

// Position du selecteur de choix dans le menu
int choix_x = 10;
int choix_y = 103;

/***********************************************************Variables globales des carrosseries*************************************************/

// On déclare les carrosseries de tank du joueur
char **carrosserieWTH; char **carrosserieWTG; char **carrosserieWTD; char **carrosserieWTB;
char **carrosserieMTH; char **carrosserieMTG; char **carrosserieMTD; char **carrosserieMTB;
char **carrosserieSTH; char **carrosserieSTG; char **carrosserieSTD; char **carrosserieSTB;

char *PATHMAP = "../Modèle/Fichiers_texte_map/map"; // Chemin de la map
char *PATHMENU = "../Modèle/Fichiers_texte_menu/main_menu"; // Chemin du menu principal
char *PATHMENUTERMINAL = "../Modèle/Fichiers_texte_menu/main_menu_mode_terminal"; // Chemin du menu terminal
char *PATHMENUGRAPHIQUE = "../Modèle/Fichiers_texte_menu/main_menu_mode_graphique"; // Chemin du menu graphique

/**********************************************************Structure du tank du joueur**********************************************************/

// Liste chaînée des structures des tanks
struct TANK{
	int pos_x;
	int pos_y;
	char direction;
	
	char **carrosserie;
	int blindage;
	int blindage_origine;
	int nb_impacts;
	
	char camp;
	int etat;

	struct TANK *suivant;
};

struct TANK *head; // Création de la liste

// Tableau indiquant le nombre de tanks faibles, moyens et forts
int *repartitionTankEnnemis;

/***********************************************************************************************************************************************/

// Structure des obus
typedef struct{
	int pos_x;
	int pos_y;
	char direction;
	int provenance; // 0 : tank ennemi faible, 1 : tank ennemi moyen, 2 : tank ennemi fort
	char camp;
} OBUS;

// Pointeur sur l'objet OBUS
typedef OBUS * OBUSP;

// Tableau de pointeurs d'obus permettant de les fair déplacer tous en même temps par accoups
OBUSP *TabPointeursObus;

/***********************************************************************************************************************************************/
// Fonction qui permet de mettre un délai sans bloquer le programme
void delay(int NbSecondes){
	int seuil = 0;
	while(seuil <= NbSecondes){
		int n = rand()%(10);		
		if (n == 5) seuil++;
	}
}

/***********************************************************Variable du pioupiou****************************************************************/

int pioupiouAlive = 1;
