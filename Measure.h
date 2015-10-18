/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   Measure.h
**********************************************************************************************/

#ifndef MEASURE_H
#define MEASURE_H

#include <unordered_map>
#include <string>
#include <vector>

class Measure 
{ 
  public:
    typedef unsigned int  uint;
    
  private:
    std::unordered_map<std::string,double> measurements;
    std::vector       <std::string>        measStrings;
    
  public:
    Measure();
    virtual ~Measure();
    
    void accumulate    (std::string label, double newMeas);
    void insert        (std::string label);
    void print         ();
    void writeAverages (std::ofstream* fout, uint numMeas);
    void writeMeasNames(std::ofstream* fout);
    void zero          ();
};

#endif  // MEASURE_H
