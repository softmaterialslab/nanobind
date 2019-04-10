# This submit a job in bigred2

JOBSCR = nanobind.pbs
TESTSCR = test.pbs
E=1 
C=9 
S=108 
w=100
T=275
NODESIZE=4
MPI_EXE=mpirun
LAMMPS_EXE=lmp_g++

run-preprocessor:
	./preprocessor -E -$(E) -C $(C) -S $(S) -w $(w) -T $(T)

submit: 
	@echo "Launching the job";
	qsub $(JOBSCR)

run-local-parallel:
	time $(MPI_EXE) -n $(NODESIZE) $(LAMMPS_EXE) < in.lammps

run-local-serial:
	time $(LAMMPS_EXE) < in.lammps

