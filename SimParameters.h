/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   SimParameters.h
**********************************************************************************************/

#ifndef SIM_PARAMETERS_H
#define SIM_PARAMETERS_H

#include <fstream>
#include <string>
#include <vector>
#include "MersenneTwister.h"

class SimParameters 
{ 
  public:
    typedef unsigned int  uint;
    typedef unsigned long ulong;
    
    std::vector<double>* TList_;  //list of temperatures
    ulong                seed_;
    uint                 numWarmUpSweeps_;
    uint                 sweepsPerMeas_;
    uint                 measPerBin_;
    uint                 numBins_;
    uint                 spinDim_;  //value of N for the O(N) model
    MTRand               randomGen_;  //random number generator
    
  //public:
    SimParameters(std::string fileName, std::string startStr);
    virtual ~SimParameters();
    
    void print();
    
    //getter methods:
    /*double      getTemperature    (uint i);
    MTRand*     getRandomGen      ();
    uint        getNumWarmUpSweeps();
    uint        getSweepsPerMeas  ();
    uint        getMeasPerBin     ();
    uint        getNumBins        ();
    std::string getLatticeType    ();
    std::string getModelName      ();*/
};

#endif  // SIM_PARAMETERS_H
