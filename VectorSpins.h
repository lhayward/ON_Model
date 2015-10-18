/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   VectorSpins.h
**********************************************************************************************/

#ifndef VECTOR_SPINS_H
#define VECTOR_SPINS_H

#include "MersenneTwister.h"
#include "Vector_NDim.h"

class VectorSpins 
{
  public:
    typedef unsigned int uint;
  
  private:
    uint          N_;       //number of spins
    uint          spinDim_; //dimensionality of each vector
    Vector_NDim** spins_;   //array of the vector spin degrees of freedom
    
  public:
    VectorSpins(uint N, uint spinDim);
    virtual ~VectorSpins();
    
    Vector_NDim* getSpin(uint i);
    void         print();
    void         randomize(MTRand &randomGen);
    void         setSpin(uint i, Vector_NDim* newSpin);
};

#endif  // VECTOR_SPINS_H
