/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   Hyperrectangle.h
* Description: Note that L_[0] == Lx = lattice length along x
*                        L_[1] == Ly = lattice length along y
*                          .
*                          .
*                          .
*
*              The vertices are labelled along the x-direction, followed by the 
*              y-direction, etc. 
*              So for example, the first Lx*Ly vertices (in the x-y plane in D>=2) are:
*                (Lx*Ly - Lx)    (Lx*Ly - Lx + 1)    . . .    (Lx*Ly - 1)
*                     .                 .                          .
*                     .                 .                          .
*                     .                 .                          .
*                     Lx               Lx+1          . . .      2Lx - 1
*                     0                 1            . . .       Lx-1
**********************************************************************************************/

#ifndef HYPERRECTANGLE_H 
#define HYPERRECTANGLE_H

#include <cmath>
#include <fstream>

class Hyperrectangle
{ 
  public:
    typedef unsigned int  uint;
    
  private:
    uint  N_;           //total number of lattice sites
    uint  z_;           //number of nearest neighbouring sites for each site
    uint  D_;           //dimension
    uint* L_;           //length in each dimension
    uint** neighbours_; //coordinates of each vertex's nearest neighbours
    
    void   initNeighbours();
    int    round(double num);
    //void   trimWhiteSpace(std::string* word);
    uint   nextInDir(uint dir); //used by initNeighbours method
    uint   uintPower(uint base, uint exp);
    
  public:
    Hyperrectangle(std::ifstream* fin, std::string fileName);
    virtual ~Hyperrectangle();
    
    uint getNeighbour(uint i, uint j);
    void printParams();
    void printNeighbours();
    
    //getter methods:
    uint  getN();
    uint  getZ();
    uint  getD();
    uint* getL();
};

#endif  // HYPERRECTANGLE_H
