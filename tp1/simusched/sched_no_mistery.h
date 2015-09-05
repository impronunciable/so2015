#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);  
    virtual int nextTask();  

  private:
	std::queue<int> tareas;
	std::queue<int> desbloqueadas;
	std::queue<int> entradas;
	int quantum; // tiempo restante del cpu
	std::vector<int> maxQuantums;	// duración total del quantum
	std::map<int, int> corridas; // cantidad de ejecuciones de una tarea
	void setQuantum(int pid);
};

#endif
