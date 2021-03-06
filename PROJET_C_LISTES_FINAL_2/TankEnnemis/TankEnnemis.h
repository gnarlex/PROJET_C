#include "TankEnnemis.c"

struct TANK *creer_tank_joueur(char **mat, struct TANK **head, int pos_x, int pos_y, char direction);
void creer_tank_ennemis(char **mat, struct TANK **head, int pos_x, int pos_y, char direction);
void deplacer_tank_ennemis_terminal(char **fake_map, struct TANK *tankSelectionne);
void deplacer_tank_joueur_terminal(char **fake_map, struct TANK **joueurP);
int NbEnnemisTanksToCreate();
