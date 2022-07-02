#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <assert.h>
#include <sys/wait.h>
#include "usual.h"
#include "helper.h"
//gestion des signals
#include<signal.h>

int banque = 0;

key_t k_coffre;

int 	shmid_coffre,
		semid_coffre;

void interruption_programme() {

		printf("Or en banque : %d\n",banque );
		printf("fin de l'administrateur.\n");

		exit(0);

}

int main(int argc,char * argv[])
{
	int *coffre;
	struct sembuf ops [] = { {0,-1,0} , {0,1,0} };
	char buf[256];
	int max_or = atoi(argv[1]);

	set_signal_handler(SIGINT,interruption_programme);


	k_coffre = nouvelle_cle("tmp/coffre");

	assert(k_coffre!=-1);
	shmid_coffre=shmget(k_coffre, sizeof(int), IPC_CREAT|0666);
	assert(shmid_coffre >= 0);

	coffre = (int*)shmat(shmid_coffre,NULL,0);
	semid_coffre = semget(k_coffre,1,IPC_CREAT|0600);

	assert(semid_coffre != -1);

	semctl(semid_coffre,0,SETVAL,1);
	assert(coffre != (void*)-1);

	while (1) {
		if (*coffre > max_or) {
			printf("L'administrateur vide le coffre pour mettre l'or en banque.\n");
			//semop
			semop(semid_coffre,ops,1);
			banque += *coffre;
			*coffre = 0;
			//fin semop
			semop(semid_coffre,ops+1,1);
		}
	}

}
