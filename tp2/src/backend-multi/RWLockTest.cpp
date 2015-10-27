#include "RWLock.h"
#include <iostream>
#include <unistd.h>

int recurso1 = 42;
RWLock* testLoopDeRecurso1;

void * testThreadA(void*);
void * testThreadB(void*);
void * testThreadC(void*);
void * testThreadD(void*);
void * testThreadE(void*);

int main(int argc, char* argv[]) {

    /* Implementar */
    testLoopDeRecurso1 = new RWLock();
    pthread_t threadA;
    pthread_t threadB;
    pthread_t threadC, threadD, threadE;

    int threadOK;
    threadOK = pthread_create(&threadA, NULL, testThreadA, NULL);
    if (threadOK){
        std::cout << "Error al crear thread A" << std::endl;
    }

    threadOK = pthread_create(&threadB, NULL, testThreadB, NULL);
    if (threadOK){
        std::cout << "Error al crear thread B" << std::endl;
    }

    threadOK = pthread_create(&threadC, NULL, testThreadC, NULL);
    if (threadOK){
        std::cout << "Error al crear thread C" << std::endl;
    }

    threadOK = pthread_create(&threadD, NULL, testThreadD, NULL);
    if (threadOK){
        std::cout << "Error al crear thread D" << std::endl;
    }

    threadOK = pthread_create(&threadE, NULL, testThreadE, NULL);
    if (threadOK){
        std::cout << "Error al crear thread E" << std::endl;
    }

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    delete testLoopDeRecurso1;

    return 0;
}


void* testThreadA(void*){
    std::cout << "Empieza Thread A" << std::endl;
    for (int i = 0; i < 5; ++i){
        testLoopDeRecurso1->rlock();
        std::cout << "Leyendo del thread A... " << i << std::endl;
        std::cout << "thA, recurso1: " << recurso1 << std::endl;
        testLoopDeRecurso1->runlock();
        usleep(1);
    }
    return NULL;
}

void* testThreadB(void*){
    std::cout << "Empieza Thread B" << std::endl;
    for (int i = 0; i < 5; ++i){
        testLoopDeRecurso1->rlock();
        std::cout << "Leyendo del thread B... " << i << std::endl;
        std::cout << "thB, recurso1: " << recurso1 << std::endl;
        testLoopDeRecurso1->runlock();
        usleep(1);
    }
    return NULL;
}


void* testThreadC(void*){
    std::cout << "Empieza Thread C" << std::endl;
    for (int i = 0; i < 5; ++i){
        testLoopDeRecurso1->wlock();
        std::cout << "Escribiendo en el thread C... " << i << std::endl;
        recurso1++;
        testLoopDeRecurso1->wunlock();
        usleep(1);
    }
    return NULL;
}

void* testThreadD(void*){
    std::cout << "Empieza Thread D" << std::endl;
    for (int i = 0; i < 5; ++i){
        testLoopDeRecurso1->wlock();
        std::cout << "Escribiendo en el thread D... " << i << std::endl;
        recurso1++;
        testLoopDeRecurso1->wunlock();
        usleep(1);
    }
    return NULL;
}

void* testThreadE(void*){
    std::cout << "Empieza Thread E" << std::endl;
    for (int i = 0; i < 5; ++i){
        testLoopDeRecurso1->wlock();
        std::cout << "Escribiendo en el thread E... " << i << std::endl;
        recurso1++;
        testLoopDeRecurso1->wunlock();
        usleep(1);
    }
    return NULL;
}
