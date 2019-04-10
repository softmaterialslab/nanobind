---
title: "Deploying Computational Codes to Cluster (BigRed2)"
keywords: BigRed2, npassemblylab
topnav: topnav
hide_sidebar: true
summary: This page has instructions to install the source code for simulating self-assembly of charged nanoparticles (NPs) on high-perfomance computing clusters (here we show specific steps for installation on IU's BigRed2)
---

## Installing from Source and Building on IU BigRed2

The nanoconfinement code is open source and can optionally be built from source and used locally or on computer clusters. The following cluster build instructions should serve as a reference. 

### Necessary Modules
* By default BR2 cluster environment has Cray programming environment module (PrgEnv-cray) loaded 
* Nanoconfinement code has dependency to Boost libraries which requires GNU programming environment
    * Switch modules to GNU - ```module swap PrgEnv-cray PrgEnv-gnu```
* Load latest boost libraries
    * ```module load boost/1.65.0```
* Load GSL libraries
    * ```module load gsl```
* Load cudatoolkit
    * ```module load cudatoolkit/7.0.28-1.0502.10742.5.1```
* Load lammps libraries
    * ```module load lammps/gnu/gpu/15May15```	

### Install instructions
* Copy or git clone np-assembly-lab project in to a directory. 
* Go to np-assembly-lab directory and (cd np-assembly-lab)
* You should provide the following make command to make the project. This will create the executable and Install the executables into bin directory (That is np-assembly-lab/bin)
    * make cluster-install
* Now you are ready to run the executable with aprun command using any of the following methods:

#### Using a debug gpu queue.
* Request a debug GPU using the following command. This will provide you 64 processing nodes with 1 hour wall time.
    * qsub -I -l nodes=4:ppn=16,walltime=01:00:00 -q debug_gpu
* Change you directory to the place where your executable was :
    * np-assembly-lab/bin
* Load the modules using following command :
    * module swap PrgEnv-cray PrgEnv-gnu && module load boost/1.65.0 && module load gsl && module load cudatoolkit/7.0.28-1.0502.10742.5.1 && module load lammps/gnu/gpu/15May15
* execute the program using following command : 
    * time aprun -n 4 lmp_xe6 < in.lammps

#### Using a jobscript
* If you need to change the computational parameter or the physical parameters, you may edit the jobscript file.
* Submit a job:
```make cluster-submit```
* All outputs from the simulation will be stored in the bin folder when the simulation is completed.
* Check and compare files inside the ```bin/outfiles``` directory.
* If you want to clean everything and create a new build, use:
```make clean```
```make dataclean```
* You can check the status of the job by typing the following command.
    * qstat -u "IU_username"
* Once the simulation has finished, data and outflies folders will contain the simulation results.
