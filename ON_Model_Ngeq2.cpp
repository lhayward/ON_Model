/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   ON_Model_Ngeq2.cpp
**********************************************************************************************/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
#include "FileReading.h"
#include "ON_Model_Ngeq2.h"

//typdef needed because uint is a return types:
typedef ON_Model_Ngeq2::uint uint;

/******* ON_Model_Ngeq2(uint spinDim, std::ifstream* fin, std::string outFileName, ... ********
********                Hyperrectangle* lattice, MTRand* randomGen)                    ********
********                                 (constructor)                                 *******/
ON_Model_Ngeq2::ON_Model_Ngeq2(uint spinDim, std::ifstream* fin, std::string outFileName, 
                               Hyperrectangle* lattice, MTRand &randomGen)
  : ON_Model(fin, outFileName, lattice)
{
  std::cout.precision(15);
  
  spinDim_ = spinDim;
  spins_ = new VectorSpins(N_, spinDim_);
  randomizeLattice(randomGen);
  
  measures.insert("sigma1");
  measures.insert("helicityMod_x");
  measures.insert("helicityMod_y");
}

/******************************* ~ON_Model_Ngeq2() (destructor) ******************************/
ON_Model_Ngeq2::~ON_Model_Ngeq2()
{
  //delete the VectorSpins object:
  if( spins_ != NULL )
  { delete spins_; }
  spins_ = NULL;
} //destructor

/**************************************** flipCluster *****************************************
* This function flips all spins in the passed cluster by using the passed vector r.
**********************************************************************************************/
void ON_Model_Ngeq2::flipCluster(std::vector<uint> &cluster, Vector_NDim* r)
{
  uint clustSize = (uint)cluster.size();
  
  for( uint i=0; i<clustSize; i++ )
  { spins_->getSpin(cluster[i])->reflectOverUnitVecAndNormalize(r); }
} //flipCluster

/*********************************** getClusterOnSiteEnergy ***********************************
* This function calculates the onsite (non-interacting) part of the energy corresponding to 
* the spins in the passed cluster.
**********************************************************************************************/
double ON_Model_Ngeq2::getClusterOnSiteEnergy(std::vector<uint> &cluster)
{
  uint         clustSize = (uint)cluster.size();
  uint         latticeSite;
  double       energyh = 0;
  Vector_NDim* currSpin;
  
  for( uint i=0; i<clustSize; i++ )
  {
    latticeSite =  cluster[i];
    currSpin    =  spins_->getSpin(latticeSite);
    energyh     += currSpin->v_[0];    
  } //for loop
  
  return (-1.0)*h_*energyh;
} //getClusterOnSiteEnergy

/**************************************** getEnergy() ****************************************/
double ON_Model_Ngeq2::getEnergy()
{
  double       energyJ       = 0;
  double       energyh       = 0;
  Vector_NDim* currSpin;
  Vector_NDim* neighbour;
  
  //nearest neighbour term:
  for( uint i=0; i<N_; i++ )
  { 
    currSpin    = spins_->getSpin(i);
    for( uint j=0; j<D_; j++ )
    {
      neighbour =  spins_->getSpin( hrect_->getNeighbour(i,j) ); //nearest neighbour along 
                                                                 //j direction
      energyJ   += currSpin->dot( neighbour );
    } //j
  } //i
  
  //field term:
  for( uint i=0; i<N_; i++ )
  { 
    currSpin =  spins_->getSpin(i);
    energyh  += currSpin->v_[0]; 
  }
  
  return ((-1.0*J_*energyJ) + (-1.0*h_*energyh)) ;
} //getEnergy()

/*********************************** getHelicityModulus() ************************************/
double ON_Model_Ngeq2::getHelicityModulus(int dir)
{
  double       helicityMod = 0; //resulting helicity modulus
  Vector_NDim* currSpin;        //current spin
  Vector_NDim* neigh;           //current neighbour
  double       sum1=0;          //first sum in formula for helicity modulus
  double       sum2=0;          //second sum in formula for helicity modulus
   
  //loop over all spins:
  for(uint i=0; i<N_; i++)
  {
   currSpin = spins_->getSpin(i);
   neigh    = spins_->getSpin( hrect_->getNeighbour(i,dir) ); //nearest neighbour along +x
   //neigh = spins[neighbours[i][neighDir]];
   
   sum1 += currSpin->dotForRange(neigh,0,1);
   sum2 += ( currSpin->v_[0] * neigh->v_[1] ) - ( currSpin->v_[1] * neigh->v_[0] );
  }
  helicityMod = sum1/(1.0*N_) - J_/(T_*N_)*pow(sum2,2); 
  
  return helicityMod;
}

/************************************* getMagnetization() ************************************/
/*Vector_NDim* ON_Model_Ngeq2::getMagnetization()
{
  Vector_NDim* mag = new Vector_NDim( spinDim_,0 );
  
  for( uint i=0; i<N_; i++ )
  { mag->add( spins_->getSpin(i) ); }
  
  return mag;
}*/

/*************************************** getSigma1Tot() ***************************************/
double ON_Model_Ngeq2::getSigma1Tot()
{
  int sigma1Tot = 0;
  
  for( uint i=0; i<N_; i++ )
  { sigma1Tot += spins_->getSpin(i)->v_[0]; }
  
  return sigma1Tot;
}

/******************************* localUpdate(MTRand* randomGen) ******************************/
void ON_Model_Ngeq2::localUpdate(MTRand &randomGen)
{ 
  uint         latticeSite; //randomly selected spin location
  double       deltaE;
  Vector_NDim* spin_old = NULL;  //previous state of spin (at randomly selected lattice site)
  Vector_NDim* spin_new = NULL;  //new proposed value for spin
  Vector_NDim  nnSum(spinDim_, 0);
  
  //randomly generate a new spin:
  spin_new = new Vector_NDim(spinDim_, randomGen);
  
  //randomly select a spin on the lattice:
  latticeSite = randomGen.randInt(N_-1);
  spin_old    = spins_->getSpin(latticeSite);
  
  //loop to calculate the nearest neighbour sum:
  for( uint i=0; i<D_; i++ )
  { 
    nnSum.add( spins_->getSpin( hrect_->getNeighbour( latticeSite, i    ) ) ); 
    nnSum.add( spins_->getSpin( hrect_->getNeighbour( latticeSite, i+D_ ) ) );
  }
  
  //calculate the energy change for the proposed move:
  deltaE = -1.0*J_*( nnSum.dot(spin_new) - nnSum.dot(spin_old) )
           - h_*( spin_new->v_[0] - spin_old->v_[0] );
  
  //if the move is accepted:
  if( deltaE<=0 || randomGen.randDblExc() < exp(-deltaE/T_) )
  { 
    //delete the vector storing the old state of the spin:
    if(spin_old!=NULL)
    { delete spin_old; }
    spin_old = NULL;
    
    spins_->setSpin( latticeSite, spin_new );
    numAccept_local_++;
  }
  //otherwise, the move is rejected:
  else
  {
    //delete the vector storing the rejected new state of the spin:
    if(spin_new!=NULL)
    { delete spin_new; }
    spin_new = NULL;  
  }
}

/************************************* makeMeasurement() *************************************/
void ON_Model_Ngeq2::makeMeasurement()
{
  double energyPerSpin = getEnergy()/(1.0*N_);
  double sigma1PerSpin = getSigma1Tot()/(1.0*N_);
  double helicityMod_x = getHelicityModulus(0);
  double helicityMod_y = getHelicityModulus(1);
  
  measures.accumulate( "E",             energyPerSpin ) ;
  measures.accumulate( "ESq",           pow(energyPerSpin,2) );
  measures.accumulate( "sigma1",        sigma1PerSpin );
  measures.accumulate( "helicityMod_x", helicityMod_x );
  measures.accumulate( "helicityMod_y", helicityMod_y );
}

/**************************************** printSpins() ***************************************/
void ON_Model_Ngeq2::printSpins()
{ spins_->print(); }

/**************************** randomizeLattice(MTRand* randomGen) ****************************/
void ON_Model_Ngeq2::randomizeLattice(MTRand &randomGen)
{ 
  spins_->randomize(randomGen);
}

/********************************** sweep(MTRand* randomGen) *********************************/
void ON_Model_Ngeq2::sweep(MTRand &randomGen, bool pr)
{ 
  uint N1 = N_/2;     //number of local updates before Wolff step
  uint N2 = N_ - N1;  //number of local updates after Wolff step
  
  for( uint i=0; i<N1; i++ )
  { localUpdate(randomGen); }
  
  wolffUpdate(randomGen, 0, (spinDim_-1), pr);
  
  for( uint i=0; i<N2; i++ )
  { localUpdate(randomGen); }
}

/******************************* wolffUpdate(MTRand* randomGen) ******************************/
void ON_Model_Ngeq2::wolffUpdate(MTRand &randomGen, uint start, uint end, bool pr)
{
  uint               latticeSite;
  uint               neighSite;
  uint               clustSize;
  double             PAdd;
  double             exponent;  //exponent for PAdd
  double             onsiteEnergy_initial;
  double             onsiteEnergy_final;
  double             onsiteEnergy_diff;
  double             PAcceptCluster;
  double             rDotRef;
  Vector_NDim*       r = new Vector_NDim(spinDim_,randomGen, start, end);
  Vector_NDim*       reflectedSpin = NULL;
  std::vector<uint>  buffer;  //indices of spins to try to add to cluster (will loop until 
                              //buffer is empty)
  std::vector<uint>  cluster; //vector storing the indices of spins in the cluster
  
  buffer.reserve(N_);
  cluster.reserve(N_);
  
  latticeSite = randomGen.randInt(N_-1);
  //flipSpin(latticeSite, r);  //don't flip yet for efficiency
  inCluster_[latticeSite] = 1;
  cluster.push_back(latticeSite);
  buffer.push_back(latticeSite);
  
  while( !buffer.empty() )
  {
    latticeSite = buffer.back();
    buffer.pop_back();
    
    //Note: the spin at site 'latticeSite' is not flipped yet so we have to consider the
    //      energy difference that would result if it were already flipped:
    reflectedSpin = spins_->getSpin(latticeSite)->getReflectionAndNormalize(r);
    rDotRef       = r->dot(reflectedSpin);
    
    for( uint i=0; i<(2*D_); i++ )
    {
      neighSite = hrect_->getNeighbour( latticeSite, i );
      
      exponent = (2.0*J_/T_)*rDotRef*( r->dot(spins_->getSpin(neighSite)) );
      
      if (exponent < 0 )
      { 
        PAdd = 1.0 - exp(exponent);
        if( !( inCluster_[ neighSite ] ) && (randomGen.randDblExc() < PAdd) )
        { 
          //flipSpin(neighbours[site][i],r); 
          inCluster_[ neighSite ] = 1;
          cluster.push_back( neighSite );
          buffer.push_back( neighSite );
        }
      }
    }  //for loop over neighbours
    
    if(reflectedSpin!=NULL)
    { delete reflectedSpin; }
    reflectedSpin = NULL;
    
  } //while loop for buffer
  
  
  //flip the cluster if it is accepted:
  onsiteEnergy_initial = getClusterOnSiteEnergy(cluster);
  flipCluster(cluster, r); //flip in order to calculate the final energy
  onsiteEnergy_final = getClusterOnSiteEnergy(cluster);
  onsiteEnergy_diff = onsiteEnergy_final - onsiteEnergy_initial;
  
  if( pr || writeClusts_ )
  { 
    clustSize = cluster.size(); 
    if( writeClusts_ )
    { clustSizes_[clustSize-1]++; }
  }
  
  //If the onsite energy diff. is negative, accept the cluster move. If the onsite energy 
  //diff. is positive, accept the cluster move with probability exp^(-beta*onsiteEnery_diff).
  //Note: cluster is already flipped currently, so this is a check to see if we need to flip 
  //      it back:
  if(pr)
  { r->print(); }
  if( onsiteEnergy_diff > 0 )
  {
    PAcceptCluster = exp(-1.0*onsiteEnergy_diff/T_);
    if(pr)
    {
      std::cout << "  PAccept = " << PAcceptCluster << std::endl;
      std::cout << "  size = " << clustSize << std::endl << std::endl;
    }
    //check if we need to flip the cluster back to its original orientation:
    if( randomGen.randDblExc() >= PAcceptCluster )
    { 
      flipCluster(cluster, r);
      if( writeClusts_ )
      { clustSizes_rejected_[clustSize-1]++; }
    }
    else
    { 
      numAccept_clust_++;
      if( writeClusts_ )
      { clustSizes_accepted_[clustSize-1]++; }
    }
  }
  else 
  {
    numAccept_clust_++;
    if( writeClusts_ )
    { clustSizes_accepted_[clustSize-1]++; }
    
    if(pr)
    {
      std::cout << "  onsite <= 0" << std::endl;
      std::cout << "  size = " << clustSize << std::endl << std::endl;
    }
  }
  
  clearCluster(cluster);
  
  if(r!=NULL)
  { delete r; }
  r = NULL;
} //wolffUpdate()
