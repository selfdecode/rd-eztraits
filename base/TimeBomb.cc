#include "base/TimeBomb.h"
#include "base/StringUtil.h"
#include "base/RandomStuff.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "base/FileParser.h"


const string g_msg = "(c) 2020 by Methority. For licensing questions, contact manfred.grabherr@methority.com";

string TimeBomb::CheckSum(const string & in)
{
  int i, j;

  int n = 0;
  for (i=0; i<(int)in.length(); i++) {
    int x = (int)in[i];
    for (j=0; j<8; j++) {
      if (x & 0x01)
	n++;
      x = (x >> 1);
    }
  }
  return Stringify(n);
}


void TimeBomb::Check(int hour,
		     int min,
		     int sec,
		     int year,
		     int month,
		     int day,
		     int days_left)
{

  
  
  struct tm start; // = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  start.tm_hour = hour;
  start.tm_min = min;
  start.tm_sec = sec;
  start.tm_year = year-1900;
  start.tm_mon = month-1;
  start.tm_mday = day;

  /*
  start.tm_hour = 0;
  start.tm_min = 0;
  start.tm_sec = 0;
  start.tm_year = 120;
  start.tm_mon = 0;
  start.tm_mday = 1;
  */

  
  time_t timer;
  time(&timer);

  double seconds = difftime(timer,mktime(&start));

  long int seconds_left = days_left*60*60*24;

  //cout << "Seconds: " << seconds << " left " << seconds_left << endl;

  
  seconds_left -= seconds;

  seconds = seconds_left;
  
  long int dl = seconds/60/60/24;
  seconds -= dl*60*60*24;
  long int hl = seconds/60/60;
  seconds -= hl*60*60;
  long int ml = seconds/60;
  seconds -= ml*60;

  cout << "INFO (free trial): " << dl << " days, "; 
  cout << hl << " hours, " << ml << " minutes, and " << seconds << " seconds left!" << endl; 
  cout << endl;

  
  if (seconds_left < 0) {
    cout << "ERROR - your free trial license has expired!!" << endl;
    cout << "ERROR - will throw an exception NOW!!" << endl;
    cout << "ERROR - terminating!!" << endl;

    cout << "          _ ._  _ , _ ._" << endl;
    cout << "        (_ ' ( `  )_  .__)" << endl;
    cout << "      ( (  (    )   `)  ) _)" << endl;
    cout << "     (__ (_   (_ . _) _) ,__)" << endl;
    cout << "         `~~`\\ ' . /`~~`" << endl;
    cout << "              ;   ;" << endl;
    cout << "              /   \\" << endl;
    cout << "_____________/_ __ \\\";_____________" << endl;

    cout << endl;

    if (m_crash)
      throw;
   

  }
  
}



void TimeBomb::Check(const string & fileName, const string & key)
{
  FILE * pF = fopen(fileName.c_str(), "r");
  if (pF == NULL) {
    cout << "ERROR: License file " << fileName << " not found!!" << endl;
    cout << endl << g_msg << endl;
    exit(-1);
  }
  
  fclose(pF);

  //const string hash = "kIgd652$&*!#*009adsnBCH{[a7&!0983qHAB";
  
  FlatFileParser parser;
  
  parser.Open(fileName);

  parser.ParseLine();
  
  string s = parser.Line();
  Hash(s, key);
  
  StringParser p;
  p.SetLine(s);

  if (p.GetItemCount() < 8) {
    cout << "ERROR: License file invalid!!!" << endl;
    cout << endl << g_msg << endl;
    exit(-1);
  }
  
  int hour = p.AsInt(0);
  int min = p.AsInt(1);
  int sec = p.AsInt(2);
  int year = p.AsInt(3);
  int month = p.AsInt(4);
  int day = p.AsInt(5);
  int days_left = p.AsInt(6);
  string checksum = p.AsString(7);

  string cc;
  for (int i=0; i<7; i++)
    cc += p.AsString(i) + " ";
  
  string check = CheckSum(cc);

  //cout << "File: " << checksum  << " required " << check << endl;
  if (checksum != check) {
    cout << "ERROR: license file is corrupt!! Exiting." << endl << endl;
    cout << endl << g_msg << endl;
    if (m_crash)
      exit(-1);
    else
      return;
  }

  
  Check(hour, min, sec, year, month, day, days_left);
}


void TimeBomb::Hash(string & s, const string & key)
{
  string out = key;

  if (s.length() > key.length())
    throw;

  //cout << "|" << s << "|" << endl;
  
  for (int i=0; i<(int)key.length(); i++) {
    if (i<(int)s.length()) {
      out[i] = 32+((s[i]-32) ^ (key[i]-32));
      //cout << "hash " << s[i] << " -> " << out[i] << " " << (int)out[i] << endl;
    } else {
      out[i] = key[i];
    }
  }
  s = out;
}

string TimeBomb::MakeKey(int n)
{
  string s;
  int i;
  double l = 0.;
  for (i=0; i<n; i++) {
    l += RandomFloat(1.);
  }

  for (i=0; i<128; i++) {
    char t = (char)(33+RandomInt(126-33));
    if (t == '\\' || t == '\"') {
      i--;
      continue;
      
    }
    s += t;
  }

  return s;
}

void TimeBomb::MakeHash(const string & fileName,
			const string & key,
			int hour,
			int min,
			int sec,
			int year,
			int month,
			int day,
			int days_left)
{
  string s = Stringify(hour) + " ";
  s += Stringify(min) + " ";
  s += Stringify(sec) + " ";
  s += Stringify(year) + " ";
  s += Stringify(month) + " ";
  s += Stringify(day) + " ";
  s += Stringify(days_left) + " ";

  //cout << "Check: " << CheckSum(s) << endl;
  s += CheckSum(s);
  
  Hash(s, key);

  FILE * p = fopen(fileName.c_str(), "w");
  fprintf(p, "%s\n", s.c_str());
  fclose(p);
  

}

void TimeBomb::MakeHashAuto(const string & fileName,
			    const string & key,
			    int days_left)
{

  
  
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

  //cout << "BUFFER: " << buffer << endl;
  
  //struct timeval tv;
  //gettimeofday(&tv, 0);

  //cout << tv.
    //sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000); 

  
  
  string s = Stringify(tm.tm_hour) + " ";
  s += Stringify(tm.tm_min) + " ";
  s += Stringify(tm.tm_sec) + " ";
  s += Stringify(tm.tm_year+1900) + " ";
  s += Stringify(tm.tm_mon+1) + " ";
  s += Stringify(tm.tm_mday) + " ";
  s += Stringify(days_left) + " ";

  //cout << "Check: " << CheckSum(s) << endl;
  s += CheckSum(s);

  Hash(s, key);

  FILE * p = fopen(fileName.c_str(), "w");
  cout << "Write to " << fileName << endl;
  fprintf(p, "%s\n", s.c_str());
  fclose(p);
  

}

