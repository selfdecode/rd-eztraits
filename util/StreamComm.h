// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef _STREAMCOMM_H_
#define _STREAMCOMM_H_

//#define FORCE_DEBUG

#include "base/FileParser.h"

/* the port users will be connecting to */
#define DEFPORT 3491

class DataPacket
{
 public:
  DataPacket(int size = 2*65536);
  //DataPacket(int size = 4096);

  ~DataPacket();

  void Clear() {
    m_ptr = 0;
  }

  const char * Data() const {return &m_data[0];}
  char * Data() {return &m_data[0];}
  int size() const {return m_size;}
  int effective_size() const {return m_ptr;}
  void Set(const char * p, int size);

  void resize(int n) {
    m_data.resize(n);
  }
  void resize_add(int n) {
    m_data.resize(m_ptr + n);
  }

  void WriteToFile(const char * fileName) {
    FILE * p = fopen(fileName, "wb");
    fwrite(&m_ptr, sizeof(m_ptr), 1, p);
    fwrite(&m_data[0], m_ptr, 1, p);
    fclose(p);
  }

  void ReadFromFile(const char * fileName) {
    FILE * p = fopen(fileName, "rb");
    fread(&m_ptr, sizeof(m_ptr), 1, p);    
    m_data.resize(m_ptr);
    fread(&m_data[0], m_ptr, 1, p);
    fclose(p);
  }
  
  bool Read(string & s);
  bool Read(float & v);
  bool Read(double & v);
  bool Read(int & v);
  bool Read(char & v);
  bool ReadBool(bool & v);

  bool Write(const string & s);
  bool Write(float v);
  bool Write(double v);
  bool Write(int v);
  bool Write(char v);
  bool WriteBool(bool v);

  const string & GetIPAddress() const {return m_address;}
  void SetIPAddress(const string & s) {m_address = s;}
  int IP() const {return m_IP;}
  void SetIP(int ip) {
    m_IP = ip;
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    char tmp[64];
    sprintf(tmp, "%d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);
    m_address = tmp;
  }

 private:
 
  svec<char> m_data;
  int m_size;
  int m_ptr;  
  int m_IP;
  string m_address;
};




class StreamCommTransmitter
{
 public:
  virtual ~StreamCommTransmitter() {}

  // Returns immediately, no feedback if successful
  virtual void Send(const DataPacket & data) = 0;
  
};


class StreamCommReceiver
{
 public:
  virtual ~StreamCommReceiver() {}

  // Retrieves the packets in order in which they were received
  virtual bool Get(DataPacket & data) = 0;

 
  virtual bool PeekLast(DataPacket & data) = 0;


  // Clears out the queue
  virtual void ClearQueue() = 0;
};

// This is how to get an actual instance
StreamCommTransmitter * GetTransmitter(const string & serverName, int port = DEFPORT);
StreamCommReceiver * GetReceiver(int port = DEFPORT);

StreamCommTransmitter * GetTransmitter(int port = DEFPORT);
StreamCommReceiver * GetReceiver(const string & serverName, int port = DEFPORT);

bool StreamGetHostName(string & host);


template <class T>
class TRingBuffer
{
 public:
  TRingBuffer(int n = 1024) {
    m_data.resize(n);
    m_read = 0;
    m_write = 0;
  }

  void SetSize(int n) {
    m_data.resize(n);
    m_read = 0;
    m_write = 0;
  }

   bool PeekLast(T & t) {
    //cout << "Pop " << m_read << " " <<  m_write << endl;
    if (m_read == m_write)
      return false;
    int n = m_write-1;
    if (n < 0)
      n = m_data.isize() - 1;
    t = m_data[n];
    return true;
  }

   bool Pop(T & t) {
    //cout << "Pop " << m_read << " " <<  m_write << endl;
    if (m_read == m_write)
      return false;
    t = m_data[m_read];
    m_read++;
    if (m_read == m_data.isize())
      m_read = 0;
    return true;
  }

  bool Push(const T & t) {
    //cout << "Pushing " << m_write << " " << m_read << endl;
    m_data[m_write] = t;
    m_write++;
    if (m_write == m_data.isize())
      m_write = 0;
    if (m_write == m_read) {
      m_read++;
      if (m_read == m_data.isize())
	m_read = 0;
    }
    return true;
  }
 
  void Clear() {
    m_read = m_write = 0;
  }

 private:
  svec<T> m_data;
  int m_read;
  int m_write;
};



class IPCache
{
 public:
  IPCache() {}

  const string GetIP(const string & host); 

 private:
  svec<string> m_ip;
  svec<string> m_host;
};


#endif 


