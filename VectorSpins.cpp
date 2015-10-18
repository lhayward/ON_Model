/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   VectorSpins.cpp
**********************************************************************************************/

#include <iostream>
#include "Vector_NDim.h"
#include "VectorSpins.h"

/****************************** VectorSpins(int N) (constructor) ******************************
* Input: N (number of spins)
* This constructor initializes the array of vector spin degrees of freedom 
**********************************************************************************************/
VectorSpins::VectorSpins(uint N, uint spinDim)
{
  N_       = N;
  spinDim_ = spinDim;
  
  spins_ = new Vector_NDim*[N];
  
  for( uint i=0; i<N_; i++ )
  { spins_[i] = NULL; }
}

/******************************** ~VectorSpins() (destructor) ********************************/
VectorSpins::~VectorSpins()
{ 
  if( spins_!=NULL)
  {
    for( uint i=0; i<N_; i++ )
    { delete spins_[i]; }
    delete[] spins_;
  }
  spins_=NULL;
}

/*************************************** getSpin(int i) **************************************/
Vector_NDim* VectorSpins::getSpin(uint i)
{
  /*Vector_NDim* result = NULL;
  
  if( (spins_ != NULL) && i<N_ )
  { result = spins_[i]; }
  else
  { 
    std::cout << "ERROR in VectorSpins::getSpin(int i): NULL spins_ array or index "
              << "out of bounds" << std::endl; 
  }
  
  return result;*/
  return spins_[i];
}

/****************************************** print() ******************************************/
void VectorSpins::print()
{
  for( uint i=0; i<N_; i++ )
  {
    std::cout << "Spin " << i << ": ";
    spins_[i]->print();
  }
  std::cout << std::endl;
} //print method

/******************************** randomize(MTRand* randomGen) *******************************/
void VectorSpins::randomize(MTRand &randomGen)
{
  for( uint i=0; i<N_; i++ )
  {
    //if there is already a Vector_NDim object in this location, then delete it first to
    //avoid memory leaks:
    if( spins_[i] != NULL )
    { delete spins_[i]; }
    
    spins_[i] = new Vector_NDim(spinDim_,randomGen); 
  } //end of loop over i
} //randomize method

/*************************** setSpin(uint i, Vector_NDim* newSpin) ***************************/
void VectorSpins::setSpin(uint i, Vector_NDim* newSpin)
{
  /*if( (newSpin != NULL) && i<N_ )
  { spins_[i] = newSpin; }
  else
  { 
    std::cout << "ERROR in VectorSpins::setSpin(uint i, Vector_NDim* newSpin): NULL"
              << " Vector_NDim object passed or index out of bounds" << std::endl; 
  }*/
  spins_[i] = newSpin;
}
