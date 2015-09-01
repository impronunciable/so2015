#include <vector>
#include <map>
#include <iostream>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  
	maxQuantums = argn;
	quantum = maxQuantums[0];
}

void SchedNoMistery::load(int pid) {
	tareas.push_back(pid);
	corridas.insert(make_pair(pid, 0));
}

void SchedNoMistery::unblock(int pid) {
	tareas.insert(tareas.begin(), pid);
}

void SchedNoMistery::setQuantum(int pid) {
	uint procCorridas = corridas[pid];
	if(procCorridas < maxQuantums.size()) {
		quantum = maxQuantums[procCorridas];
	} else {
		quantum = maxQuantums[maxQuantums.size() - 1];
	}
	corridas[pid]++;
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
				setQuantum(buffer);
				return buffer;
			}

		} else {
			quantum -= 1;

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
				setQuantum(sig);
				// Finalmente, reseteamos el quantum
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
			// El siguiente es el primero en la fila
			int sig = tareas[0];
			tareas.erase(tareas.begin());
			setQuantum(sig);

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
			// El siguiente es el primero en la fila
			int sig = tareas[0];
			setQuantum(sig);

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
