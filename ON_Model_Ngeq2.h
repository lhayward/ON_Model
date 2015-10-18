/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   ON_Model_Ngeq2.h
**********************************************************************************************/

#ifndef ON_MODEL_NGEQ2
#define ON_MODEL_NGEQ2

#include <string>
#include "Hyperrectangle.h"
#include "ON_Model.h"
#include "Vector_NDim.h"
#include "VectorSpins.h"

class ON_Model_Ngeq2 : public ON_Model
{ 
  public:
    typedef unsigned int  uint;
    
  private:                  
    VectorSpins*    spins_; //the degrees of freedom (d.o.f.) for the model
    
    void   flipCluster           (std::vector<uint> &cluster, Vector_NDim* r);
    double getClusterOnSiteEnergy(std::vector<uint> &cluster);
    double getEnergy             ();
    double getHelicityModulus    (int dir);
    double getSigma1Tot();
    void   updateObservables     ();
    void   wolffUpdate           (MTRand &randomGen, uint start, uint end, bool pr);
    
  public:
    ON_Model_Ngeq2(uint spinDim, std::ifstream* fin, std::string outFileName, 
                   Hyperrectangle* lattice, MTRand &randomGen);
    virtual ~ON_Model_Ngeq2();
    
    virtual void localUpdate        (MTRand &randomGen);
    virtual void makeMeasurement    ();
    virtual void printSpins         ();
    virtual void randomizeLattice   (MTRand &randomGen);
    virtual void sweep              (MTRand &randomGen, bool pr);
};  

#endif  //ON_MODEL_NGEQ2
