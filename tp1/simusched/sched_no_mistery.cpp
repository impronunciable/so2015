#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  
	maxQuantum = argn[0];
	quantum = maxQuantum;
}

void SchedNoMistery::load(int pid) {
	tareas.push_back(pid);
}

void SchedNoMistery::unblock(int pid) {
	tareas.push_back(pid);
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  
	switch(m){
	case TICK: {
		// La tarea actual es la que se está corriendo
		int actual = current_pid(cpu);
		// por defecto, el siguiente es la actual
		int sig = actual;

		if (actual == IDLE_TASK){

			if (tareas.empty()){
				return IDLE_TASK;
			} else {
				int buffer = tareas.front();
				tareas.erase(tareas.begin());
				quantum = maxQuantum;
				return buffer;
			}

		} else {

			// Si se le acaba e:l quantum, le toca al próximo
			if (quantum <= 0){
				// Agregamos a la cola la tarea que acaba
				// de ser procesada	por falta de quantum
				tareas.push_back(actual);
				// El siguiente es el primero en la fila
				//cout << actual << "actual"<< endl;
				sig = tareas[0];
				//cout << sig << "siguiente"<<endl;
				tareas.erase(tareas.begin());
				// Finalmente, reseteamos el quantum
				quantum = maxQuantum;
			} else {
				quantum -= 1;
			}
		}
		return sig;
	}
	break;
	case BLOCK:
		if (tareas.empty()){
			return IDLE_TASK;
		} else {
			// Reseteamos el quantum
			quantum = maxQuantum;
			// El siguiente es el primero en la fila
			int sig = tareas[0];
			tareas.erase(tareas.begin());
			return sig;
		}
	break;
	case EXIT:
		//cout << "terminé!" << endl;
		//cout << "Soy: " << current_pid(cpu) << endl;
		// Si no hay más elementos, terminamos
		if (tareas.empty()){
			return IDLE_TASK;
		} else {
			// Reseteamos el quantum
			quantum = maxQuantum;
			// El siguiente es el primero en la fila
			int sig = tareas[0];
			//cout << "Sacamos a " << sig << endl;
			tareas.erase(tareas.begin());
			return sig;
		}
	break;
	default:
		return IDLE_TASK;
	break;
	}

}
