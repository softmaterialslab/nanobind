# Preprocessing

This takes ligand_xyz.coords and generates P22CD40L.in and lammps_script.in

### ligand_xyz.coords

This file stores the xyz coordinates of the possible ligand placements around a single nanoparticle centered at (0,0,0)

### P22CD40L.in

This is the input file containing information about atoms and bonds to be read in by the lammps script

### lammps_script.in

This is the lammps script to be run

## Running the preprocessor

* First, git clone the project:
```git clone https://github.com/softmaterialslab/nanobind.git '''

* Then, load the required modules using following command:
```module load gsl && module load boost/1_67_0'''

* Install the project:
```make cluster-install```

* Run the project:
''' ./generate_lammps_script'''

* If you want to clean everything and create a new build, use:
```make clean```


