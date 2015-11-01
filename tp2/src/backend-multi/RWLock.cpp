#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() {

    readers = 0;
    writing = false;
    pthread_cond_init(&lock, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&turnstile, NULL);
}

void RWLock :: rlock() {
    /* Cambiar por su implementación */
    //pthread_rwlock_rdlock(&(this->rwlock));

    pthread_mutex_lock(&turnstile);
    pthread_mutex_unlock(&turnstile);

    pthread_mutex_lock(&mutex);
    //los lectores se quedan esperando que se termine de escribir
    while(writing){
        pthread_cond_wait(&lock, &mutex);
    }
    readers++;
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {

    // Llega un escritor, no pueden pasar más lectores
    pthread_mutex_lock(&turnstile);

    pthread_mutex_lock(&mutex);
    while(writing || readers > 0){
        pthread_cond_wait(&lock, &mutex);
    }
    writing = true;
    pthread_mutex_unlock(&mutex);
}

void RWLock :: runlock() {

    pthread_mutex_lock(&mutex);
    readers--;
    if (readers == 0){
        pthread_cond_signal(&lock);
    }
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {

    pthread_mutex_lock(&mutex);
    writing = false;
    pthread_cond_broadcast(&lock); // Broadcast para despertar también a readers
    // Desbloqueo el turnstile, para que pasen lectores/escritores
    pthread_mutex_unlock(&turnstile);
    pthread_mutex_unlock(&mutex);
}
