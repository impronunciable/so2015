#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <queue>
#include <map>
#include "basesched.h"

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);  
    virtual int proxima();  

  private:
	std::queue<int>* colas;
	int colas_len;
	int quantum; // tiempo restante del cpu
	std::vector<int> quantum_cola;	// duración total del quantum
	std::map<int, int> tareas; // cantidad de ejecuciones de una tarea
};

#endif
