/**********************************************************************************************
************************************ OPE COEFFICIENTS CODE ************************************
***********************************************************************************************
* Lauren E. Hayward Sierens
***********************************************************************************************
* File:   FileReading.h
**********************************************************************************************/

#ifndef FILE_READING_H
#define FILE_READING_H

#include <fstream>
#include <string>
#include <vector>

class FileReading 
{ 
  public:
    typedef unsigned int  uint;
    typedef unsigned long ulong;

    //methods to read from the passed input stream:
    static double               readDouble    (std::ifstream* fin, char delim);
    static std::vector<double>* readDoubleVec (std::ifstream* fin, char delim, char startChar,
                                               char endChar);
    static uint*                readUintArray (std::ifstream* fin, uint size, uint defaultVal,
                                               char delim, char startChar, char endChar);
    static uint                 readUint      (std::ifstream* fin, char delim);
    static ulong                readULong     (std::ifstream* fin, char delim);
    static std::string          readString    (std::ifstream* fin, char delim);
    static void                 readUntilFound(std::ifstream* fin, std::string searchStr);
};

#endif  // FILE_READING_H
