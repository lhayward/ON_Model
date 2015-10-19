/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   IsingSpins.h
**********************************************************************************************/

#ifndef ISING_SPINS_H
#define ISING_SPINS_H

#include <fstream>
#include <iostream>
#include "MersenneTwister.h"

class IsingSpins 
{
  public:
    typedef unsigned int uint;
  
  private:
    uint  N_;       //number of spins
    bool* spins_;   //array of the spin degrees of freedom
    
  public:
    IsingSpins(uint N);
    virtual ~IsingSpins();
    
    void flipSpin(uint i);
    int  getSpin(uint i);
    void print();
    void print(std::ofstream* fout);
    void randomize(MTRand &randomGen);
};

#endif  // ISING_SPINS_H
