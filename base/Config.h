/* Copyright (c) 2019 by Digidactylus AB.
   See LICENSE for more information.
*/


#ifndef CONFIG_H
#define CONFIG_H

#include "base/SVector.h"
#include "util/mutil.h"

class Reporting;

inline string FixSlashes(const string & s)
{
  int i;
  string o = s;
  for (i=0; i<(int)s.length(); i++) {

#ifdef _WIN64
    if (o[i] == '/')
      o[i] = '\\';
#else
    if (o[i] == '\\')
      o[i] = '/';

#endif
  }
  return o;
}

class ConfItem
{
  
 public:
  ConfItem() {
    m_valueInt = 0;
    m_valueDouble = 0.;
    m_valueBool = false;
  }

  const string & Key() const {return m_key;}
  string & Key()  {return m_key;}

  const string & ValueString() const {return m_valueString;}
  int ValueInt() const {return m_valueInt;}
  double ValueDouble() const {return m_valueDouble;}
  //bool ValueBool() const {return m_valueBool;}

  void SetKey(const string & key) {m_key = key;}
  void SetValue(const string & value);
  void SetValue(int value);
  void SetValue(double value);
  //void SetValue(bool value);


  
  string Get() const;

  
 private:
  string m_key;
  string m_valueString;
  int m_valueInt;
  double m_valueDouble;
  bool m_valueBool;
};

class Config
{
 public:
  Config() {
    m_version = 3;
  }

  int GetVersion() const {return m_version;}
  void SetVersion(int v) {m_version = v;}
  
  void Read(const string & fileName);
  void Write(const string & fileName) const;

  const string & ValueString(const string & key) const;
  int ValueInt(const string & key) const;
  double ValueDouble(const string & key) const;
  //bool ValueBool(const string & key) const;

  void SetValue(const string & key, const string & value);
  void SetValue(const string & key, int value);
  void SetValue(const string & key, double value);
  //void SetValue(const string & key, bool value);

  void Update(const string & key, string & value);
  void Update(const string & key, int & value);
  void Update(const string & key, double & value);
  void UpdateF(const string & key, double & value);
  //void Update(const string & key, bool & value);

  void Update(const string & key, string & value, const string & def);
  void Update(const string & key, int & value, int def);
  void Update(const string & key, double & value, int def);
  void UpdateF(const string & key, double & value, int def);
  //void Update(const string & key, bool & value, int def);

  
  void Read(IMReadStream & s);
  void Write(IMWriteStream & s);

  void Print(const string & fileName) const;
  void Print(Reporting & rep) const;
  void Print(svec<string> & out) const;
  void Print(svec<string> & keys, svec<string> & values) const;
  
 private:
  int Index(const string & key) const {
    for (int i=0; i<m_conf.isize(); i++) {
      if (m_conf[i].Key() == key)
	return i;
    }
    return -1;
  }

  int IndexPlus(const string & key) {
    int index = Index(key);
    if (index < 0) {
      ConfItem tmp;
      tmp.Key() = key;
      m_conf.push_back(tmp);
      return m_conf.isize()-1;
    } else {
      return index;
    }
  }
  
  svec<ConfItem> m_conf;
  int m_version;
};


#endif

