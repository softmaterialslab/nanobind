# This submit a job in bigred2

JOBSCR = nanobind.pbs
TESTSCR = test.pbs
E=1 
C=9 
S=108 
W=100
T=275
NODESIZE=4

run-preprocessor:
	./preprocessor -E -$(E) -C $(C) -S $(S) -W $(W) -T $(T)

submit: 
	@echo "Launching the job";
	qsub $(JOBSCR)

run-local-parallel:
	time mpirun -n $(NODESIZE) lmp_g++ < in.lammps

run-local-serial:
	time lmp_g++ < in.lammps

