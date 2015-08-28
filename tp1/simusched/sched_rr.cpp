#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	coreNum = argn[0];
	maxQuantum = argn[1];
	quantum = vector<int>(coreNum, maxQuantum);
}

SchedRR::~SchedRR() {

}

void SchedRR::load(int pid) {
	// Carga la tarea para ser usada
	cola.push(pid);
}

void SchedRR::unblock(int pid) {
	// Volver a meter la tarea en la cola
	cola.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	// Motivo puede ser TICK, BLOCK, EXIT...
	//cout << "size: " << cola.size() << endl;
	switch(m){
	case TICK: {
		// La tarea actual es la que se está corriendo
		int actual = current_pid(cpu);
		// por defecto, el siguiente es la actual
		int sig = actual;

		if (actual == IDLE_TASK){

			if (cola.empty()){
				return IDLE_TASK;
			} else {
				//cout << "Salgo " << current_pid(cpu) << endl;
				int buffer = cola.front();
				////cout << "Sacamos a " << cola.front() << endl;
				cola.pop();
				//cout << endl;
				quantum[cpu] = maxQuantum;
				return buffer;
			}

		} else {

			// Si se le acaba e:l quantum, le toca al próximo
			//cout << "me qued a"<<quantum[cpu] << endl;
			if (quantum[cpu] <= 0){
				// Agregamos a la cola la tarea que acaba
				// de ser procesada	por falta de quantum
				cola.push(actual);
				// El siguiente es el primero en la fila
				//cout << actual << "actual"<< endl;
				sig = cola.front();
				//cout << sig << "siguiente"<<endl;
				cola.pop(); //Experimental
				// Finalmente, reseteamos el quantum
				quantum[cpu] = maxQuantum;
			} else {
				quantum[cpu] -= 1;
			}
		}
		return sig;
	}
	break;
	case BLOCK:
		if (cola.empty()){
			return IDLE_TASK;
		} else {
			// Reseteamos el quantum
			quantum[cpu] = maxQuantum;
			// El siguiente es el primero en la fila
			int sig = cola.front();
			cola.pop();
			return sig;
		}
	break;
	case EXIT:
		//cout << "terminé!" << endl;
		//cout << "Soy: " << current_pid(cpu) << endl;
		// Si no hay más elementos, terminamos
		if (cola.empty()){
			return IDLE_TASK;
		} else {
			// Reseteamos el quantum
			quantum[cpu] = maxQuantum;
			// El siguiente es el primero en la fila
			int sig = cola.front();
			//cout << "Sacamos a " << sig << endl;
			cola.pop();
			return sig;
		}
	break;
	default:
		return IDLE_TASK;
	break;
	}
}
