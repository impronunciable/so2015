#include "tasks.h"
#include <stdlib.h>
#include <algorithm>

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConsola(int pid, vector<int> params) { // params: n, bmin, bmax
	int tiempo;
	for(int i = 0; i < params[0]; ++i) {
		tiempo = params[1] + (rand() % ((params[2] + 1) - params[1]));
		uso_IO(pid, tiempo);
	}

}

void TaskBatch(int pid, vector<int> params) { // params: total_cpu, cant_bloqueos
	// Tiempo discretizado
	auto ticks = vector<bool>(params[0]);
	// vector para obtener posiciones aleatorias
	auto posTiempo = vector<int>(params[0]);
	//inicializamos ambos vectores
	for (int i = 0; i < params[0]; ++i){
		ticks[i] = false;
		posTiempo[i] = i;
	}

	//desordena el vector en tiempo lineal
	random_shuffle(posTiempo.begin(), posTiempo.end());
	for(int i = 0; i < params[1]; ++i){
		ticks[posTiempo[i]] = true;
	}

	/*
	for(int i = 0; i < params[1]; ++i) {
		int j = rand() % params[0];
		bool tick = ticks[j];
		if(tick == true) {
			i--;
		} else {
			ticks[j] = true;
		}
	}
	*/

	//Para cada instante de tiempo, usamos cpu o bloquemos
	for(int i = 0; i < params[0]; ++i) {
		if(ticks[i]) {
			uso_IO(pid, 1);
		} else {
			uso_CPU(pid, 1);
		}
	}

}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
}
