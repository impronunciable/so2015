#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include "basesched.h"

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);  

  private:
	std::vector<int> tareas;
	int quantum; // tiempo restante del cpu
	int maxQuantum;	// duración total del quantum

};

#endif
