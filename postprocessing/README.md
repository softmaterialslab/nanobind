# Postprocessing

This takes ovitomovie.melt, generated from lammps, and generates ovitomovie.melt.processed

### ovitomovie.melt.processed

This is a z-axis histogram of the average positions of vlp's at equilibrium

## Running the postprocessor

* First, git clone the project:
'''git clone https://github.com/softmaterialslab/nanobind.git '''

* Install the project:
'''make'''

* Run the project:
''' ./generate_histogram'''

* If you want to clean everything and create a new build, use:
'''make clean'''
