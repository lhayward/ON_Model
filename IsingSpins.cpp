/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   IsingSpins.cpp
*
* Ising spins will be stored as boolean variables (0 or 1) but returned as integers 
* (-1 or +1, respectively).
**********************************************************************************************/

#include <iostream>
#include "IsingSpins.h"

/****************************** IsingSpins(int N) (constructor) *******************************
* Input: N (number of spins)
* This constructor initializes the array of spin degrees of freedom 
**********************************************************************************************/
IsingSpins::IsingSpins(uint N)
{
  N_       = N;
  
  spins_ = new bool[N_];
  for( uint i=0; i<N_; i++ )
  { spins_[i] = 0; }
}

/********************************* ~IsingSpins() (destructor) ********************************/
IsingSpins::~IsingSpins()
{ 
  if( spins_ != NULL )
  { delete[] spins_; }
  spins_ = NULL;
}

/************************************** flipSpin(uint i) *************************************/
void IsingSpins::flipSpin(uint i)
{
  spins_[i] = !spins_[i];
}

/*************************************** getSpin(int i) **************************************/
int IsingSpins::getSpin(uint i)
{
  return (2*spins_[i] - 1);
}

/****************************************** print() ******************************************/
void IsingSpins::print()
{
  for( uint i=0; i<N_; i++ )
  { 
    //print an extra space if spin i is in the +1 state:
    if( spins_[i] )
    {  std::cout << " "; }
    
    std::cout << (2*spins_[i] - 1) << " "; 
  }
  std::cout << std::endl;
} //print method

/******************************** randomize(MTRand* randomGen) *******************************/
void IsingSpins::randomize(MTRand &randomGen)
{
  for( uint i=0; i<N_; i++ )
  { spins_[i] = randomGen.randInt(1); } //end of loop over i
} //randomize method
