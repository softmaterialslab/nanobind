#This make file builds the sub folder make files
#example make call for bigred2
# make cluster-submit Q=1500 n=25 c=0.150 q=35 D=56 d=6.7 i=0 N=150

PROGPRE = preprocessor
PROGPOST = postprocessor
JOBSCR = nanobind.pbs
TESTSCR = test.pbs
BIN = bin
PRE = src/preprocessing
POST = src/postprocessing
SCRIPT = scripts
E=1 
C=9 
S=108 
W=100
T=275
NODESIZE=4

all:
	@echo "Starting build of the project";
ifeq ($(CCF),BigRed2)	
	+$(MAKE) -C $(PRE) cluster-install
	+$(MAKE) -C $(POST) cluster-install
else ifeq ($(CCF),nanoHUB)
	+$(MAKE) -C $(PRE) nanoHUB-install
	+$(MAKE) -C $(POST) nanoHUB-install
else
	+$(MAKE) -C $(PRE) install
	+$(MAKE) -C $(POST) install
endif
	@echo "Ending the build of the project";

local-install: create-dirs
	make all

cluster-install: create-dirs
	make CCF=BigRed2 all

nanoHUB-install: create-dirs
	make CCF=nanoHUB all

create-dirs:
	@echo "Checking and creating needed sub-directories in the $(BIN) directory"
	if ! [ -d $(BIN) ]; then mkdir $(BIN); fi
	if ! [ -d $(BIN)/outfiles ]; then mkdir $(BIN)/outfiles; fi
	if ! [ -d $(BIN)/infiles ]; then mkdir $(BIN)/infiles; fi
	@echo "Directory creation is over."

cluster-submit:
	@echo "Running the preprocessor to create lammps script."
	+$(MAKE) -C $(BIN) run-preprocessor E=$(E) C=$(C) S=$(S) W=$(W) T=$(T)
	@echo "Running the preprocessor is over."
	@echo "Installing jobscript into $(BIN) directory"
	cp -f $(SCRIPT)/$(JOBSCR) $(BIN)
	+$(MAKE) -C $(BIN) submit

local-run-parallel:
	@echo "Running the preprocessor to create lammps script."
	+$(MAKE) -C $(BIN) run-preprocessor E=$(E) C=$(C) S=$(S) W=$(W) T=$(T)
	@echo "Running the preprocessor is over."
	+$(MAKE) -C $(BIN) run-local-parallel NODESIZE=$(NODESIZE)

local-run:
	@echo "Running the preprocessor to create lammps script."
	+$(MAKE) -C $(BIN) run-preprocessor E=$(E) C=$(C) S=$(S) W=$(W) T=$(T)
	@echo "Running the preprocessor is over."
	+$(MAKE) -C $(BIN) run-local-serial 

clean:
	rm -f $(PRE)/*.o
	rm -f $(PRE)/$(PROGPRE)
	rm -f $(BIN)/$(PROGPRE)
	rm -f $(POST)/*.o
	rm -f $(POST)/$(PROGPOST)
	rm -f $(BIN)/$(PROGPOST)

dataclean:
	rm -f $(BIN)/outfiles/*
	rm -f $(BIN)/*.lammps
	rm -f $(BIN)/*.log
	rm -f $(BIN)/*.pbs

.PHONY: all clean
