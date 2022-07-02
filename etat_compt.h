#ifndef _ETAT_COMPT_H
#define _ETAT_COMPT_H


typedef enum { ATT, PAYE } etat_type_compt;
//char * etat_nom [] = {"est a table", "pense","a faim","mange"};
//La structure pour les états des différents comptables
typedef struct etat_compt {
	int no  ; // le numero du comptable
	etat_type_compt et; // son etat de type etat_type_compt
} etat_compt;

int ecrire_etat_compt(etat_compt *e);
int lire_etat_compt(etat_compt *e);

#endif
