#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include "vector3d.h"
#include <math.h>
#include <vector>
#include <gsl/gsl_rng.h>
#include <time.h>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

int main(int argc, char **argv) {

    /*Replacable variables*/
    string permitivityText = "USERINPUT_E";
    string totalTimeText = "USERINPUT_TIME";
    string initFileNameText = "USERINPUT_INIT_FILENAME";
    string mpiText = "NODESIZE";

    string initFile = "P22CD40L.in";

    // Set up the ligand xyz coordinates in a vector array

    vector<VECTOR3D> ligandCoordinates;
    double x;
    double y;
    double z;

    ifstream crds;                                      //open coordinates file
    crds.open("infiles/ligandCoordinates.coords");
    if (!crds) {                                        //check to make sure file is there
        cerr << "ERR: FILE ligandCoordinates.coords NOT OPENED. Check directory and/or filename.";
        exit(1);
    }
    for (int i = 0; i < 60; i++) {
        crds >> x >> y >> z;                // Add coordinates the the vector array
        ligandCoordinates.push_back(VECTOR3D(x, y, z));
    }


    // Get important parameters from the user

    double epsilon;
    double wallSpacing;
    double concentration;
    int numberComplexes;
    double timesteps;
    bool verbose;

    options_description desc("Usage:\nrandom_mesh <options>");
    desc.add_options()
            ("help,h", "print usage message")
            ("lennard jones well depth,E", value<double>(&epsilon)->default_value(1),
             "Binding strength of the ligand-recptor interaction (KbT)")
            ("[ligand - Virus] complex concentration,C", value<double>(&concentration)->default_value(9),
             "[ligand - Virus] complex concentration (nM)") // box size adjusteded for nanomolar conc.
            ("receptor spacing,w", value<double>(&wallSpacing)->default_value(100), "receptor spacing (nm)")
            ("number of vlp-ligand complexes,S", value<int>(&numberComplexes)->default_value(108),
             "number of vlp-ligand complexes")
            ("simulation time,T", value<double>(&timesteps)->default_value(275), "simulation time (milliseconds)")
            ("verbose,V", value<bool>(&verbose)->default_value(true), "verbose true: provides detailed output");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    //add options for ligand - virus ratio, properties of ligand / vlp, free ligand in solution

    // GENERATE P22CD40L.in FILE!!!

    // generate atoms

    ofstream datafile(initFile);

    datafile << "#Script for P22-CD40L complex. To be read into LAMMPS template. " << endl;

    datafile << "#LAMMPS  data    file " << endl;

    double vlpDiameter = 1; //reduced units
    double ligandDiameter = 0.1; //reduced units
    double wallDiameter = 0.1; //reduced units

    double boxLength;
    double wallNumberX; //number of wall particles in x direction
    double atomNumber;
    double bondNumber;
    boxLength = pow((numberComplexes * 0.001 / (6.022E23)) / (concentration * 1E-9), (1.0 / 3.0)); //in meters

    wallSpacing = wallSpacing * 1E-9; // in meters
    wallNumberX = boxLength / wallSpacing; //calculate number of mesh points
    wallNumberX = trunc(wallNumberX); // truncate this value
    cout << "adjusted spacing is " << (boxLength / wallNumberX) * 1E9 << " nm" << endl; //this is the adjusted spacing
    atomNumber = (numberComplexes * 61) + (wallNumberX * wallNumberX);
    bondNumber = numberComplexes * 180;

    double sigma = 60e-9; //sigma value currently used, diameter of P22
    double sigmaHC = 0.12; // sigma hc in reduced units
    double massSI = 3.819E-20; // mass of P22 in kg
    double epsilonSI = (1.38E-23) * (298.15); // epsilon in J
    boxLength = boxLength / sigma; // now in reduced units
    wallSpacing = boxLength / wallNumberX; // now in reduced units

    //convert simulation time to reduced units
    double tau = sigma * sqrt(massSI / epsilonSI); //1 MD timestep in seconds
    timesteps = ceil(timesteps * 1e-3 / tau);
    cout << "lammps will run for " << timesteps << " MD timesteps" << endl;

    if (boxLength < (10 * vlpDiameter)) {
        cout << "Uh oh! The z-direction is too small!" << endl;
    }


    datafile << atomNumber << " atoms" << endl;

    datafile << bondNumber << " bonds" << endl;

    datafile << "3 atom types" << endl;

    datafile << "3 bond types" << endl;

    datafile << "0" << " " << boxLength << " xlo xhi" << endl; // in reduced units...

    datafile << "0" << " " << boxLength << " ylo yhi" << endl;

    datafile << "0" << " " << boxLength << " zlo zhi" << endl;

    datafile << endl;

    //atoms section

    datafile << "Atoms" << endl << endl;

    double atomType;
    double vlpX;
    double vlpY;
    double vlpZ;
    double index = 0;
    vector<VECTOR3D> vlpXYZ;
    gsl_rng *r = gsl_rng_alloc(gsl_rng_mt19937);                    //setting up random seed
    //unsigned long int Seed = 23410982;
    gsl_rng_set(r, time(NULL));        //seed with time
    VECTOR3D dist;

    ///////////////////////////////...To give vlp random positions use these...////////////////////////////////
    bool add;

    while (vlpXYZ.size() < numberComplexes) {
        add = true;
        vlpX = (gsl_rng_uniform(r)) * boxLength; // guess some coordinates
        vlpY = (gsl_rng_uniform(r)) * boxLength;
        vlpZ = (gsl_rng_uniform(r)) * boxLength;

        if (vlpZ < 2.5) add = false; // if too close to the cell wall, reject the point
        if (vlpZ > (boxLength - (vlpDiameter + ligandDiameter)))
            add = false; // if overlaps with the upper z boundary reject


        if (vlpXYZ.size() == 0) {        //if no vlps have been added yet, add it
            vlpXYZ.push_back(VECTOR3D(vlpX, vlpY, vlpX));
            index += 1;
            datafile << index << " 1 " << "1" << " " << vlpX << " " << vlpY << " " << vlpZ << " " << endl;
            for (unsigned int m = 0; m < ligandCoordinates.size(); m++) {
                index += 1;
                x = vlpX + ligandCoordinates[m].x;
                y = vlpY + ligandCoordinates[m].y;
                z = vlpZ + ligandCoordinates[m].z;
                datafile << index << " 1 " << "2" << " " << x << " " << y << " " << z << " " << endl;
            }
            add = false;
        }

        for (int j = 0; j < vlpXYZ.size(); j++) { //make sure they don't overlap with other particles
            dist.x = vlpXYZ[j].x - vlpX;
            dist.y = vlpXYZ[j].y - vlpY;
            dist.z = vlpXYZ[j].z - vlpZ;
            if (dist.x > boxLength / 2) dist.x -= boxLength; //account for periodic boundaries in x & y
            if (dist.x < -boxLength / 2) dist.x += boxLength;
            if (dist.y > boxLength / 2) dist.y -= boxLength;
            if (dist.y < -boxLength / 2) dist.y += boxLength;
            //       if (dist.z>boxLength/2) dist.z -= boxLength; 	// z is not periodic
            //       if (dist.z<-boxLength/2) dist.z += boxLength;
            //   cout << "distance is " << dist.GetMagnitude() << " btw " << j << " ( " << vlpXYZ[j].x << " , " << vlpXYZ[j].y << " , " << vlpXYZ[j].z << " ) " << " and " << index/61 << " ( " << vlpX << " , " << vlpY << " , " << vlpZ << " ) " << endl;

            if (dist.GetMagnitude() < 2.5) {  //flag it if it intersects with anything
                //cout << "too close!" << endl;
                add = false;
                break;
            }
        }


        if (add == true) {        //if it doesn't intersect, add it and its ligands to the list
            vlpXYZ.push_back(VECTOR3D(vlpX, vlpY, vlpZ));
            index += 1;
            datafile << index << "  1 " << "1" << "  " << vlpX << "  " << vlpY << "  " << vlpZ << endl;
            for (unsigned int m = 0; m < ligandCoordinates.size(); m++) {
                index += 1;
                x = vlpX + ligandCoordinates[m].x;
                y = vlpY + ligandCoordinates[m].y;
                z = vlpZ + ligandCoordinates[m].z;
                datafile << index << "  1 " << "2" << "  " << x << "  " << y << "  " << z << endl;
            }
        } //else cout << "rejected!" << endl;

    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////   

//Making wall particles
    for (unsigned int i = 0; i < wallNumberX; i++) {
        for (unsigned int j = 0; j < wallNumberX; j++) {
            index += 1;
            datafile << index << " 1 " << "3" << "  " << i * wallSpacing << "  " << j * wallSpacing << "  " << "0"
                     << endl;
        }
    }

    cout << "Done making atoms!" << endl;

// Generate bonds

    vector<VECTOR3D> ligandBonds;
    int type;
    int b1;
    int b2;

    ifstream bonds;                                      //open coordinates file
    bonds.open("infiles/ligandBonds.coords");
    if (!bonds) {                                        //check to make sure file is there
        cerr << "ERR: FILE ligandBonds.coords NOT OPENED. Check directory and/or filename.";
        exit(1);
    }
    for (int i = 0; i < 180; i++) {
        bonds >> index >> type >> b1 >> b2;                // Add coordinates the the vector array
        ligandBonds.push_back(VECTOR3D(type, b1, b2));
    }

    datafile << endl << "Bonds" << endl << endl;

    index = 0;

    for (int i = 0; i < numberComplexes; i++) {
        for (int j = 0; j < ligandBonds.size(); j++) {
            index += 1;
            datafile << index << "     " << ligandBonds[j].x << "     " << ligandBonds[j].y + (i * 61) << "     "
                     << ligandBonds[j].z + (i * 61) << endl;
        }

    }

//bond type 1 length
    dist = VECTOR3D(0, 0, 0) - ligandCoordinates[0];
    double lengthType1 = dist.GetMagnitude();
//bond type 2 length
    dist = ligandCoordinates[0] - ligandCoordinates[1];
    double lengthType2 = dist.GetMagnitude();
//bond type 3 length
    dist = ligandCoordinates[0] - ligandCoordinates[20];
    double lengthType3 = dist.GetMagnitude();


    cout << "Done making bonds!" << endl;

/*************************Generating the LAMMPS file*************************/

    ofstream inputScript("in.lammps", ios::trunc);
    if (inputScript.is_open()) {

        /*Open the template file*/
        string line;
        ifstream inputTemplate("infiles/in.lammps.template", ios::in);
        if (inputTemplate.is_open()) {
            while (getline(inputTemplate, line)) {

                std::size_t found = line.find(permitivityText);
                if (found != std::string::npos)
                    line.replace(found, permitivityText.length(),  std::to_string(epsilon));

                found = line.find(totalTimeText);
                if (found != std::string::npos)
                    line.replace(found, totalTimeText.length(), std::to_string(int(timesteps)));

                found = line.find(initFileNameText);
                if (found != std::string::npos)
                    line.replace(found, initFileNameText.length(), initFile);

                inputScript << line << endl;
            }
            inputTemplate.close();
        } else cout << "Unable to open the template input script" << endl;
        inputScript.close();
    } else cout << "Unable create a input Script" << endl;


/*
//GENERATE LAMMPS INPUT SCRIPT

    ofstream outfile("lammps_script.in");

    outfile << "#Lammps input script for nanoparticle binding with multivalency #" << endl << endl;

    outfile << "units lj" << endl << "boundary p p f" << endl << "atom_style bond " << endl << "neighbor 0.3 bin"
            << endl << "neigh_modify every 1 delay 0 check yes" << endl << endl;

//Creating simulation box

    outfile << "## Create Simulation Box, Atoms ##" << endl;

    outfile << "read_data	   P22CD40L.in" << endl << endl;

    outfile << "group vlp type 1" << endl << "group ligand type 2" << endl << "group wall type 3" << endl << endl;

//Defining particle properties

    outfile << "## Defining Particle/Medium Properties ##" << endl;

    outfile << "mass	1	1	# reduced mass of vlp P22" << endl;

    outfile << "mass	2	0.002	# reduced mass of ligand " << endl;

    outfile << "mass	3	1	# mass of wall, irrelevant " << endl << endl;

    outfile << "## Ascribing Initial Velocities ##" << endl;

    outfile << "velocity	all	create	1.0	4928459	rot	yes	dist	gaussian	units	box" << endl
            << endl;

    outfile << "## Fixing Wall particles ##" << endl;

    outfile << "fix zwalls all wall/reflect zhi EDGE zlo EDGE" << endl << endl;

//Interparticle potentials

    outfile << "## Ascribing interparticle potentials: ##" << endl << endl;

    outfile << "pair_style      hybrid lj/expand        2.5     lj/cut 2.5" << endl;

    outfile << "pair_coeff      1       1       lj/cut           1      " << vlpDiameter << "	"
            << vlpDiameter * 1.12246 << "       # epsilon, sigma, cut-off  V-V" << endl;

    outfile << "pair_coeff      1       2       lj/expand        1      " << sigmaHC << "     "
            << (vlpDiameter + ligandDiameter) / 2 - sigmaHC << "	" << (vlpDiameter + ligandDiameter) / 2 * 1.12246
            << "  # epsilon, sigmaHC, delta_V-L, cut-off" << endl;

    outfile << "pair_coeff      2       2       lj/cut           1      " << ligandDiameter << "     "
            << ligandDiameter * 1.12246 << "       # L-L" << endl;

    outfile << "pair_coeff      1       3       lj/expand        1      " << sigmaHC << "     "
            << (vlpDiameter + wallDiameter) / 2 - sigmaHC << "	" << (vlpDiameter + wallDiameter) / 2 * 1.12246
            << "  # V-W" << endl;

    outfile << "pair_coeff      2       3       lj/cut       " << epsilon << "	   " << ligandDiameter << "	"
            << "2.5" << "  # L-W" << endl;

    outfile << "pair_coeff      3       3       lj/cut           0      0               # W-W" << endl << endl;

    outfile << "#pair_modify  shift   yes     # the additive e_LJ for repulsion-only" << endl << endl;

//Defining bond properties

    outfile << "## Making bonds ##" << endl;

    outfile << "bond_style harmonic" << endl;

    outfile << "bond_coeff 1    1000.0  " << lengthType1 << "  # type, K, distance" << endl;

    outfile << "bond_coeff 2    1000.0  " << lengthType2 << endl;

    outfile << "bond_coeff 3    1000.0  " << lengthType3 << endl << endl;

//Ensemble fixes

    outfile << "## Ensemble Fixes (+ for output) ##" << endl;

    outfile << "variable        myTStep equal   0.00005 # timestep where 1 MD step is "
            << sigma * sqrt((massSI) / (epsilonSI)) << " seconds" << endl;

    outfile << "timestep        ${myTStep}" << endl;

    outfile << "variable        myDStep equal   1000" << endl << endl;

    outfile << "fix     ens     vlp     nvt     temp    1.      1.      0.005" << endl;

    outfile << "fix     ens2    ligand  nvt     temp    1.      1.      0.005  # T_start, T_stop, T_damp=100*timestep"
            << endl;

    outfile << "fix_modify ens energy yes  # adds thermostat energy to potential" << endl;

    outfile << "fix_modify ens2 energy yes" << endl << endl;

//Output dump

    outfile << "## Initial Image Dump ##" << endl;

    outfile << "dump mymovie all custom 100 ovitomovie.melt id type x y z" << endl << endl;

    outfile << "print \"OUTPUT COLUMNS: SIMULATION STEP NUMBER | TEMPERATURE | NP POTENTIAL ENERGY | SYSTEM VOLUME\" "
            << endl << endl;

    outfile << "thermo_style   custom  step    temp   etotal  ke      pe     #print info to file" << endl;

    outfile << "thermo  50000" << endl << endl;

    outfile << fixed;

    outfile << "run     " << int(timesteps) << endl << endl;

    outfile << "## Defining Output Information ##" << endl;

    outfile
            << "dump    posD    all     custom  ${myDStep}      outfiles/dump.melt      id      type    x       y       z       # c_atomPot     c_atomKin"
            << endl << endl;

    outfile << " unfix   ens" << endl;

    outfile << "unfix   ens2" << endl;

    outfile << "undump  posD" << endl;

    outfile << "undump mymovie" << endl;

    outfile << "#shell          echo \"Lammps Simulation Ended\" " << endl;

*/

    gsl_rng_free(r);

    return 0;

}







