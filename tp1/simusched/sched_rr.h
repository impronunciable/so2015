#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		// Función privada
		// Más que nada por una cuestión de legibilidad
		// Devuelve el id de la próxima tarea a ejecutar
		int nextTask(int cpu);

		// Cola de procesos a ejecutar
		std::queue<int> cola;
		// cantidad de cores
		int coreNum;
		// tiempo restante por cada cpu
		std::vector<int> quantum;
		// duración total del quantum
		int maxQuantum;
};

#endif
