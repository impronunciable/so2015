#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <map>
#include "basesched.h"

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	private:
		// cantidad de cores
		int coreNum;
		// cola por cada cpu [READY + RUNNING]
		std::vector<std::queue<int> > colas;
		// cantidad de procesos activos [RUNNING + BLOCKED + READY] por cpu
		std::vector<int> activos;
		// Mapa de procesos. Indica a qué CPU corresponde cada proceso
		// (PROCESO, CPU);
		std::map<int, int> procesos;

		// tiempo restante por cada cpu
		std::vector<int> quantum;
		// duración total del quantum
		int maxQuantum;
};

#endif
