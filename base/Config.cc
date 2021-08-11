/* Copyright (c) 2019 by Digidactylus AB.
   See LICENSE for more information.
*/

#include "base/Reporting.h"

#include "base/Config.h"
#include "base/StringUtil.h"
#include "base/FileParser.h"
#include "base/Log.h"

void ConfItem::SetValue(const string & value)
{
  m_valueString = value;
  
  m_valueInt = atol(value.c_str());

  // Make this work for any locale...
  double f = atof(value.c_str());
  m_valueDouble = f;
  string loc = value.c_str();
  for (int i=0; i<(int)loc.size(); i++) {
    if (loc[i] == '.')
      loc[i] = ',';
  }
  double f2 = atof(loc.c_str());
  if (f2 > f)
    m_valueDouble = f2;
   
  
  //cout << "DEBUG " << value.c_str() << " -> " << m_valueDouble << " " << f << endl;

  
  if (value == "1" || value == "true" || value == "TRUE")
    m_valueBool = true;
  else
    m_valueBool = false;

  //log_n << m_valueString << " " << m_valueBool << endl; 
}

void ConfItem::SetValue(int value)
{
  m_valueInt = value;
  if (value != 0)
    m_valueBool = true;
  else
    m_valueBool = false;
  m_valueDouble = value;
  m_valueString = Stringify(value);
}

void ConfItem::SetValue(double value)
{
  m_valueInt = value;
  if (value != 0)
    m_valueBool = true;
  else
    m_valueBool = false;
  m_valueDouble = value;
  m_valueString = Stringify(value);
}

/*
void ConfItem::SetValue(bool value)
{
  log_n << "SetBool" << endl;
  m_valueBool = value;
  if (value) {
    m_valueInt = 1;
    m_valueDouble = 1.;
    m_valueString = "true";
  } else {
    m_valueInt = 0;
    m_valueDouble = 0.;
    m_valueString = "false";
  }
}
*/

string ConfItem::Get() const
{
  string out = m_key + " " + m_valueString;
  return out;
}

//=================================================================
void Config::Read(IMReadStream & s)
{
  s.Read(m_version);
  int n;
  s.Read(n);
  int i;
  for (i=0; i<n; i++) {
    ConfItem item;
    CMString key, value;
    s.Read(key);
    s.Read(value);
    //log_n << "C++ config: " << key << "=" << value << endl;
    item.Key() = key;
    string v = (const char*)value;
    item.SetValue(v);
    m_conf.push_back(item);
 
  }

}

void Config::Write(IMWriteStream & s)
{
  s.Write(m_version);
 
  s.Write(m_conf.isize());
  for (int i=0; i<m_conf.isize(); i++) {
    s.Write(m_conf[i].Key().c_str());
    s.Write(m_conf[i].ValueString().c_str());
  
  }
}
  
void Config::Read(const string & fileName)
{
  FlatFileParser parser;
  
  parser.Open(fileName);

  while (parser.ParseLine()) {
    if (parser.GetItemCount() < 2)
      continue;
    if (parser.AsString(0)[0] == '#')
      continue;
    string key = parser.AsString(0);
    string value = parser.AsString(1);

    if (Index(key) != -1) {
      //log_e << "ERROR: key " << key << " was assigned more than once!!" << endl;
    }
    
    ConfItem item;
    item.Key() = key;
    item.SetValue(value);
    m_conf.push_back(item);
  }

}

void Config::Write(const string & fileName) const
{
  FILE * p = fopen(fileName.c_str(), "w");

  int i;
  for (i=0; i<m_conf.isize(); i++) {
    fprintf(p, "%s\n", m_conf[i].Get().c_str());
  }
  
  fclose(p);
}

void Config::Print(const string & fileName) const
{
  int i;
  FILE * p = fopen(fileName.c_str(), "w");
  for (i=0; i<m_conf.isize(); i++) {
    fprintf(p, "%s = %s\n", m_conf[i].Key().c_str(), m_conf[i].ValueString().c_str());
  }
}

void Config::Print(Reporting & rep) const
{
  int i;
  for (i=0; i<m_conf.isize(); i++) {
    rep.Stream() << m_conf[i].Key() << " = " << m_conf[i].ValueString() << endl;
  }
}

void Config::Print(svec<string> & out) const
{
  int i;
  for (i=0; i<m_conf.isize(); i++) {
    string s = m_conf[i].Key() + " = " + m_conf[i].ValueString();
    out.push_back(s);
  }
  
}
 
void Config::Print(svec<string> & keys, svec<string> & values) const
{
  int i;
  for (i=0; i<m_conf.isize(); i++) {
    keys.push_back(m_conf[i].Key());
    values.push_back(m_conf[i].ValueString());
  }

}



const string empty = "";

const string & Config::ValueString(const string & key) const
{
  int index = Index(key);
  if (index < 0)
    return ::empty;
  else
    return m_conf[index].ValueString();
}

int Config::ValueInt(const string & key) const
{
  int index = Index(key);
  if (index < 0)
    return -1;
  else
    return m_conf[index].ValueInt();
}

double Config::ValueDouble(const string & key) const
{
  int index = Index(key);
  if (index < 0)
    return -1;
  else
    return m_conf[index].ValueDouble();
}

/*
bool Config::ValueBool(const string & key) const
{
  int index = Index(key);
  if (index < 0)
    return false;
  else
    return m_conf[index].ValueBool();

    }*/

void Config::SetValue(const string & key, const string & value)
{
  int index = IndexPlus(key);
  m_conf[index].SetValue(value);
}

void Config::SetValue(const string & key, int value)
{
  int index = IndexPlus(key);
  m_conf[index].SetValue(value);
}

void Config::SetValue(const string & key, double value)
{
  int index = IndexPlus(key);
  m_conf[index].SetValue(value);
}

/*
void Config::SetValue(const string & key, bool value)
{
  cout << "Bool" << endl;
  int index = IndexPlus(key);
  m_conf[index].SetValue(value);
  }*/

void Config::Update(const string & key, string & value)
{
  int index = Index(key);

  if (index < 0) {
    index = IndexPlus(key);
    m_conf[index].SetValue(value);
  } else {
    value = m_conf[index].ValueString();
  }
}

void Config::Update(const string & key, int & value)
{
  int index = Index(key);
  if (index < 0) {
    index = IndexPlus(key);
    m_conf[index].SetValue(value);
  } else {
    value = m_conf[index].ValueInt();
  }
}

void Config::Update(const string & key, double & value)
{
  int index = Index(key);
  if (index < 0) {
    index = IndexPlus(key);
    m_conf[index].SetValue(value);
  } else {
    value = m_conf[index].ValueDouble();
  }
}

void Config::UpdateF(const string & key, double & value)
{
  int index = Index(key);
  if (index < 0) {
    index = IndexPlus(key);
    m_conf[index].SetValue(value);
  } else {
    value = m_conf[index].ValueDouble();
  }
}

/*
void Config::Update(const string & key, bool & value)
  
{
  int index = Index(key);
  if (index < 0) {
    index = IndexPlus(key);
    m_conf[index].SetValue(value);
  } else {
    value = m_conf[index].ValueBool();
  }
  }*/

void Config::Update(const string & key, string & value, const string & def)
{
  value = def;
  Update(key, value);
}

void Config::Update(const string & key, int & value, int def)
{
  value = def;
  Update(key, value);
}

void Config::Update(const string & key, double & value, int def)
{
  value = def;
  Update(key, value);
}
void Config::UpdateF(const string & key, double & value, int def)
{
  value = def;
  UpdateF(key, value);
}


/*
void Config::Update(const string & key, bool & value, int def)
{
  value = def;
  Update(key, value);
  }*/
