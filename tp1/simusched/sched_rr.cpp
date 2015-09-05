#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round-Robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// Inicializamos todo
	coreNum = argn[0];

	if ((int)argn.size() < coreNum+1){
		cout << "No hay suficientes quantums para la cantidad de núcleos indicada!" << endl;
		exit(1);
	}

	maxQuantum = vector<int>(coreNum);
	quantum = vector<int>(coreNum);

	for (int i = 0; i < coreNum; ++i){
		maxQuantum[i] = argn[i + 1];
		quantum[i] = maxQuantum[i];
	}

}

SchedRR::~SchedRR() {
	// :(
}

void SchedRR::load(int pid) {
	// Carga la tarea para ser usada
	cola.push(pid);
}

void SchedRR::unblock(int pid) {
	// Volver a meter la tarea en la cola
	cola.push(pid);
}

int SchedRR::nextTask(int cpu){
	// Si la cola estuviera vacía
	if (cola.empty()){
		// devolvemos IDLE
		return IDLE_TASK;
	} else {
		// El siguiente es el primero en la fila
		int sig = cola.front();
		// Reseteamos el quantum
		quantum[cpu] = maxQuantum[cpu];
		// Lo quitamos de la fila
		cola.pop();
		// y lo devolvemos
		return sig;
	}
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	// Motivo puede ser TICK, BLOCK ó EXIT...
	switch(m){
	case TICK: {
		// obtenemos la tarea actual
		int actual = current_pid(cpu);

		if (actual == IDLE_TASK){
			return nextTask(cpu);
		} else {
			// Restamos 1 al quantum
			quantum[cpu] -= 1;
			// Si se le acaba el quantum, le toca al próximo
			if (quantum[cpu] <= 0){
				// Agregamos a la cola la tarea que acaba
				// de ser procesada	por falta de quantum
				cola.push(actual);
				// El siguiente es el primero en la fila
				actual = cola.front();
				// Lo quitamos...
				cola.pop();
				// y finalmente, reseteamos el quantum
				quantum[cpu] = maxQuantum[cpu];
				// y devolvemos al próximo proceso
				return actual;
			} else {
				// y seguimos con el mismo proceso
				return actual;
			}
		}
	}
	break;
	case BLOCK:
		return nextTask(cpu);
	break;
	case EXIT:
		return nextTask(cpu);
	break;
	default:
		return IDLE_TASK;
	break;
	}
}
