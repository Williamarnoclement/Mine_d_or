#include "usual.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>

key_t nouvelle_cle(char * s[]){
	key_t cle = ftok(s,1);
	assert(cle!=-1);

	return cle;
}

int attribuer_memoire_partagee(key_t k){

	int shmid=shmget(k, sizeof(int), IPC_CREAT|0666);
	assert(shmid >= 0);

	return shmid;
}

int attribuer_semaphore(key_t k){
	int semid = semget(k,1,IPC_CREAT|0600);
	semctl(semid,0,SETVAL,1);
	return semid;
}
