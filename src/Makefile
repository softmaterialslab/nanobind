#This make file builds the sub folder make files
#example make call for bigred2
# make cluster-submit Q=1500 n=25 c=0.150 q=35 D=56 d=6.7 i=0 N=150

PROGPRE = preprocessor
PROGPOST = postprocessor
JOBSCR = macroassembly.pbs
TESTSCR = test.pbs
BIN = ../bin/
PRE = preprocessing
POST = postprocessing
SCRIPT = scripts
E=1 
C=9 
S=108 
w=100
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


install: create-dirs
	make CCF=nanoHUB all

create-dirs:
	@echo "Checking and creating needed sub-directories in the $(BIN) directory"
	if ! [ -d $(BIN) ]; then mkdir $(BIN); fi
	if ! [ -d $(BIN)/outfiles ]; then mkdir $(BIN)/outfiles; fi
	if ! [ -d $(BIN)/infiles ]; then mkdir $(BIN)/infiles; fi
	@echo "Directory creation is over."

clean:
	rm -f $(PRE)/*.o
	rm -f $(PRE)/$(PROGPRE)
	rm -f $(POST)/*.o
	rm -f $(POST)/$(PROGPOST)

dataclean:
	rm -f $(BIN)/outfiles/*
	rm -f $(BIN)/dumpfiles/*
	rm -f $(BIN)/*.lammps
	rm -f $(BIN)/*.log
	rm -f $(BIN)/*.pbs
	
distclean: clean
	rm -f $(BIN)/$(PROGPRE)
	rm -f $(BIN)/$(PROGPOST)

.PHONY: all clean distclean
