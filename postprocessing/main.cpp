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
   
   double vlpDiameter = 1; //reduced units
   double ligandDiameter = 0.1; //reduced units
   double wallDiameter = 0.1; //reduced units
   string fileName = "ovitomovie.melt" ; //name of ovito dump file from lammps
   
   cout << "How many vlp-ligand complexes?" << endl;
   int numberComplexes;
   cin >> numberComplexes;
   
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
   vector<VECTOR3D> ligandXYZ;
   vector<VECTOR3D> wallXYZ;
   int index;
   int type;                                         //particle type
   double x;
   double y;
   double z;
   double boxLength;
   VECTOR3D dist;
   int bondedWall;
   
   int i = 0;
   while (crds.get())
   {
      
      crds >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy ;   //reading the header of the timestep
      crds >> atomNumber;
      crds >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy ;
      crds >> dummy >> boxLength >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;
      
      if (i == 0) //for first timestep, push back ligands, vlp and wall mesh particles
      {
         for (int j = 0; j < atomNumber; j++)
         {
            crds >> index >> type >> x >> y >> z;
            if (type == 1 ) vlpXYZ.push_back(VECTOR3D(x,y,z));  //if it is a virus, push it to a vector
            if (type == 2 ) ligandXYZ.push_back(VECTOR3D(x,y,z)); //if it is a ligand
            if (type == 3 ) wallXYZ.push_back(VECTOR3D(x,y,z)); //if it is a wall mesh particle
         }
         
         if (vlpXYZ.size() != numberComplexes) cout << "ERROR! number of complexes entered does not match datafile!" << endl;
         
      } else {
         double vlpIndex = 0;
         double ligandIndex = 0;
         double wallIndex = 0;
         
         for (int j = 0; j < atomNumber; j++)
         {
            crds >> index >> type >> x >> y >> z;
            if (i >= equilibrium)
            {
               if (type == 1 ) 
               {
                  vlpXYZ[vlpIndex] = (VECTOR3D(x,y,z));  //if it is a virus
                  vlpIndex += 1;
               }
               if (type == 2 ) 
               {
                  ligandXYZ[ligandIndex] = (VECTOR3D(x,y,z)); //if it is a ligand
                  ligandIndex += 1;
               }
               if (type == 3 ) 
               {
                  wallXYZ[wallIndex] = (VECTOR3D(x,y,z)); //if it is a wall mesh particle
                  wallIndex += 1;
               }
            }
         } //end for
         
         if (i >= equilibrium)
         {
            bondedWall = 0;
            for (int j = 0; j < wallXYZ.size(); j++)
            {
               for (int k = 0; k < ligandXYZ.size(); k++)
               {
                  dist.x = wallXYZ[j].x - ligandXYZ[k].x;
                  dist.y = wallXYZ[j].y - ligandXYZ[k].y;
                  dist.z = wallXYZ[j].z - ligandXYZ[k].z;
                  if (dist.x>boxLength/2) dist.x -= boxLength; //account for periodic boundaries in x & y  
                  if (dist.x<-boxLength/2) dist.x += boxLength;
                  if (dist.y>boxLength/2) dist.y -= boxLength;
                  if (dist.y<-boxLength/2) dist.y += boxLength;
                  //       if (dist.z>boxLength/2) dist.z -= boxLength;  // z is not periodic
                  //       if (dist.z<-boxLength/2) dist.z += boxLength;
                  if (dist.GetMagnitude() < ligandDiameter*2.5)
                  {
                     bondedWall += 1;
                     break;
                  }
               }
            }
            processedData << i << "  " << bondedWall << "  " << wallXYZ.size() << endl;
         } //end if
      } // end else; after first timestep
      
      if (crds.eof())                      // check for EOF
      {
         cout << "End of file reached after " << i << " timesteps." << endl;
         crds.close();                        // close file
         break;
      } else i++ ;
      
   } //end timesteps
   
   
   cout << "Done!" << endl;
   
   
return 0 ;

}







