#ifndef _ETAT_MINEUR_H
#define _ETAT_MINEUR_H


typedef enum {  ATTM, DESC, REG, REP} etat_type_mineur;
//char * etat_nom [] = {"est a table", "pense","a faim","mange"};
//La structure pour les états des différents mineurs
typedef struct etat_mineur {
	int no  ; // le numero du mineur
	etat_type_mineur et; // son etat de type etat_type_mineur
} etat_mineur;

int ecrire_etat_mineur(etat_mineur *e);
int lire_etat_mineur(etat_mineur *e);

#endif