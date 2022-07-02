#include "etat_mineur.h"
#include <unistd.h>


int ecrire_etat_mineur(etat_mineur *e){
	 return write(1,e,sizeof(etat_mineur)) == sizeof(etat_mineur);
}

int lire_etat_mineur(etat_mineur * e){
	 return read(0,e,sizeof(etat_mineur)) == sizeof(etat_mineur);
}