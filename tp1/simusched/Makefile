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

ejercicio1:
    
ejercicio2:

ejercicio3:

ejercicio4:
	./simusched ej4.tsk 1 2 0 SchedRR 1 3 | ./graphsched.py > ej4-1core.png
	./simusched ej4.tsk 2 1 0 SchedRR 2 2 | ./graphsched.py > ej4-2core.png
ejercicio5:

ejercicio6:

ejercicio7:

ejercicio8:

new: clean all
