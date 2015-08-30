#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>
#include <algorithm>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// Inicialización de cosas
	coreNum = argn[0];
	maxQuantum = argn[1];
	quantum = vector<int>(coreNum, maxQuantum);

	colas = std::vector<std::queue<int> >(coreNum);
	cout << "coreNum: " << coreNum << endl;
	activos = std::vector<int>(coreNum);
}

SchedRR2::~SchedRR2() {
	// Los destructores son inútiles :/
	// Quiero destrucción y caos...
}

void SchedRR2::load(int pid) {
	// Obtengo qué core tiene menos procesos activos
	int core = 0;
	int minElem = activos[0]; //Por lo menos hay un core
	for (unsigned int i = 0; i < activos.size(); ++i){
		if (activos[i] < minElem){
			minElem = activos[i];
			core = i;
		}
	}
	//cout << "minCore: " << core << endl;
	// Carga la tarea para ser usada en el core determinado
	colas[core].push(pid);
	// Agrego uno a la cantidad de procesos del core seleccionado
	activos[core] += 1;
	// Por último, decimos que este proceso corresponde a este core
	procesos[pid] = core;
}

void SchedRR2::unblock(int pid) {
	// Volver a meter la tarea en la cola del cpu correspondiente
	colas[procesos[pid]].push(pid);
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	// Motivo puede ser TICK, BLOCK, EXIT...
	switch(m){
	case TICK: {
		// La tarea actual es la que se está corriendo
		int actual = current_pid(cpu);
		// por defecto, el siguiente es la actual
		int sig = actual;

		if (actual == IDLE_TASK){

			if (colas[cpu].empty()){
				return IDLE_TASK;
			} else {
				int buffer = colas[cpu].front();
				colas[cpu].pop();
				quantum[cpu] = maxQuantum;
				return buffer;
			}

		} else {

			// Si se le acaba el quantum, le toca al próximo
			if (quantum[cpu] <= 0){
				// Agregamos a la cola la tarea que acaba
				// de ser procesada	por falta de quantum
				colas[cpu].push(actual);
				// El siguiente es el primero en la fila
				sig = colas[cpu].front();
				colas[cpu].pop();
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
		if (colas[cpu].empty()){
			return IDLE_TASK;
		} else {
			// Reseteamos el quantum
			quantum[cpu] = maxQuantum;
			// El siguiente es el primero en la fila
			int sig = colas[cpu].front();
			colas[cpu].pop();
			return sig;
		}
	break;
	case EXIT:
		// Ya no es un proceso activo,
		// por lo que restamos uno a ese core
		activos[cpu] -= 1;
		// Está bueno quitar este proceso del mapa de procesos
		// Así desbloquear sería más rápido
		// Hacer esto sería logarítmico en la cantidad de procesos activos
		// totales, lo cual realentiza la finalización de un proceso, pero
		// creo que vale la pena
		procesos.erase(current_pid(cpu));

		// Si no hay más elementos, terminamos
		if (colas[cpu].empty()){
			return IDLE_TASK;
		} else {
			// Reseteamos el quantum
			quantum[cpu] = maxQuantum;
			// El siguiente es el primero en la fila
			int sig = colas[cpu].front();
			colas[cpu].pop();
			return sig;
		}
	break;
	default:
		return IDLE_TASK;
	break;
	}
}
