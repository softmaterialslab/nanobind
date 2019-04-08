#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include "vector3d.h"
#include <math.h>
#include <vector>




using namespace std;




int main() {
   
  
   cout << "Please enter the file name of the ovito dump file." << endl;
   string fileName;
   getline (cin, fileName);
   cout << "How many vlp-ligand complexes?" << endl;
   int numberComplexes;
   cin >> numberComplexes;
   cout << "How many timesteps in the ovito dump file?" << endl;
   int timesteps;
   cin >> timesteps;
   cout << "Which timestep marks equilibrium?" << endl;
   int equilibrium;
   cin >> equilibrium;
   
   ifstream crds;                                      //open coordinates file
   crds.open(fileName.c_str());
   if (!crds) 
   {                                        //check to make sure file is there
      cerr << "ERR: FILE " << fileName << " NOT OPENED. Check directory and/or filename.";
      exit(1);
   }
   
   cout << "Analysis file will be saved to " << fileName << ".processed" << endl;
   ofstream processedData( (fileName+".processed").c_str() );
 
   string dummy;                                     //dummy string for text in file
   double atomNumber;                                //total number of atoms, counting wall particles
   vector<VECTOR3D> vlpXYZ;                          //virus particles coordinates
   //vlpXYZ.resize(numberComplexes);
   int index;
   int type;                                         //particle type
   double x;
   double y;
   double z;
   double boxLength;
   
   
   for (int i = 0; i < timesteps; i++)
   {
      
      crds >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy ;   //reading the header of the timestep
      crds >> atomNumber;
      crds >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy ;
      crds >> dummy >> boxLength >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;
      
      for (int j = 0; j < atomNumber; j++)
      {
         crds >> index >> type >> x >> y >> z;
         if (type == 1 && i >= equilibrium) vlpXYZ.push_back(VECTOR3D(x,y,z));  //if it is a virus, push it to a vector. wait for equilibrium
      }
      
      if (i == equilibrium && vlpXYZ.size() != numberComplexes) cout << "ERROR! number of complexes entered does not match datafile!" << endl;
      
   }
   
   // Create density profile out of z coordinates
   
   vector<int> histogram(floor(boxLength),0);
  // double binSize = 1; 
  
   
   for (int i = 0; i < vlpXYZ.size(); i++)
   {
      histogram[floor(vlpXYZ[i].z)] += 1 ;
   }
   
   
   for (int i = 0; i < histogram.size(); i++)
   {
      processedData << i << " - " << i+1 << "   " << double(histogram[i])/double((timesteps-equilibrium)) << endl;
   }
   
   cout << "Done!" << endl;
   
   
return 0 ;

}







