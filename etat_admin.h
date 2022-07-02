#ifndef _ETAT_ADMIN_H
#define _ETAT_ADMIN_H


typedef enum { SURV, VIDCOFFR} etat_type_admin;
//char * etat_nom [] = {"est a table", "pense","a faim","mange"};
//La structure pour les états de l'administrateur
typedef struct etat_admin {
	int no  ; // le numero de l'administrateur
	etat_type_admin e; // son etat de type etat_type_admin
} etat_admin;

int ecrire_etat_admin(etat_admin *e);
int lire_etat_admin(etat_admin *e);

#endif