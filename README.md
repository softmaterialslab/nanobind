# Nanobind

### Description

What does this code do
* The code simulates coarse-grained ligand-receptor binding of ligand-decorated virus particles interacting with a cell wall

* Users can input control parameters such as ligand-receptor binging affinity (E: 1 - 10 KbT), number of ligand-decorated virus particles (S: 1 - 1000), concentration of ligand-decorated virus particles (C: 0.3 - 9 nM), receptor-receptor spacing (W: 10 - 200 nm) and simulation run time (T: 100 - 5000 milliseconds).

* Outputs are the number of bound receptor at equilibrium and simulation snapshots (in lammps movie format). The viruses are modeled after P22 virus-like particles (VLPs) of diameter 60 nanometers, and ligands represent smaller nanoparticles of 6 nanometer diameter. Each virus is fully decorated with 60 ligands.

* Simulations are performed using LAMMPS; pre- and postprocessing are done using C++ codes. 

* At the end of simulation run, which will take close to 60 minutes, the number of bound receptors will be printed in bin/outfiles. 

For further details please refer to the [documentation](https://softmaterialslab.github.io/nanobind/) 

### Install and run instructions on BigRed2

* First, git clone the project: ```https://github.com/softmaterialslab/nanobind.git```
* Go to the root director: ```cd /nanobind```
* Install the project: ```make cluster-install```
* Submit a job: ```make cluster-submit```
* To change the default parameters of E (ligand-receptor binding strength, KbT), S (number of virus-ligand complexes), T (simulation runtime, milliseconds) or C (concentration of virus-ligand, nanomolar); ```make cluster-submit E=5 S=125 T=100 C=0.3```
* All outputs from the simulation will be stored in the bin folder when the simulation is completed.
* If you want to clean everything and create a new build, use: ```make clean```

### Install and run instructions on Local

* First, git clone the project: ```https://github.com/softmaterialslab/nanobind.git```
* Load necessary modules: ```module load gsl && module load mpi/openmpi-x86_64 && module load boost/1_67_0```
* Also make sure to export OMP_NUM_THREADS environment variable with maximum threads available in your CPU: ```export OMP_NUM_THREADS=16```
* Install the project: ```make local-install```
* Run the simulation: ```make local-run-parallel LAMMPS_EXE=lmp_g++```
* Run the simulation in parellel mode: ```make local-run-parallel LAMMPS_EXE=lmp_g++ MPI_EXE=mpirun NODESIZE=4```
* To change the default parameters of E (ligand-receptor binding strength, KbT), S (number of virus-ligand complexes), T (simulation runtime, milliseconds) or C (concentration of virus-ligand, nanomolar); ```make local-run-parallel E=5 S=125 T=100 C=0.3```
* All outputs from the simulation will be stored in the bin folder when the simulation is completed.
* If you want to clean everything and create a new build, use: ```make clean```
* If you want to clean data, use: ```make dataclean```
