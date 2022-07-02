#include "etat_compt.h"
#include <unistd.h>


int ecrire_etat_compt(etat_compt *e){
	 return write(1,e,sizeof(etat_compt)) == sizeof(etat_compt);
}

int lire_etat_compt(etat_compt * e){
	 return read(0,e,sizeof(etat_compt)) == sizeof(etat_compt);
}