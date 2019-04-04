# This is a makefile.

PROG = generate_lammps_script

OBJ = main.o

CC = g++ -O3 -g -Wall

LFLAG = -lgsl -lgslcblas

CFLAG = -c

OFLAG = -o

$(PROG) : $(OBJ)
	$(CC) $(OFLAG) $(PROG) $(OBJ) $(LIBS) $(LFLAG)

main.o: vector3d.h 


clean:
	rm -f *.o
