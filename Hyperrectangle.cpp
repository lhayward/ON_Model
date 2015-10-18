/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   Hyperrectangle.cpp
**********************************************************************************************/

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "FileReading.h"
#include "Hyperrectangle.h"

//typdef needed because uint is a return types:
typedef Hyperrectangle::uint uint;

/*********** Hyperrectangle(std::ifstream* fin, std::string fileName) (constructor) **********/
Hyperrectangle::Hyperrectangle(std::ifstream* fin, std::string fileName)
{
  const char EQUALS_CHAR     = '=';
  const char LIST_START_CHAR = '[';
  const char LIST_END_CHAR   = ']';
  
  N_          = 0;
  neighbours_ = NULL;
  
  //read in D_ from the file:
  if( fin!=NULL && fin->is_open() )
  { 
    D_ = FileReading::readUint     (fin, EQUALS_CHAR); 
    L_ = FileReading::readUintArray(fin, D_, 1, EQUALS_CHAR, LIST_START_CHAR, LIST_END_CHAR);
    
    N_ = 1;
    for( uint i=0; i<D_; i++ )
    { N_ *= L_[i]; }
    
    z_ = 2*D_;
    initNeighbours();
  }
  else
  {
    std::cout << "ERROR in Hyperrectangle constructor: could not read from file \"" << fileName
              << "\"\n" << std::endl;
    D_=1;
  }
}

/******************************* ~Hyperrectangle() (destructor) ******************************/
Hyperrectangle::~Hyperrectangle()
{
  //delete the neighbours_ array:
  for(uint i=0; i<N_; i++)
  { 
    if( neighbours_[i] != NULL )
    { delete[] neighbours_[i]; }
    neighbours_[i] = NULL; 
  }
  if( neighbours_ != NULL )
  { delete[] neighbours_; }
  neighbours_ = NULL;
} // ~Hyperrectangle

/******************************** getNeighbour(uint i, uint j) *******************************/
uint Hyperrectangle::getNeighbour(uint i, uint j)
{
  /*uint result = 0;
  
  if( (neighbours_ != NULL) && i<N_ && j<(2*D_) )
  { result = neighbours_[i][j]; }
  else
  { 
    std::cout << "ERROR in Hyperrectangle::getNeighbour(uint i, uint j): NULL neighbours_ "
              << "array or index out of bounds" << std::endl; 
  }
  
  return result;*/
  return neighbours_[i][j];
}

/************************************** initNeighbours() *************************************/
void Hyperrectangle::initNeighbours()
{
  uint next;     //used to naively estimate neighbour (before boundary correction) 
  uint next_mod; //used to determine if we are on boundary

  //initialize the neighbours_ array (note periodic boundary conditions):
  neighbours_ = new uint*[N_];
  for( uint i=0; i<N_; i++ )
  { neighbours_[i] = new uint[2*D_]; }
  for( uint i=0; i<N_; i++ )
  { 
    for( uint j=0; j<D_; j++ ) 
    {
      next     = nextInDir(j);
      next_mod = nextInDir(j+1);
      
      neighbours_[i][j] = i + next;
      //fix at the boundaries:
      if( neighbours_[i][j]%next_mod < next )
      { neighbours_[i][j] -= next_mod; }
      
      //initialize the corresponding neighbour (note that this information is redundant for a
      //hyperrectangle, but we include it since some Model classes won't assume a 
      //hyperrectangular lattice)
      neighbours_[ neighbours_[i][j] ] [j + D_] = i;
    } //j
  } //i
}

/************************************ nextInDir(uint dir) ************************************/
uint Hyperrectangle::nextInDir(uint dir)
{
  uint result = 1;
  for(uint i=0; i<dir; i++)
  { result *= L_[i]; } 
  
  return result;
} //nextInDir method

/*************************************** printParams() ***************************************/
void Hyperrectangle::printParams()
{
  std::cout << "Hyperrectangle Parameters:\n"
            << "-------------------------\n"
            << "                Dimension D = " << D_    << "\n"
            << "        Lattice Length L[0] = " << L_[0] << "\n";
  for( uint i=1; i<D_; i++ )
  { std::cout << "                       L[" << i << "] = " << L_[i] << "\n"; }
  
  std::cout << "  Number of Lattice Sites N = " << N_ << "\n"
            << std::endl;
  
} //printParams method

/****************************************** printNeighbours() ******************************************/
void Hyperrectangle::printNeighbours()
{
  std::cout << "Hyperrectangle Neighbours list:" << std::endl;

  //print the neighbours_ array:
  for( uint i=0; i<N_; i++ )
  {
    std::cout.width(4);
    std::cout << "    " << i << ": ";
    for( uint j=0; j<(2*D_); j++ )
    {
      std::cout.width(4);
      std::cout << neighbours_[i][j] << " ";
    } //j
    std::cout << std::endl;
  } //i 
  std::cout << std::endl;
} //printNeighbours method

/******************************************* round ******************************************/
int Hyperrectangle::round(double num)
{
    int result;
    
    if( num < 0.0 )
    { result = (int)ceil(num - 0.5); }
    else
    { result = (int)floor(num + 0.5); }
    
    return result;
}

/***************************** trimWhiteSpace(std::string* word) *****************************/
/*void Hyperrectangle::trimWhiteSpace(std::string* word)
{
  std::size_t index;
  
  //trim the leading whitespace:
  index = word->find_first_not_of(" \t\n");
  *word = word->substr(index);

  //trim the trailing whitespace:
  index = word->find_last_not_of(" \t\n");
  *word = word->substr(0,index+1);
}*/

/******************************** uintPower(int base, int exp) *******************************/
uint Hyperrectangle::uintPower(uint base, uint exp)
{
  uint result = 1;
  for(uint i=1; i<=exp; i++)
  { result *= base; } 
  
  return result;
} //uintPower method

/*********************************** Public Getter Methods: **********************************/
uint  Hyperrectangle::getN(){ return N_; }
uint  Hyperrectangle::getZ(){ return z_; }
uint  Hyperrectangle::getD(){ return D_; }
uint* Hyperrectangle::getL(){ return L_; }
