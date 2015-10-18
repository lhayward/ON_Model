/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   ON_Model.cpp (Abstract Class)
**********************************************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include "FileReading.h"
#include "ON_Model.h"

/***************** ON_Model(std::ifstream* fin, std::string outFileName, ... ******************
******************          Hyperrectangle* lattice)                         ******************
******************                       (constructor)                       *****************/
ON_Model::ON_Model(std::ifstream* fin, std::string outFileName, Hyperrectangle* lattice)
{ 
  const char EQUALS_CHAR = '=';
  D_ = 0;
  L_ = 0;
  N_ = 0;
  
  if( fin!=NULL && fin->is_open() )
  {
    J_ = FileReading::readDouble(fin, EQUALS_CHAR);
    h_ = FileReading::readDouble(fin, EQUALS_CHAR);
  }
  else
  { 
    std::cout << "ERROR in ON_Model constructor: could not read from input file\n" << std::endl; 
  }
  
  if( lattice != NULL )
  {
    hrect_ = lattice;
    D_ = hrect_->getD();
    L_ = hrect_->getL();
    N_ = hrect_->getN();
  } //if for non-null Lattice object
  else
  {
    std::cout << "ERROR in ON_Model constructor: The passed Hyperrectangle object is not "
              << "valid\n" << std::endl; 
  }
  
  fout.open(outFileName.c_str());
  fout.precision(15);
  
  warmupDone = false;
  
  //initialize the temperature (should be changed by user to desired temperature before
  //starting the simulation):
  T_ = 1.0;
  
  inCluster_ = new bool[N_];
  for( uint i=0; i<N_; i++ )
  { inCluster_[i] = 0; }
  
  //Add measurement names to Measure object:
  measures.insert("E");
  measures.insert("ESq");
  measures.insert("AccRate_local");
  measures.insert("AccRate_clust");
  numAccept_local_ = 0;
  numAccept_clust_ = 0;
  
  //if we will track cluster sizes, initialize the corresponding arrays:
  if( writeClusts_ )
  {
    clustSizes_          = new uint[N_];
    clustSizes_accepted_ = new uint[N_];
    clustSizes_rejected_ = new uint[N_];
    
    for( uint i=0; i<N_; i++ )
    { 
      clustSizes_[i]          = 0;
      clustSizes_accepted_[i] = 0;
      clustSizes_rejected_[i] = 0; 
    }
  } //if for writeClusts_
}

/********************************** ~ON_Model() (destructor) *********************************/
ON_Model::~ON_Model()
{ 
  fout.close(); 
  
  if( inCluster_ != NULL )
  { delete[] inCluster_; }
  inCluster_ = NULL;
  
  if( writeClusts_ )
  {
    if( clustSizes_ != NULL )
    { delete[] clustSizes_; }
    clustSizes_ = NULL;
    
    if( clustSizes_accepted_ != NULL )
    { delete[] clustSizes_accepted_; }
    clustSizes_accepted_ = NULL;
    
    if( clustSizes_rejected_ != NULL )
    { delete[] clustSizes_rejected_; }
    clustSizes_rejected_ = NULL;
  }
}

/************************************ changeT(double newT) ***********************************/
void ON_Model::changeT(double newT)
{ 
  T_ = newT;
  warmupDone = false;
}

/*************************************** clearCluster ***************************************/ 
void ON_Model::clearCluster(std::vector<uint> &cluster)
{
  uint clustSize = (uint)cluster.size();
  
  for( uint i=0; i<clustSize; i++ )
  { inCluster_[cluster[i]]=0; }
  
  //test to make sure the inCluster_ array was properly cleared (for testing purposes only):
  /*for( uint i=0; i<N_; i++ )
  {
    if( inCluster_[i]==1 )
    { std::cout << "*** 1 ***" << std::endl; }
  }*/
} //clearCluster

/************************************** markWarmupDone() *************************************/
void ON_Model::markWarmupDone()
{ 
  warmupDone = true; 
  
  //if we will track cluster sizes, set all stored sizes to zero (since warm-up has just 
  //finished and the user can start tracking measurements):
  if( writeClusts_ )
  {
    for( uint i=0; i<N_; i++ )
    { 
      clustSizes_[i] = 0;
      clustSizes_accepted_[i] = 0;
      clustSizes_rejected_[i] = 0; 
    }
  }
}

/*************************************** printParams() ***************************************/
void ON_Model::printParams()
{
  std::cout << "O(" << spinDim_ << ") Model Parameters:\n"
            << "---------------------" << std::endl;
  std::cout << "  J = " << J_ << "\n"
            << "  h = " << h_ << "\n";
}

/************************* writeClustHistoData(std::string fileName) *************************/
void ON_Model::writeClustHistoData(std::string fileName)
{
  std::ofstream fout_clust;
  
  if( writeClusts_ )
  {
    fout_clust.open(fileName.c_str());
    fout_clust.precision(15);
  
    fout_clust << "#T \t clustSize \t num_generated \t num_accepted \t num_rejected" << std::endl;
    for( uint i=0; i<N_; i++ )
    { 
      fout_clust << T_ << '\t' << (i+1) << '\t' << clustSizes_[i] << '\t' << clustSizes_accepted_[i] 
                 << '\t' << clustSizes_rejected_[i] << std::endl;
    }
    fout_clust.close();
  }
}

/******************************** uintPower(int base, int exp) *******************************/
uint ON_Model::uintPower(uint base, uint exp)
{
  uint result = 1;
  for(uint i=1; i<=exp; i++)
  { result *= base; } 
  
  return result;
} //uintPower method

/******************** writeBin(int binNum, int numMeas, int sweepsPerMeas) *******************/
void ON_Model::writeBin(int binNum, int numMeas, int sweepsPerMeas)
{
  //Note: the following two measurements will be divided by numMeas in the call to
  //writeAverages() such that acceptance rates are written to file
  measures.accumulate( "AccRate_local", numAccept_local_/(1.0*N_*sweepsPerMeas) );
  measures.accumulate( "AccRate_clust", numAccept_clust_/(1.0*sweepsPerMeas) );
  
  //if this is the first bin being written to file, then also write a line of text explaining
  //each column:
  if( binNum == 1)
  {
    fout << "# L \t T \t binNum";
    measures.writeMeasNames(&fout);
    fout << std::endl;
  }
  fout << L_[0] << '\t' << T_ << '\t' << binNum;
  measures.writeAverages(&fout, numMeas);
  fout << std::endl;
} //writeBin method

/************************************* zeroMeasurements() ************************************/
void ON_Model::zeroMeasurements()
{ 
  measures.zero();
  numAccept_local_ = 0;
  numAccept_clust_ = 0;
}