#include "RWLock.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

// Variables comunes
int recurso1 = 42;
RWLock* testLock;

// Prototipo de funciones de threads
void * threadTest(void*);

// struct de parámetros de funciones de threads
struct testParams {
    int id;     // id del thread
    bool type;  // 0 = read, 1 = write
    int cant;   // Cantidad de lecturas o escrituras
};

// Prototipos de funciones para tests;
// Tests de lecturas y escrituras en paralelo
// crearPrimeroEscritores hace que primero se crean los threads de escritura
// [No garantiza que estos sean los que empiezan]
void testGenerico(int cantLecturas, int cantEscrituras, bool crearPrimeroEscritores = false);

int main(int argc, char* argv[]) {
    std::cout << "Comienzan los tests." << std::endl << std::endl;

    // Varias lecturas
    testGenerico(5, 0);
    // Varias escrituras
    testGenerico(0, 5);
    // Igual cantidad de lecturas que escrituras;
    testGenerico(5, 5);
    testGenerico(5, 5, 1);
    // Muchas escrituras
    testGenerico(1, 10);
    testGenerico(1, 10, 1);
    // Muchas lecturas
    testGenerico(10, 1);
    testGenerico(10, 1, 1);

    // Algunas lecturas, algunas escrituras
    testGenerico(7, 3);
    testGenerico(7, 3, 1);
    testGenerico(3, 7);
    testGenerico(3, 7, 1);

    std::cout << "Terminaron todos los tests." << std::endl;
    return 0;
}

void testGenerico(int cantLecturas, int cantEscrituras, bool crearPrimeroEscritores){
    std::cout << "----Comienza el test----" << std::endl;
    // Cantidad de threads en este test
    const int THREAD_NUM = cantLecturas + cantEscrituras;

    testLock = new RWLock();        // Creamos un nuevo lock
    pthread_t thread[THREAD_NUM];   // Declaramos todos los threads
    testParams params[THREAD_NUM];  // Y todos los parámetros de cada threadTest

    std::cout << "Creando " << cantLecturas << " threads de lectura y ";
    std::cout << cantEscrituras << " threads de escritura." << std::endl;

    // Si creamos primero los escritores, hago un swap
    if (crearPrimeroEscritores){
        std::cout << "Primero los de escritura." << std::endl;
        int buffer = cantLecturas;
        cantLecturas = cantEscrituras;
        cantEscrituras = buffer;
    }

    // Para separar...
    std::cout << std::endl;

    // indica código de error si algún thread falla
    int threadOK;
    // Crear todos los threads
    for (int i = 0; i < THREAD_NUM; ++i){
        std::cout << "Creando thread " << i << std::endl;

        // parámetros
        params[i].id = i;
        params[i].type = (i >= cantLecturas) xor crearPrimeroEscritores;
        params[i].cant = 5;

        // creación
        threadOK = pthread_create(&thread[i], NULL, threadTest, (void*) &params[i]);

        // chequeo de errores
        if (threadOK){
            std::cout << "Error al crear thread " << i << ", con código de error: " << threadOK << std::endl;
            exit(1); // fallamos :(
        }
    }

    // esperar a que todos los threads terminen
    for (int i = 0; i < THREAD_NUM; ++i){
        pthread_join(thread[i], NULL);
    }
    // eliminar el lock
    delete testLock;
    std::cout << "----Termina el test----" << std::endl << std::endl;
}

void* threadTest(void* args){
    testParams* myParams = (testParams*) args;
    int id = myParams->id;
    bool type = myParams->type;
    int cant = myParams->cant;

    std::cout << "Empieza el thread " << id << "!" << std::endl;

    if (type == 0){
        for (int i = 0; i < cant; ++i){
            testLock->rlock();
            std::cout << "Leyendo del thread " << id << std::endl;
            std::cout << "thread " << id <<  ", recurso1: " << recurso1 << std::endl;
            testLock->runlock();
            usleep(1);
        }
    } else {
        for (int i = 0; i < cant; ++i){
            testLock->wlock();
            std::cout << "Escribiendo en el thread " << id << std::endl;
            recurso1++;
            testLock->wunlock();
            usleep(1);
        }
    }

    std::cout << "Termina el thread " << id << std::endl;
    pthread_exit(NULL);
}
