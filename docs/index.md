---
title: "self-assembly of charged nanoparticles (NPs)"
keywords: npassemblylab
topnav: topnav
hide_sidebar: true
permalink: index.html
---

## Synopsis

The code simulates the self-assembly of charged nanoparticles (NPs) into aggregates mediated by smaller, oppositely-charged linkers under different ionic physiological conditions. 

Users can input control parameters such as NP charge (from -500e to -1500e), linker density (from 25X to 100X the NP density), and ionic strength (from 0.01 M to 0.3 M) to predict formation of NP aggregates. 

This information may be useful in designing NP features to produce desired effects when NPs interface with biological entities. Outputs are structural information such as pair correlation functions (often denoted as g(r)) and simulation snapshots (with only NPs shown for clarity). The NPs are modeled after P22 virus-like particles (VLPs) of diameter 56 nanometers, and linkers represent smaller nanoparticles (dendrimers) of 6 nanometer diameter. Linker charge is fixed to about 35e.

Simulations are performed using LAMMPS; pre- and postprocessing are done using C++ codes. 

Simulation results are tested and validated using SAXS and dynamic light scattering measurements of the VLP aggregates; experiments also guide the model design.

At the end of simulation run, which will take close to 60 minutes, g(r) will be produced in the "Pair Correlation" tab. 

## Deploying the simulation code as Apps on nanoHUB
To deploy the code as a user-friendly app on nanoHUB Cyber Platform, refer to [nanoHUB Deployment](nanohub_deployment)

## Deploying the simulation code on Clusters (BigRed2)
To deploy the code on supercomputing clusters (e.g. BigRed2) to obtain accelerated performance and simulate larger systems for longer times, refer to [Cluster Deployment](cluster_bigred2_deployment)

## Deploying the simulation code on a Local Computer (Linux)
To deploy the code on a local computer (e.g. Linux) to develop and debug, refer to [Linux Deployment](local_computer_deployment)
