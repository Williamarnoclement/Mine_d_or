#include <sys/types.h>
#ifndef _USUAL_H
#define _USUAL_H

key_t nouvelle_cle(char * s[]);
int attribuer_memoire_partagee(key_t k);
int attribuer_semaphore(key_t k);

#endif
