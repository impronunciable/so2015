#include <vector>
#include <array>
#include <iostream>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) { 
	quantum_cola = argn;
	colas_len = argn.size();
	colas = new queue<int>[colas_len];
}

void SchedNoMistery::load(int pid) {
	colas[0].push(pid);
	tareas.insert(make_pair(pid, 0));
}

void SchedNoMistery::unblock(int pid) {
	for(int i = 0; i < bloqueadas.size(); i++) {
		if(bloqueadas[i] == pid) {
			if(tareas[pid] > 0) {
				tareas[pid]--;
			}
			colas[tareas[pid]].push(pid);

			vector<int>::iterator it = bloqueadas.begin() + i;
			bloqueadas.erase(it);
			break;
		}
	}
}

int SchedNoMistery::proxima() {
	for(int i = 0; i < colas_len; i++) {
		if(!colas[i].empty()) {
			int tarea = colas[i].front();
			colas[i].pop();
			quantum = quantum_cola[i];
			return tarea;
		}
	}


	return IDLE_TASK;
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  
	// La tarea actual es la que se está corriendo
	int actual = current_pid(cpu);

	switch(m){
	case TICK: {
		if (actual == IDLE_TASK){
			return proxima();
		} else {
			quantum -= 1;
			if (quantum <= 0){
				tareas[actual] = tareas[actual] >= colas_len - 1 ? colas_len - 1 : tareas[actual] + 1;
				colas[tareas[actual]].push(actual);
				return proxima();
			} else {
				return actual;
			}
		}
	}
	break;
	case BLOCK:
		bloqueadas.push_back(actual);
	case EXIT:
	return proxima();
	break;
	default:
	return IDLE_TASK;
	break;
	}
}
