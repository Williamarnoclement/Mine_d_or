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
shmid_pelles,
shmid_pioches,
shmid_lanternes,
shmid_coffre,
semid_registre,
shmid_registre;

int paye = 0;
int num_ordre;

void interruption_programme() {

  printf("Le mineur %d est gagné %d roubles.\n",num_ordre, paye);
  printf("fin du mineur.\n");
  exit(0);

}

int main(int argc,char * argv[])
{
  int *nb_pelles;
  int *nb_pioches;
  int *nb_lanternes;
  int *coffre;
  int **registre;


  int or_recupere = 0;
  int nb_desc = 0;

  struct sembuf ops [] = { {0,-1,0} , {0,1,0} };
  num_ordre = atoi(argv[1]);
  printf("numero d'ordre: %d\n", num_ordre);

  //un peu d'aléatoire
  srand(getpid());

  //fini le programme proprement
  set_signal_handler(SIGINT,interruption_programme);

  //on créé les clés
  k_coffre = nouvelle_cle("tmp/coffre");
  k_pelles = nouvelle_cle("tmp/pelles");
  k_pioches = nouvelle_cle("tmp/pioches");
  k_lanternes = nouvelle_cle("tmp/lanternes");
  k_registre = nouvelle_cle("tmp/registre");

  //on attribue l'espace memoire partagee pour les trois types de ressources
  shmid_pelles = attribuer_memoire_partagee(k_pelles);
  shmid_pioches = attribuer_memoire_partagee(k_pioches);
  shmid_lanternes = attribuer_memoire_partagee(k_lanternes);
  shmid_coffre = attribuer_memoire_partagee(k_coffre);
  shmid_registre = attribuer_memoire_partagee(k_registre);

  nb_pelles = (int*)shmat(shmid_pelles,NULL,0);
  semid_pelles = semget(k_pelles,1,IPC_CREAT|0600);

  nb_pioches = (int*)shmat(shmid_pioches,NULL,0);
  semid_pioches = semget(k_pioches,1,IPC_CREAT|0600);

  nb_lanternes = (int*)shmat(shmid_lanternes,NULL,0);
  semid_lanternes = semget(k_lanternes,1,IPC_CREAT|0600);

  coffre = (int*)shmat(shmid_coffre,NULL,0);
  semid_coffre = semget(k_coffre,1,IPC_CREAT|0600);

  registre = (int**)shmat(shmid_registre,NULL,0);
  semid_registre = semget(k_registre,1,IPC_CREAT|0600);

  etat_mineur e = { num_ordre, ATTM};

  semctl(semid_coffre,0,SETVAL,1);
  semctl(semid_pelles,0,SETVAL,1);
  semctl(semid_pioches,0,SETVAL,1);
  semctl(semid_lanternes,0,SETVAL,1);
  semctl(semid_registre,0,SETVAL,1);

  printf("before boucle\n" );
  int incr = 0;

  while (1) {
    if (incr < 30) {//imprime les n premières itérations de la boucle
      //printf("Il y a %d lanternes, %d pelles et %d pioches disponible.\n",*nb_lanternes, *nb_pelles, *nb_pioches );
      //en une opération, on verifie qu'on peut prendre les ressources nécéssaires
      semop(semid_lanternes,ops,1);
      semop(semid_pelles,ops,1);
      semop(semid_pioches,ops,1);
      if ((*nb_lanternes > 0) && (*nb_pelles > 0) && (*nb_pioches > 0) && (e.et == ATTM)) {
        //semop
        printf("Le mineur %d prends les ressources pour descendre.\n", num_ordre);
        *nb_lanternes = *nb_lanternes - 1;
        *nb_pelles = *nb_pelles - 1;
        *nb_pioches = *nb_pioches - 1;

        e.et = DESC;
        ecrire_etat_mineur(&e);

      }
      semop(semid_pioches,ops+1,1);
      semop(semid_pelles,ops+1,1);
      semop(semid_lanternes,ops+1,1);

      if (e.et == DESC) {
        printf("boucle test\n" );
        if (nb_desc < 5) {
          //remonte or
          or_recupere += rand() % 100 + 1;
          printf("Le mineur %d descend.\n", num_ordre);
          //incremente nb_desc
          nb_desc++;
        } else {
          semop(semid_lanternes,ops,1);
          semop(semid_pelles,ops,1);
          semop(semid_pioches,ops,1);
          *nb_lanternes = *nb_lanternes + 1;
          *nb_pioches = *nb_pioches + 1;
          *nb_pelles = *nb_pelles + 1;
          semop(semid_pioches,ops+1,1);
          semop(semid_pelles,ops+1,1);
          semop(semid_lanternes,ops+1,1);
          printf("Le mineur repose son materiel.\n" );
          //fin de la descente, le mineur remonte pour s'enregistrer
          e.et = REG;
          ecrire_etat_mineur(&e);
          printf("le mineur %d remonte avec %d grammes d'or.\n",num_ordre, or_recupere );
          paye += 10 * or_recupere;//on laisse le temps de faire comptable
          //il dépose l'or dans le coffre
          printf("le mineur %d dépose l'or dans le coffre.\n",num_ordre );
          semop(semid_coffre,ops,1);
          *coffre += or_recupere;
          semop(semid_coffre,ops+1,1);
          or_recupere = 0;
          //le nb_desc se remet à 0
          nb_desc = 0;
          //part se reposer
          e.et = REP;
          ecrire_etat_mineur(&e);
          printf("le mineur %d part se reposer.\n",num_ordre);
        }
      }
    }
    incr++;
  }
  printf("after boucle\n" );
}
