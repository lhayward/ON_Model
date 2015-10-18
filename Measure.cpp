/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   Measure.cpp (Abstract Class)
**********************************************************************************************/

#include <fstream>
#include <iostream>
#include "Measure.h"

/********************************** Measure() (constructor) **********************************/
Measure::Measure()
{ }

/********************************** ~Measure() (destructor) **********************************/
Measure::~Measure()
{ }

/*********************** accumulate(std::string label, double newMeas) ***********************/
void Measure::accumulate(std::string label, double newMeas)
{ measurements[label] += newMeas; }

/********************************* insert(std::string label) *********************************/
void Measure::insert(std::string label)
{ 
  measurements.insert( std::pair<std::string,double>(label, 0.0) );
  measStrings.push_back( label );
}

/****************************************** print() ******************************************/
void Measure::print()
{
  std::cout << "Measurements:" << std::endl;
  
  //Print the measurements in the order the measStrings were added:
  for( uint i=0; i<measStrings.size(); i++ )
  { std::cout << "  " << measStrings[i] << ": " << measurements[measStrings[i]] << '\n'; }

  std::cout << std::endl;
}

/********************** writeAverages(std::ofstream* fout, uint numMeas) *********************/
void Measure::writeAverages(std::ofstream* fout, uint numMeas)
{
  //Write the measurement averages in the order the measStrings were added:
  for( uint i=0; i<measStrings.size(); i++ )
  { (*fout) << '\t' << (measurements[measStrings[i]]/(1.0*numMeas)); }
}

/**************************** writeMeasNames(std::ofstream* fout) ****************************/
void Measure::writeMeasNames(std::ofstream* fout)
{
  //Write the names of each observable to the passed file stream (for clarity when processing
  //the bin file):
  for( uint i=0; i<measStrings.size(); i++ )
  { (*fout) << '\t' << measStrings[i]; } 
}

/******************************************* zero() ******************************************/
void Measure::zero()
{
  for( uint i=0; i<measStrings.size(); i++ )
  { measurements[measStrings[i]] = 0.0; }
}