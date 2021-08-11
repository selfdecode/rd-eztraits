#ifndef TIMEBOMB_H
#define TIMEBOMB_H


#include "base/SVector.h"

class TimeBomb
{
 public:
  TimeBomb() {
    m_crash = true;
  }
  
  void Check(int hour,
	     int min,
	     int sec,
	     int year,
	     int month,
	     int day,
	     int days_left);
  
  void Check(const string & fileName, const string & key); 

  void MakeHash(const string & fileName,
		const string & key,
		int hour,
		int min,
		int sec,
		int year,
		int month,
		int day,
		int days_left);

  void MakeHashAuto(const string & fileName,
		    const string & key,
		    int days_left);

  string MakeKey(int n);

  void SetCrash(bool b) {
    m_crash = b;
  }
  
 private:
  // struct tm m_start;
  void Hash(string & s, const string & key);
  string CheckSum(const string & in);
  
  bool m_crash;
};










#endif


