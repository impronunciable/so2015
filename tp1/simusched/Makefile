export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb -std=c++11
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_no_mistery.o
MAIN=simusched

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_rr2.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)

cleanpng:
	rm *.png

ejercicio1:
	./simusched ej5.tsk 1 1 1 SchedFCFS | ./graphsched.py > ej1.png

ejercicio2:
	./simusched ej2.tsk 1 1 1 SchedFCFS | ./graphsched.py > ej2-1core.png
	./simusched ej2.tsk 2 1 1 SchedFCFS | ./graphsched.py > ej2-2core.png

ejercicio3:
	./simusched ej3.tsk 1 1 1 SchedFCFS | ./graphsched.py > ej3.png

ejercicio4:
	./simusched ej4.tsk 1 2 0 SchedRR 3 | ./graphsched.py > ej4-1core.png
	./simusched ej4.tsk 2 1 0 SchedRR 2 5 2 | ./graphsched.py > ej4-2core.png

ejercicio5:
# 1 core
	./simusched ej5.tsk 1 2 0 SchedRR 2 | ./graphsched.py > ej5-2q-1c.png
	./simusched ej5.tsk 1 2 0 SchedRR 10 | ./graphsched.py > ej5-10q-1c.png
	./simusched ej5.tsk 1 2 0 SchedRR 50 | ./graphsched.py > ej5-50q-1c.png

# 3 cores. No necesario.
#	./simusched ej5.tsk 3 2 0 SchedRR 3 2 | ./graphsched.py > ej5-2q-3c.png
#	./simusched ej5.tsk 3 2 0 SchedRR 3 10 | ./graphsched.py > ej5-10q-3c.png
#	./simusched ej5.tsk 3 2 0 SchedRR 3 50 | ./graphsched.py > ej5-50q-3c.png

ejercicio6:
	./simusched ej5.tsk 1 2 0 SchedFCFS | ./graphsched.py > ej6.png

ejercicio7:

	./simusched ej7-1.tsk 1 0 0 SchedMistery 10 | ./graphsched.py > ej7/misterio-1.png
	./simusched ej7-2.tsk 1 0 0 SchedMistery 1 2 | ./graphsched.py > ej7/misterio-2.png
	./simusched ej7-3.tsk 1 1 0 SchedMistery 2 4 6 | ./graphsched.py > ej7/misterio-3.png

	./simusched ej7-1.tsk 1 0 0 SchedNoMistery 10 | ./graphsched.py > ej7/no-misterio-1.png
	./simusched ej7-2.tsk 1 0 0 SchedNoMistery 1 2 | ./graphsched.py > ej7/no-misterio-2.png
	./simusched ej7-3.tsk 1 1 0 SchedNoMistery 2 4 6 | ./graphsched.py > ej7/no-misterio-3.png

ejercicio8:
	./simusched ej8_malo.tsk 2 1 1 SchedRR2 2 2 | ./graphsched.py > ej8MaloRR2.png
	./simusched ej8_malo.tsk 2 1 1 SchedRR 2 2 | ./graphsched.py > ej8MaloRR.png
	./simusched ej8_bueno.tsk 2 1 0 SchedRR2 2 2 | ./graphsched.py > ej8BuenoRR2.png
	./simusched ej8_bueno.tsk 2 1 0 SchedRR 2 2 | ./graphsched.py > ej8BuenoRR.png

new: clean all
