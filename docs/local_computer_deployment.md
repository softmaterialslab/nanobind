---
title: "Deploying Computational Codes on a Local Computer"
keywords: Linux, npassemblylab
topnav: topnav
hide_sidebar: true
summary: This page has instructions to install the source code for self-assembly of charged nanoparticles (NPs) on a linux computer (here we show specific steps for installation on Ubuntu)
---

## Installing from Source and Building on Linux

The npassemblylab code is open source and can optionally be built from source and used locally or on computer clusters. The following local build instructions should serve as a reference. 

### Necessary Modules
* Load the necessary modules:
```module load gsl && module load openmpi/3.0.1 && module load boost/1_67_0```
* Make sure to have lammps installed and loaded in your systems.
* Make sure to export BOOST_LIBDIR environment variable with location to the lib directory:
```export BOOST_LIBDIR=/opt/boost/gnu/openmpi_ib/lib/```
* Also make sure to export OMP_NUM_THREADS environment variable with maximum threads available in your CPU:
```export OMP_NUM_THREADS=16```

### Install instructions
* Copy or git clone np-assembly-lab project in to a directory. 
* Go to np-assembly-lab directory and (cd np-assembly-lab)
* You should provide the following make command to make the project. This will create the executable and Install the executables into bin directory (That is np-assembly-lab/bin)
    * ```make local-install```
* Now you are ready to run the program sequentially or parallely based on your lammps installation:
```make local-run-parallel Q=1500 n=25 c=0.150 q=35 D=56 d=6.7 i=0 N=150```
```make local-run Q=1500 n=25 c=0.150 q=35 D=56 d=6.7 i=0 N=150```
* All outputs from the simulation will be stored in the bin folder when the simulation is completed.
* If you want to clean everything and create a new build, use:
```make clean``
```make dataclean``
* Once the simulation has finished, outflies folder will contain the simulation results.
