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
		std::queue<int> cola;
		int coreNum; // cantidad de cores
		std::vector<int> quantum; // tiempo restante por cada cpu
		int maxQuantum;	// duración total del quantum
		std::vector<int> runningProcs; // pid del proceso actual corriendo para cada CPU
};

#endif
