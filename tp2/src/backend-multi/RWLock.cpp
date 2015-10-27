#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() {
    /* Cambiar por su implementación */
    //pthread_rwlock_init(&(this->rwlock),NULL);

    readers = 0;
    writing = false;
    pthread_cond_init(&lock, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&estaEscribiendo, NULL);
    hayEscritor = false;
}

void RWLock :: rlock() {
    /* Cambiar por su implementación */
    //pthread_rwlock_rdlock(&(this->rwlock));

    pthread_mutex_lock(&mutex);
    while(hayEscritor || writing){
        pthread_cond_wait(&lock, &mutex);
    }
    readers++;
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {
    /* Cambiar por su implementación */
    //pthread_rwlock_wrlock(&(this->rwlock));

    pthread_mutex_lock(&estaEscribiendo); //Llegué

    pthread_mutex_lock(&mutex);
    hayEscritor = true;

    while(writing || readers > 0){
        pthread_cond_wait(&lock, &mutex);
    }
    writing = true;
    // escribir...
    pthread_mutex_unlock(&mutex);
}

void RWLock :: runlock() {
    /* Cambiar por su implementación */
    //pthread_rwlock_unlock(&(this->rwlock));

    pthread_mutex_lock(&mutex);
    readers--;
    if (readers == 0){
        pthread_cond_signal(&lock);
    }
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {
    /* Cambiar por su implementación */
    //pthread_rwlock_unlock(&(this->rwlock));

    pthread_mutex_lock(&mutex);

    writing = false;
    hayEscritor = false;
    pthread_cond_broadcast(&lock); //broadcast para despertar también a readers

    pthread_mutex_unlock(&estaEscribiendo);
    pthread_mutex_unlock(&mutex);
}
