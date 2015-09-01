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

int SchedNoMistery::nextTask() {
	if(tareas.empty()) return IDLE_TASK;

	int pid = tareas.front();
	tareas.erase(tareas.begin());
	setQuantum(pid);
	return pid;
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  
	switch(m){
	case TICK: {
		// La tarea actual es la que se está corriendo
		int actual = current_pid(cpu);

		if (actual == IDLE_TASK){
			return nextTask();
		} else {
			quantum -= 1;
			if (quantum <= 0){
				tareas.push_back(actual);
				return nextTask();
			} else {
				return actual;
			}
		}
	}
	break;
	case BLOCK:
	case EXIT:
	return nextTask();
	break;
	default:
	return IDLE_TASK;
	break;
	}
}
