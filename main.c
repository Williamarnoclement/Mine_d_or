#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
//gestion d'erreur
#include <errno.h>
//gestion des chaines de caractères
#include <string.h>
//gestion des signals
#include<signal.h>

#include "helper.h"
#include "usual.h"

#include "etat_mineur.h"
#include "etat_compt.h"
#include "etat_admin.h"


key_t k_pelles;
key_t k_pioches;
key_t k_lanternes;
key_t k_coffre;

int semid_pelles,
semid_pioches,
semid_lanternes,
semid_coffre,
shmid_pelles,
shmid_pioches,
shmid_lanternes,
shmid_coffre;

void afficher_etat(int nb_mineurs,int nb_comptables){
	size_t i;

	//affiche l'administrateur
	printf("admin");
	/* Affichage Etat */

	//afficher les comptables
	for ( i = 0; i < nb_comptables; i++) {
		/* Affichage Etat */
	}

	printf("mineurs");
	//afficher les mineurs
	for ( i = 0; i < nb_mineurs; i++) {
		/* Affichage Etat */
	}

	printf("\n");

}


void interruption_programme() {

	//printf("fin du programme.\n");
	/**assert(shmctl(shmid_pelles,IPC_RMID,0) >=0);
	assert(semctl(semid_pelles,IPC_RMID,0) >=0);

	assert(shmctl(shmid_pioches,IPC_RMID,0) >=0);
	assert(semctl(semid_pioches,IPC_RMID,0) >=0);

	assert(shmctl(shmid_lanternes,IPC_RMID,0) >=0);
	assert(semctl(semid_lanternes,IPC_RMID,0) >=0);**/


	assert(shmctl(shmid_coffre,IPC_RMID,0) >=0);

	kill(0,SIGINT);
	exit(0);

}

//     a.  c .... m ...............


int main(int argc,char * argv[])
{
	int i,
	nb_mineurs,
	nb_comptables,
	max_or,
	banque,
	*nb_pelles,
	*nb_pioches,
	*nb_lanternes,
	*coffre;

	char buf[256];

	set_signal_handler(SIGINT,interruption_programme);

	//on créé les clés
	k_pelles = nouvelle_cle("tmp/pelles");
	k_pioches = nouvelle_cle("tmp/pioches");
	k_lanternes = nouvelle_cle("tmp/lanternes");
	k_coffre = nouvelle_cle("tmp/coffre");

	//on attribue l'espace memoire partagee pour les trois types de ressources
	shmid_pelles = attribuer_memoire_partagee(k_pelles);
	shmid_pioches = attribuer_memoire_partagee(k_pioches);
	shmid_lanternes = attribuer_memoire_partagee(k_lanternes);
	shmid_coffre = attribuer_memoire_partagee(k_coffre);

	//on verifie le nombre d'arguments
	if (argc < 7) {
		fprintf(stderr, "Mauvais nombre d'arguments: %d\n", 0);
		exit(1);
	}

	nb_comptables = (int) atoi(argv[1]);
	nb_mineurs = (int) atoi(argv[2]);
	nb_pelles = (int*)shmat(shmid_pelles,NULL,0);
	nb_pioches = (int*)shmat(shmid_pioches,NULL,0);
	nb_lanternes = (int*)shmat(shmid_lanternes,NULL,0);
	//max_or = (int) strtol(argv[6], (char **)NULL, 10);

	*nb_pelles = atoi(argv[3]);
	*nb_pioches = atoi(argv[4]);
	*nb_lanternes = atoi(argv[5]);

	//
	assert(nb_pelles != (void*)-1);
	assert(nb_pioches != (void*)-1);
	assert(nb_lanternes != (void*)-1);

	semctl(semid_coffre,0,SETVAL,1);
	semctl(semid_pelles,0,SETVAL,1);
	semctl(semid_pioches,0,SETVAL,1);
	semctl(semid_lanternes,0,SETVAL,1);

	//on instancie l'administrateur
	pid_t p = fork();
	assert( p != -1);
	if (p == 0) {
		execl("./administrateur","./administrateur",argv[6]/**max_or**/,NULL);
		wait(NULL);
		assert(0);
	}


	// On instancie les mineurs
	for (size_t i = 0; i < nb_mineurs; i++) {
		pid_t p = fork();
		assert( p != -1);

		if (p==0) {
			snprintf(buf,sizeof(buf),"%d",i);
			execl("./mineur","./mineur",buf,NULL);
			assert(0);
		}
	}
	//for (i=1;i<=nb_mineurs;i++) wait(NULL);<----NE FONCTIONNE PAS DANS NOTRE CAS


	// On instancie les comptables
	for (size_t i = 0; i < nb_comptables; i++) {

		pid_t p = fork();

		assert( p != -1);

		if (p==0) {
			snprintf(buf,sizeof(buf),"%d",i);
			execl("./comptable","./comptable",buf/**numéro d'ordre du comptable**/,argv[2]/**nb mineurs**/,NULL);
			assert(0);
		}
	}
	//for (i=1;i<=nb_comptables;i++) wait(NULL);<----NE FONCTIONNE PAS DANS NOTRE CAS

	coffre = (int*)shmat(shmid_coffre,NULL,0);
	//*coffre = 7;
	int incr = 0;
	while(1){

		//printf("Valeur de coffre = %d\n",*coffre);
		if (incr == 0) {
			fprintf(stdout, "Valeur de coffre = %d\n",*coffre );
		}
		incr++;

	}

	printf("Valeur de nb_pelles = %d\n",*nb_pelles);
	printf("Valeur de nb_pioches = %d\n",*nb_pioches);
	printf("Valeur de nb_lanternes = %d\n",*nb_lanternes);
}
