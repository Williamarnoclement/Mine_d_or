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
#include "etat_mineur.h"
#include <time.h>


key_t k_pelles;
key_t k_pioches;
key_t k_coffre;
key_t k_lanternes;
key_t k_registre;

int semid_pelles,
semid_pioches,
semid_lanternes,
semid_coffre,
semid_registre,
shmid_pelles,
shmid_pioches,
shmid_lanternes,
shmid_registre,
shmid_coffre;


void interruption_programme() {

  printf("fin du comptable.\n");
  exit(0);

}

int main(int argc,char * argv[])
{
  int *nb_pelles;
  int *nb_pioches;
  int *nb_lanternes;
  int *coffre;
  int ** registre;

  struct sembuf ops [] = { {0,-1,0} , {0,1,0} };
  int num_ordre = atoi(argv[1]);
  printf("numero d'ordre: %d\n", num_ordre);
  int nb_mineurs = atoi(argv[2]);
  printf("nb mineurs dans comptable : %d\n",nb_mineurs );
  //un peu d'aléatoire
  srand(getpid());

  //fini le programme proprement
  set_signal_handler(SIGINT,interruption_programme);

  //on créé les clés
  k_coffre = nouvelle_cle("tmp/coffre");
/**  k_pelles = nouvelle_cle("tmp/pelles");
  k_pioches = nouvelle_cle("tmp/pioches");
  k_lanternes = nouvelle_cle("tmp/lanternes");**/
  k_registre = nouvelle_cle("tmp/registre");

  //on attribue l'espace memoire partagee pour les trois types de ressources
  /**shmid_pelles = attribuer_memoire_partagee(k_pelles);
  shmid_pioches = attribuer_memoire_partagee(k_pioches);
  shmid_lanternes = attribuer_memoire_partagee(k_lanternes);**/
  shmid_coffre = attribuer_memoire_partagee(k_coffre);
  shmid_registre = attribuer_memoire_partagee(k_registre);

  /**nb_pelles = (int*)shmat(shmid_pelles,NULL,0);
  semid_pelles = semget(k_pelles,1,IPC_CREAT|0600);

  nb_pioches = (int*)shmat(shmid_pioches,NULL,0);
  semid_pioches = semget(k_pioches,1,IPC_CREAT|0600);

  nb_lanternes = (int*)shmat(shmid_lanternes,NULL,0);
  semid_lanternes = semget(k_lanternes,1,IPC_CREAT|0600);
  **/
  coffre = (int*)shmat(shmid_coffre,NULL,0);
  semid_coffre = semget(k_coffre,1,IPC_CREAT|0600);

  registre = (int**)shmat(shmid_registre,NULL,0);
  semid_registre = semget(k_registre,1,IPC_CREAT|0600);

  semctl(semid_coffre,0,SETVAL,1);
  semctl(semid_registre,0,SETVAL,1);


  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" );

  while (1) {
    printf("debug 1\n");
    for (size_t i = 0; i < nb_mineurs; i++) {
      printf("debug 2\n");

      etat_mineur e;
      e.no = 1;
      printf("etat repos %d\n", REP);
      //if (lire_etat_mineur(&e) == 0) printf("NOoOOOOOOOOOOOOOOOOOOO\n" );
      //lire_etat_mineur(&e);
      printf("-----------------numero %d etat_mineur %d\n",e.no, e.et );
    }

  }
}
