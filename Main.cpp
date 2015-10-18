/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File: Main.cpp
* Note: MersenneTwister.h was taken from external sources and is used as the random number
*       generator
**********************************************************************************************/

//#include <cmath>
#include <ctime>
//#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include "FileReading.h"
#include "Hyperrectangle.h"
#include "Ising_Model.h"
#include "MersenneTwister.h"
#include "Hyperrectangle.h"
#include "ON_Model.h"
#include "ON_Model_Ngeq2.h"
#include "SimParameters.h"

typedef unsigned long ulong;
typedef unsigned int  uint;

std::string getFileSuffix(int argc, char** argv);
Hyperrectangle* readLattice(std::string fileName, std::string startStr);
ON_Model* readModel(uint spinDim, std::string inFileName, std::string startStr, 
                    std::string outFileName, Hyperrectangle* lattice, MTRand &randomGen);

/**********************************************************************************************
******************************************** main *********************************************
**********************************************************************************************/
int main(int argc, char** argv) 
{
  //MTRand            randomGen;
  SimParameters*    params;
  Hyperrectangle*   lattice;
  ON_Model*         model;
  uint              numT;  //number of temperatures
  double            T; //current temperature
  std::stringstream ss;
  time_t            sec1, sec2;  //for timing
  
  //variables related to input/output data from/to files:
  std::string fileSuffix        = getFileSuffix( argc, argv );
  std::string paramFileName     = "params" + fileSuffix + ".txt";
  std::string simParamStr       = "SIMULATION PARAMETERS";
  std::string latticeParamStr   = "LATTICE PARAMETERS";
  std::string modelParamStr     = "MODEL PARAMETERS";
  std::string outFileName       = "bins" + fileSuffix + ".txt";
  std::string outFileName_clust;
  
  std::cout.precision(8);
  std::cout << "\nParameter File: " << paramFileName << "\n" << std::endl;
  
  params = new SimParameters( paramFileName, simParamStr );
  params->print();
  
  lattice = readLattice( paramFileName, latticeParamStr );
  lattice->printParams();

  model = readModel( params->spinDim_, paramFileName, modelParamStr, outFileName, lattice,
                     params->randomGen_ );
  model->printParams();
  
  std::cout << "\n*** STARTING SIMULATION ***\n" << std::endl;
  sec1 = time (NULL);
  //loop over the different temperatures:
  numT = params->TList_->size();
  for( uint TIndex=0; TIndex<numT; TIndex++)
  {
    T = (*(params->TList_))[TIndex];
    std::cout << "******** T = " << T << " (Temperature #" << (TIndex+1) << ") ********"
              << std::endl;
    model->changeT(T);
    model->randomizeLattice( params->randomGen_ );
    
    //equilibrate:
    for( uint i=0; i<params->numWarmUpSweeps_; i++ )
    { model->sweep( params->randomGen_, false ); }
    model->markWarmupDone();
    
    //loop over Monte Carlo bins:
    for( uint i=0; i<params->numBins_; i++ )
    {
      model->zeroMeasurements();
      //perform the measurements for one bin:
      for( uint j=0; j<params->measPerBin_; j++ )
      {
        //perform the sweeps for one measurement:
        for( uint k=0; k<params->sweepsPerMeas_; k++ )
        { model->sweep( params->randomGen_, false ); }
        model->makeMeasurement();
      } //loop over measurements
      model->writeBin((i+1), params->measPerBin_, params->sweepsPerMeas_);
      
      if( (i+1)%100==0 )
      { std::cout << (i+1) << " Bins Complete" << std::endl; }
    } //loop over bins
    
    //possibly write the cluster sizes to file (if it is turned on):
    outFileName_clust = "clustSizes" + fileSuffix + ".txt";
    if( numT > 1 )
    { 
      ss.str("");
      ss << "clustSizes" << fileSuffix << "_" << TIndex << ".txt"; 
      outFileName_clust = ss.str(); 
    }
    model->writeClustHistoData(outFileName_clust);
    
    std::cout << std::endl;
  } //temperature loop 
  
  sec2 = time(NULL);
  std::cout << "Time: " << (sec2 - sec1) << " seconds" << std::endl;
  std::cout << "\n*** END OF SIMULATION ***\n" << std::endl;
  return 0;
} //closes main

/**************************** getFileSuffix(int argc, char** argv) ***************************/
std::string getFileSuffix(int argc, char** argv)
{
  std::string result = "";
  
  if( argc > 1 )
  { result = "_" + std::string(argv[1]); }
  
  return result;
}

/****************** readLattice(std::string fileName, std::string startStr) ******************/
Hyperrectangle* readLattice(std::string fileName, std::string startStr)
{
  std::ifstream fin;
  fin.open(fileName.c_str());
  
  if( fin.is_open() )
  { FileReading::readUntilFound(&fin, startStr); }
  
  return new Hyperrectangle(&fin, fileName);
}

/******* readModel(uint spinDim, std::string inFileName, std::string startStr, ...      *******
********           std::string outFileName, Hyperrectangle* lattice, MTRand &randomGen) ******/
ON_Model* readModel(uint spinDim, std::string inFileName, std::string startStr, 
                    std::string outFileName, Hyperrectangle* lattice, MTRand &randomGen )
{
  ON_Model* result = NULL;
  std::ifstream fin;
  fin.open(inFileName.c_str());
  
  if( fin.is_open() )
  { FileReading::readUntilFound(&fin, startStr); }
  
  if( spinDim == 1 )
  { 
    result = new Ising_Model(&fin, outFileName, lattice, randomGen);
  }
  else
  {
    result = new ON_Model_Ngeq2(spinDim, &fin, outFileName, lattice, randomGen);
  }
  return result;
}