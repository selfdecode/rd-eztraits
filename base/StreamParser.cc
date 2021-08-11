
#include "base/StreamParser.h"
#include <stdlib.h>


StreamParser::StreamParser()
{
  m_count = 0;
}


StreamParser::~StreamParser()
{
}


void StreamParser::Set(stringstream & ss)
{
  //cout << "Call set." << endl;
  int i;
  int last = 0;
  string tmp = ss.str();
  m_count = 0;
  m_data.clear();
  for (i=0; i<tmp.size(); i++) {
    if (tmp[i] == '\n') {
      tmp[i] = 0;
      string n = &(tmp.c_str())[last];
      last = i+1;
      m_data.push_back(n);
    }
  }
  //cout << "Items: " << m_data.isize() << endl;
}

bool StreamParser::ParseLine()
{
  if (m_count == m_data.isize())
    return false;
  m_parser.SetLine(m_data[m_count]);
  m_count++;
  return true;
}


int StreamParser::GetItemCount()
{
  return m_parser.GetItemCount();
}

bool StreamParser::IsString(int index)
{
  return m_parser.IsString(index);
}


bool StreamParser::IsInt(int index)
{
  return m_parser.IsInt(index);
}


bool StreamParser::IsFloat(int index)
{
  return m_parser.IsFloat(index);

}

const string & StreamParser::Line()
{
  return m_data[m_count-1];
}

const string & StreamParser::AsString(int i)
{
  return m_parser.AsString(i);
}

char StreamParser::AsChar(int i)
{
  return m_parser.AsChar(i);
}

int StreamParser::AsInt(int i)
{
  return m_parser.AsChar(i);
}


double StreamParser::AsFloat(int i)
{
  return m_parser.AsChar(i);
}



