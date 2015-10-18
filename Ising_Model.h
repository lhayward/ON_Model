/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   Ising_Model.h
**********************************************************************************************/

#ifndef ISING_MODEL
#define ISING_MODEL

#include <string>
#include "Hyperrectangle.h"
#include "IsingSpins.h"
#include "ON_Model.h"

class Ising_Model : public ON_Model
{ 
  public:
    typedef unsigned int  uint;
    
  private:                  
    IsingSpins*  spins_; //the degrees of freedom (d.o.f.) for the model
    
    void   flipCluster       (std::vector<uint> &cluster);
    double getEnergy         ();
    int    getSigmaTot       ();
    void   updateObservables ();
    void   wolffUpdate       (MTRand &randomGen, bool pr);
    
  public:
    Ising_Model(std::ifstream* fin, std::string outFileName, Hyperrectangle* lattice, 
                MTRand &randomGen);
    virtual ~Ising_Model();
    
    virtual void localUpdate        (MTRand &randomGen);
    virtual void makeMeasurement    ();
    virtual void printSpins         ();
    virtual void randomizeLattice   (MTRand &randomGen);
    virtual void sweep              (MTRand &randomGen, bool pr);
};  

#endif  //ISING_MODEL
