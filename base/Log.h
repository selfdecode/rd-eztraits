/* Copyright (c) 2019 by Digidactylus AB.
   See LICENSE for more information.
*/

#ifndef LOG_H
#define LOG_H


#include <sstream>
#include <string>
#include <stdio.h>
#include "base/ErrorHandling.h"
#include <iostream>
using namespace std;


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)

#include <windows.h>

inline void MSleep(int ms) {
  Sleep(ms);
}

#else

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

inline void MSleep(int ms) {
  usleep(ms*1000);
}


#endif //WIN



//==========================================================

class MLogger
{
public:

  MLogger() {
    m_pLog = NULL;
    m_stdOut = false;
    m_newLine = true;
    m_first = -1;
    m_timer = -1;
    m_flip = 1000000;
    m_level = 0;
  }

  MLogger(const string & fileName) {
    m_pLog = NULL;
    m_stdOut = false;
    m_newLine = true;
    m_first = -1;
    m_timer = -1;
    m_flip = 1000000;
    Open(fileName);
  }
  
  
  ~MLogger() {
    Close();
  }


  bool Open(const string & fileName) {
    Close();
    //cout << "Open log file: " << fileName << endl;
    m_pLog = fopen(fileName.c_str(), "a");
    if (m_pLog == NULL) {
      //cout << "FAILED!" << endl;
      return false;
    } else {
      return true;
    }
  }

  void Close() {
    if (m_pLog != NULL)
      fclose(m_pLog);
    m_pLog = NULL;

  }

  void StdOut(bool b) {
    m_stdOut = b;
  }
  
  void Print(const string & s) {
    if (m_newLine) {
      string t = TimeStamp();
      string extra;
      if (m_level == 1) {
	extra = "WARNING - ";
      }
      if (m_level == 2) {
	extra = "ERROR - ";
      }
      if (m_pLog != NULL) {
	fprintf(m_pLog, "- %s %s", t.c_str(), extra.c_str());
	//fflush(m_pLog);
      }
      if (m_stdOut) {
	printf("- %s %s", t.c_str(), extra.c_str());     
      }
      m_level = 0;
    }
    
    if (m_pLog != NULL) {
      fprintf(m_pLog, "%s", s.c_str());
      fflush(m_pLog);
    }
    if (m_stdOut) 
      printf("%s", s.c_str());     

    
    m_newLine = false;
  }
  
  // function that takes a custom stream, and returns it
  typedef MLogger& (*MLoggerManipulator)(MLogger&);
  
  // take in a function with the custom signature
  MLogger& operator<<(MLoggerManipulator manip)
  {
    // call the function, and return it's value
    return manip(*this);
  }
  
  // define the custom endl for this stream.
  // note how it matches the `MLoggerManipulator`
  // function signature

  /*static MLogger& endl(MLogger& stream)
  {
    // print a new line
    // std::cout << std::endl;
    //fprintf(m_pLog, "\n");

    // do other stuff with the stream
    // std::cout, for example, will flush the stream
    //stream << "Called MLogger::endl!" << std::endl;
    
    return stream;
    }*/
  
  // this is the type of std::cout
  typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
  
  // this is the function signature of std::endl
  typedef CoutType& (*StandardEndLine)(CoutType&);
  
  // define an operator<< to take in std::endl
  MLogger& operator<<(StandardEndLine manip)
  {
    // call the function, but we cannot return it's value
    //manip(std::cout);
    if (m_pLog != NULL) {
      fprintf(m_pLog, "\n");
      fflush(m_pLog);
    }
    if (m_stdOut)
      printf("\n");
    m_newLine = true;
  
    return *this;
  }

  void SetLogLevel(int n) {
    m_level = n;
  }
  
  void StartTimer() {
    m_timer = GetMilliseconds();
    string tag = TimeStamp();
    string t = "**************** Start TIMER";
    if (m_pLog != NULL) {
      fprintf(m_pLog, "- %s %s\n", tag.c_str(), t.c_str());
      fflush(m_pLog);
    }
    if (m_stdOut) {
      printf("- %s %s\n", tag.c_str(), t.c_str());     
    }
 
  }

  int Elapsed() {
    int t = GetMilliseconds() - m_timer;
    if (t < 0)
      t += m_flip;

    string tag = TimeStamp();
    string text = "**************** End TIMER, elapsed: ";
    if (m_pLog != NULL) {
      fprintf(m_pLog, "- %s %s%d ms\n", tag.c_str(), text.c_str(), t);
      fflush(m_pLog);
    }
    if (m_stdOut) {
      printf("- %s %s%dms\n", tag.c_str(), text.c_str(), t);      
    }
 
    return t;
  }
  
 private:


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)

  string TimeStamp() {
    const int MAX_LEN = 200;
    char buffer[MAX_LEN];
    if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, 
    "HH':'mm':'ss", buffer, MAX_LEN) == 0) {
       return "Error in TimeStamp()";
    }
  
    char result[100] = {0};

    if (m_first < 0)
      m_first = GetTickCount();
  
    //std::sprintf(result, "%s.%03ld", buffer, (long)(GetTickCount() - m_first) % 1000); 
    std::sprintf(result, "%s", buffer); 
   
    return result;
  }

  int GetMilliseconds() {
    return GetTickCount();
  }
  
#else


  string TimeStamp()
  {
    char buffer[32];
    time_t t;
    time(&t);
    tm r = tm();
    
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    struct timeval tv;
    gettimeofday(&tv, 0);
    char result[100] = {0};
    
    sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000); 
    return result;
  }

  int GetMilliseconds() {
    timespec tp;
    clock_gettime(CLOCK_REALTIME,
                  &tp);
    int t = tp.tv_sec % m_flip;
    t *= 1000;
    t += tp.tv_nsec/1000000;
    //cout << "TIMER: " << tp.tv_sec % m_flip << " " << t << " " << tp.tv_nsec << endl; 
    return t;
  }
    

#endif //WIN


  int m_level;

  int m_first;
  int m_timer;
  FILE * m_pLog;
  bool m_stdOut;
  bool m_newLine;
  int m_flip;
};


template <typename T>
MLogger& operator<<(MLogger & log, const T& x)
{
  stringstream s;
  s << x;
  
  //cout << s.str();
  log.Print(s.str());
  //fprintf(m_pLog, "%s", s.str().c_str());
  return log;
}






#endif


