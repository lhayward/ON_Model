/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   Vector_NDim.cpp
**********************************************************************************************/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Vector_NDim.h"

/********************************* Vector_NDim (constructor) *********************************/
Vector_NDim::Vector_NDim(uint N, Vector_NDim* oldVec)
{
  N_ = N;
  v_ = new double[N];
  
  for( uint i=0; i<N_; i++)
  { v_[i] = oldVec->v_[i]; }
}

/********************************* Vector_NDim (constructor) *********************************/
Vector_NDim::Vector_NDim(uint N, double val) 
{
  N_ = N;
  v_ = new double[N];
  
  for( uint i=0; i<N_; i++ )
  { v_[i] = val; }
}

/********************************* Vector_NDim (constructor) *********************************/
Vector_NDim::Vector_NDim(uint N, int val) 
{
  N_ = N;
  v_ = new double[N];
  
  for( uint i=0; i<N_; i++ )
  { v_[i] = val; }
}

/********************************* Vector_NDim (constructor) **********************************
* This is a method for generating a random point on an N-dimensional unit hypersphere. For 
* efficiency reasons, different techniques are used for different values of N:
* 
* For N<=3:
* The method generates a random point insides an N-dimensional hypercube of volumes 2^N. If 
* this point is outside the inscribed unit hypersphere, then is it is rejected. Otherwise, the 
* point is accepted and it is projected onto the surface of the unit hypersphere.
*
* For N>=4:
* The method generates N random coordinates according to a normal distribution with mean 0 and 
* variance 1. Each coordinate is then divided by the norm of the resulting vector so that the 
* final point is on the surface of the N-dimensional unit hypersphere.
*
* Note: It is also possible to use the method proposed by Marsaglia in 1972 for the N=4 case.
**********************************************************************************************/
Vector_NDim::Vector_NDim(uint N, MTRand &randomGen)
{
  N_ = N;
  v_ = new double[N];
  double S=0;
  
  //hypercube method is more efficient for N_<=3
  if(N_<=3)
  {
    for( uint i=0; i<N_; i++ )
    {
      v_[i] = -1.0 + 2*(randomGen.randDblExc());
      S += pow(v_[i],2);
    } //for
  
    while( S >= 1 )
    {
      S=0;
      for( uint i=0; i<N_; i++ )
      {
        v_[i] = -1.0 + 2*(randomGen.randDblExc());
        S += pow(v_[i],2);
      } //for
    } //while
  } //if
  
  //normal distribution method is more efficient for N_>=4:
  else
  {
    for( uint i=0; i<N_; i++ )
    {
      v_[i] = randomGen.randNorm(0,1);
      S += pow(v_[i],2);
    }
  } //else
  
  //normalize:
  S = pow(S,0.5);
  
  for( uint i=0; i<N_; i++ )
  { v_[i] = v_[i]/S; }
}

/********************************* Vector_NDim (constructor) *********************************/
Vector_NDim::Vector_NDim(uint N, MTRand &randomGen, uint start, uint end)
{
  N_ = N;
  v_ = new double[N];
  uint n;
  
  for( uint i=0; i<N_; i++ )
  { v_[i] = 0; }
  
  n=1;
  if( end>start )
  { n = end-start+1; }
  
  Vector_NDim nonzeroParts(n, randomGen);
  
  for( uint i=0; i<n; i++ )
  { v_[i+start] = nonzeroParts.v_[i]; }
}

/********************************* ~Vector_NDim (destructor) *********************************/
Vector_NDim::~Vector_NDim()
{
  if( v_ != NULL )
  { delete[] v_; }
  v_ = NULL;
}

/******************************************** add ********************************************/
void Vector_NDim::add(Vector_NDim* vec2)
{
  for( uint i=0; i<N_; i++ )
  { v_[i] += vec2->v_[i]; }
}

/******************************************* clear *******************************************/
void Vector_NDim::clear()
{
  for( uint i=0; i<N_; i++ )
  { v_[i] = 0; }
}

/******************************************** dot ********************************************/
double Vector_NDim::dot(Vector_NDim* vec2)
{
  double result=0;
  
  for( uint i=0; i<N_; i++ )
  { result += v_[i]*vec2->v_[i]; }
  
  return result;
}

/**************************************** dotForRange ****************************************/
double Vector_NDim::dotForRange(Vector_NDim* vec2, uint start, uint end)
{
  double result=0;
  uint index1=start;
  uint index2=end;
  
  /*if(index1 >= N_)
  { index1 = (N_-1); }
  if(index2 >= N_)
  { index2 = (N_-1); }*/
  
  for( uint i=index1; i<=index2; i++ )
  { result += v_[i]*vec2->v_[i]; } 
  
  return result;
}

/************************************** getAbsComponents *************************************/
Vector_NDim* Vector_NDim::getAbsComponents()
{
  Vector_NDim* result = new Vector_NDim(N_,0);
  
  for( uint i=0; i<N_; i++ )
  { result->v_[i] = abs(v_[i]); }
  
  return result;
}

/**************************************** getMultiple ****************************************/
Vector_NDim* Vector_NDim::getMultiple(double c)
{
  Vector_NDim* result = new Vector_NDim(N_,0);
  
  for( uint i=0; i<N_; i++ )
  { result->v_[i] = c*v_[i]; }
  
  return result;
}

/********************************* getReflectionAndNormalize *********************************/
Vector_NDim* Vector_NDim::getReflectionAndNormalize(Vector_NDim* r)
{
  Vector_NDim* result = new Vector_NDim(N_, this);
  result->reflectOverUnitVecAndNormalize(r);
  return result;
}

/***************************************** getSquare *****************************************/
double Vector_NDim::getSquare()
{
  double result = 0;
  
  for( uint i=0; i<N_; i++ )
  { result += pow(v_[i],2); }
  
  return result;
}

/************************************* getSquareForRange *************************************/
double Vector_NDim::getSquareForRange(uint start, uint end)
{
  double result = 0;
  uint index1=start;
  uint index2=end;
  
  /*if(index1 >= N_)
  { index1 = (N_-1); }
  if(index2 >= N_)
  { index2 = (N_-1); }*/
  
  for( uint i=index1; i<=index2; i++ )
  { result += pow(v_[i],2); }
  
  return result;
}

/************************************** getSqComponents **************************************/
Vector_NDim* Vector_NDim::getSqComponents()
{
  Vector_NDim* result = new Vector_NDim(N_,0);
  
  for( uint i=0; i<N_; i++ )
  { result->v_[i] = pow(v_[i],2); }
  
  return result;
}

/****************************************** multiply *****************************************/
void Vector_NDim::multiply(double c)
{
  for( uint i=0; i<N_; i++ )
  { v_[i] = c*v_[i]; }
}

/***************************************** normalize *****************************************/
void Vector_NDim::normalize()
{
  double len = pow(getSquare(),0.5);
  
  for( uint i=0; i<N_; i++ )
  { v_[i] = v_[i]/len; }
}

/******************************************* print *******************************************/
void Vector_NDim::print()
{
  std::cout << "[ "; 
  for( uint i=0; i<(N_-1); i++ )
  { std::cout << v_[i] << " , "; }
  std::cout << v_[N_-1] << " ]" << std::endl;
}

/******************************* reflectOverUnitVecAndNormalize *******************************
* Note that the passed 'r' must be a unit vector (could write this method for general, non-unit
* 'r' (see commented out code with 'rSq'), but it is much slower. 
**********************************************************************************************/
void Vector_NDim::reflectOverUnitVecAndNormalize(Vector_NDim* r)
{
  double dotProd = dot(r); 
  //double rSq     = r->getSquare();
  
  for( uint i=0; i<N_; i++ )
  { 
    //v_[i] = v_[i] - (2.0*dotProd/rSq)*r->v_[i]; 
    v_[i] = v_[i] - (2.0*dotProd)*r->v_[i];
  }
  
  //ensure that the norm is still one (helps avoid rounding errors building up when the vector
  //is supposed to be a unit vector):
  normalize();
}

/****************************************** subtract *****************************************/
void Vector_NDim::subtract(Vector_NDim* vec2)
{
  for( uint i=0; i<N_; i++ )
  { v_[i] -= vec2->v_[i]; }
}
