#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round-Robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// Inicializamos todo
	coreNum = argn[0];
	maxQuantum = argn[1];
	quantum = vector<int>(coreNum, maxQuantum);
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
		quantum[cpu] = maxQuantum;
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
				quantum[cpu] = maxQuantum;
				// y devolvemos al próximo proceso
				return actual;
			} else {
				// Si todavía le queda, restamos 1
				quantum[cpu] -= 1;
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
