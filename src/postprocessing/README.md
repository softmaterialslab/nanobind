# Postprocessing

This takes ovitomovie.melt, generated from lammps, and generates ovitomovie.melt.processed

### ovitomovie.melt.processed

This generates a file with 3 lines; timestep (1) number of receptor bound (2) number of total receptors (3)

## Running the postprocessor

* First, git clone the project:
```git clone https://github.com/softmaterialslab/nanobind.git ```

* Install the project:
```make```

* Run the project:
``` ./generate_histogram```

* If you want to clean everything and create a new build, use:
```make clean```
