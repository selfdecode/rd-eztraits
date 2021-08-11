#ifndef REPORTING_H
#define REPORTING_H

#include "base/SVector.h"
#include <stdio.h>
#include "base/Log.h"
#include "util/mutil.h"

class Reporting
{
 public:
  Reporting() {
    m_pLog = NULL;
    m_version = 2;
  }
  
  ~Reporting() {
    Close();
  }

  void Open(const string & fileName) {
    Close();
    m_pLog = fopen(fileName.c_str(), "w");
    if (m_pLog == NULL) {
      ThrowError("ERROR: Cannot open report file: ", fileName);
    }
  }
  
  void Close() {
    if (m_pLog != NULL)
      fclose(m_pLog);
    m_pLog = NULL;
  }

  void Log(const string & log) {
    if (m_pLog != NULL)
      fprintf(m_pLog, "%s\n", log.c_str());
  }

  void Log(const string & key, const string & value) {
    if (m_pLog != NULL)
      fprintf(m_pLog, "%s: %s\n", key.c_str(), value.c_str());
  }
  
  void Log(const string & key, int value) {
    if (m_pLog != NULL)
      fprintf(m_pLog, "%s: %d\n", key.c_str(), value);
  }
  
  void Log(const string & key, double value) {
    if (m_pLog != NULL)
      fprintf(m_pLog, "%s: %f\n", key.c_str(), value);
  }
  
  void Log(const string & key, double value, double value2) {
    if (m_pLog != NULL)
      fprintf(m_pLog, "%s: %f - %f\n", key.c_str(), value, value2);
  } 

  void Error(const string & key, const string & value = "") {
    if (m_pLog != NULL)
      fprintf(m_pLog, "ERROR: %s %s\n", key.c_str(), value.c_str());
    m_tag.push_back("ERROR " + key); 
    m_msg.push_back(value); 
  }
  
  void Warning(const string & key, const string & value = "") {
    if (m_pLog != NULL)
      fprintf(m_pLog, "WARNING: %s %s\n", key.c_str(), value.c_str());
    m_tag.push_back("WARNING " + key); 
    m_msg.push_back(value); 
  }

  void Section(const string & key) {
    if (m_pLog != NULL) {
      fprintf(m_pLog, "-------- %s ", key.c_str());
      for (int i=(int)key.length(); i<50; i++)
	fprintf(m_pLog, "-");
      fprintf(m_pLog, "\n");
    }
  }

  MLogger & Stream(int level = 0) {
    m_syslog.SetLogLevel(level);
    return m_syslog;
  }

  // Read/write buffer IO
  void Read(IMReadStream & s) {
    s.Read(m_version);
    int n;
    s.Read(n);
    int i;
    for (i=0; i<n; i++) {
     
      CMString tag, msg;
      s.Read(tag);
      s.Read(msg);
     
      m_tag.push_back((const char*)tag);
      m_msg.push_back((const char*)msg);
      
    }
  }

  void Write(IMWriteStream & s) const {
    s.Write(m_version);
    
    s.Write(m_tag.isize());
    for (int i=0; i<m_tag.isize(); i++) {
      s.Write(m_tag[i].c_str());
      s.Write(m_msg[i].c_str());      
    }    
  }

 private:
  FILE * m_pLog;
  MLogger m_syslog;
  svec<string> m_tag;
  svec<string> m_msg;
  int m_version;
};


#endif //REPORTING_H


