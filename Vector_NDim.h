/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   Vector_NDim.h
**********************************************************************************************/

#ifndef VECTOR_NDIM_H
#define VECTOR_NDIM_H

#include "MersenneTwister.h"

class Vector_NDim 
{
  private:
  public:
    typedef unsigned int  uint;
    
    uint    N_;  //dimensionality of the vector
    double* v_;  //components of the ND vector
  
    Vector_NDim(uint N, Vector_NDim* oldVec); //copy constructor
    Vector_NDim(uint N, double val);
    Vector_NDim(uint N, int val);
    Vector_NDim(uint N, MTRand &randomGen);
    Vector_NDim(uint N, MTRand &randomGen, uint start, uint end);
    virtual ~Vector_NDim();
  
    void         add(Vector_NDim* vec2);
    void         clear();
    double       dot(Vector_NDim* vec2);
    double       dotForRange(Vector_NDim* vec2, uint start, uint end);
    Vector_NDim* getAbsComponents();
    Vector_NDim* getMultiple(double c);
    Vector_NDim* getReflectionAndNormalize(Vector_NDim* r);
    double       getSquare();
    double       getSquareForRange(uint start, uint end);
    Vector_NDim* getSqComponents();
    void         multiply(double c);
    void         normalize();
    void         print();
    void         reflectOverUnitVecAndNormalize(Vector_NDim* r);
    void         subtract(Vector_NDim* vec2);
};

#endif  /* VECTOR_NDIM_H */

