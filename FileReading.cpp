/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   FileReading.cpp
**********************************************************************************************/

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "FileReading.h"

//typdefs needed because uint and ulong are return types:
typedef FileReading::uint  uint;
typedef FileReading::ulong ulong; 

/************************* readDouble(std::ifstream* fin, char delim) *************************
* This method reads in and parses a double from the given input stream.
**********************************************************************************************/
double FileReading::readDouble(std::ifstream* fin, char delim)
{
  std::string currLine;
  std::size_t index;
  double result=0;
  
  if( fin!=NULL && fin->is_open() )
  {
    getline(*fin, currLine);
    index = currLine.find_last_of(delim);
    result = strtod( (currLine.substr(index+1)).c_str(), NULL);
  }
  
  return result;
} //readDouble method

/******** readDoubleVec(std::ifstream* fin, char delim, char startChar, char endChar); ********
* This method reads in and parses a vector of doubles from the given input stream.
**********************************************************************************************/
std::vector<double>* FileReading::readDoubleVec(std::ifstream* fin, char delim, 
                                                char startChar, char endChar)
{
  std::vector<double>* result = new std::vector<double>;
  std::string          currLine;
  std::size_t          delimIndex;
  std::size_t          startIndex;
  std::size_t          endIndex;
  std::size_t          commaIndex;
  
  if( fin!=NULL && fin->is_open() )
  {
    //cut off the part of the line up to and including the delim character:
    getline(*fin, currLine);
    delimIndex = currLine.find_last_of(delim);
    currLine = currLine.substr(delimIndex+1);
  
    //cut off the characters that indicate the start and end of the list:
    startIndex = currLine.find_first_of(startChar);
    endIndex = currLine.find_last_of(endChar);
    currLine = currLine.substr(startIndex+1, (endIndex - startIndex - 1) );
  
    //read in the doubles in the list and store them in TList_:
    commaIndex = currLine.find_first_of(",");
    while( commaIndex<currLine.size() )
    {
      result->push_back( strtod( (currLine.substr(0, commaIndex)).c_str(), NULL) );
      currLine = currLine.substr(commaIndex+1);
      commaIndex = currLine.find_first_of(",");
    }
    result->push_back( strtod( currLine.c_str(), NULL) );
  }
  
  return result;
} //readDoubleVec method

/************************** readUint(std::ifstream* fin, char delim) **************************
* This method reads in and parses an unsigned integer from the given input stream.
**********************************************************************************************/
uint FileReading::readUint(std::ifstream* fin, char delim)
{
  std::string currLine;
  std::size_t index;
  uint result = 0;
  
  if( fin!=NULL && fin->is_open() )
  {
    getline(*fin, currLine);
    index = currLine.find_last_of(delim);
    result = (uint)strtoul( (currLine.substr(index+1)).c_str(), NULL, 0 );
  }
  
  return result;
} //readUint method

/************* readUintArray(std::ifstream* fin, uint size, uint defaultVal, ... **************
**************               char delim, char startChar, char endChar);          **************
* This method reads in and parses an array of unsigned integers from the given input stream.
**********************************************************************************************/
uint* FileReading::readUintArray(std::ifstream* fin, uint size, uint defaultVal, char delim, 
                                 char startChar, char endChar)
{
  uint*       result = new uint[size];
  uint        arrayPos;
  std::string currLine;
  std::size_t delimIndex;
  std::size_t startIndex;
  std::size_t endIndex;
  std::size_t commaIndex;
  
  //initially, result will be the passed default value:
  for( uint i=0; i<size; i++ )
  { result[i] = defaultVal; }
  
  if( fin!=NULL && fin->is_open() )
  {
    //cut off the part of the line up to and including the delim character:
    getline(*fin, currLine);
    delimIndex = currLine.find_last_of(delim);
    currLine = currLine.substr(delimIndex+1);
  
    //cut off the characters that indicate the start and end of the list:
    startIndex = currLine.find_first_of(startChar);
    endIndex = currLine.find_last_of(endChar);
    currLine = currLine.substr(startIndex+1, (endIndex - startIndex - 1) );
  
    //read in the doubles in the list and store them in TList_:
    commaIndex = currLine.find_first_of(",");
    arrayPos   = 0;
    //loop for as long as there are elements to read and as long as we do not exceed the 
    //requested size:
    while( commaIndex<currLine.size() && arrayPos<size )
    {
      result[arrayPos] = (uint)strtoul( (currLine.substr(0, commaIndex)).c_str(), NULL, 0 ) ;
      currLine = currLine.substr(commaIndex+1);
      commaIndex = currLine.find_first_of(",");
      arrayPos++;
    }
    if( arrayPos < size )
    { result[arrayPos] = (uint)strtoul( currLine.c_str(), NULL, 0 ); }
  }
  
  return result;
}

/************************* readULong(std::ifstream* fin, char delim) **************************
* This method reads in and parses an unsigned long integer from the given input stream.
**********************************************************************************************/
ulong FileReading::readULong(std::ifstream* fin, char delim)
{
  std::string currLine;
  std::size_t index;
  ulong result = 0;
  
  if( fin!=NULL && fin->is_open() )
  {
    getline(*fin, currLine);
    index = currLine.find_last_of(delim);
    result = strtoul( (currLine.substr(index+1)).c_str(), NULL, 0);
  }
  
  return result;
} //readLongInt method

/************************* readString(std::ifstream* fin, char delim) *************************
* This method reads in a string from the given input stream.
**********************************************************************************************/
std::string FileReading::readString(std::ifstream* fin, char delim)
{
  std::string currLine;
  std::string result = "";
  std::size_t index;
  
  if( fin!=NULL && fin->is_open() )
  {
    //cut off the part of the line up to and including the delim character:
    getline(*fin, currLine);
    index = currLine.find_last_of(delim);
    currLine = currLine.substr(index+1);
  
    //trim the leading whitespace:
    index = currLine.find_first_not_of(" \t\n");
    currLine = currLine.substr(index);
  
    //trim the trailing whitespace:
    index = currLine.find_last_not_of(" \t\n");
    result = currLine.substr(0,index+1);
  }
  
  return result;
} //readString method

/***************** readUntilFound(std::ifstream* fin, std::string searchStr) ******************
* This method reads lines from a file until it either reaches the end of the file or finds a 
* that contains the given std::string searchStr. 
**********************************************************************************************/
void FileReading::readUntilFound(std::ifstream* fin, std::string searchStr)
{ 
  std::string currLine;
  bool        doneReading;
  std::size_t index;
  
  if( fin!=NULL )
  {
    doneReading = fin->eof();
    
    while( !doneReading )
    {
      getline(*fin, currLine);
      index = currLine.find(searchStr);
      if( index != std::string::npos || fin->eof() )
      { doneReading = true; }
    } //while
  } //if
}
