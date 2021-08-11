

#ifndef STREAMPARSER_H_
#define STREAMPARSER_H_


#include "util/mutil.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "base/SVector.h"
#include "base/FileParser.h"
#include <set>
#include <sstream>

using namespace std;



class StreamParser 
{
 public:
  StreamParser();

  virtual ~StreamParser();
  void Set(stringstream & ss);

  bool ParseLine();
  
  int GetItemCount();

  bool IsString(int index);
  bool IsInt(int index);
  bool IsFloat(int index);


  const string & AsString(int index);
  const string & Line();
  char AsChar(int index);
  int AsInt(int index);
  double AsFloat(int index);


  //void operator<< (const char* s);
 private:
  StringParser m_parser;
  svec<string> m_data;
  int m_count;

};






#endif

