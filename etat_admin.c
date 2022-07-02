#include "etat_admin.h"
#include <unistd.h>


int ecrire_etat_admin(etat_admin *e){
	 return write(1,e,sizeof(etat_admin)) == sizeof(etat_admin);
}

int lire_etat_admin(etat_admin * e){
	 return read(0,e,sizeof(etat_admin)) == sizeof(etat_admin);
}