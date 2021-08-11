#ifndef MULTVECTOR_H
#define MULTVECTOR_H



#include "base/SVector.h"


template <class T> 
class multvec : public vector<T> 
{
 public:
  multvec() {
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_w = 0;
  }

  void init(const T & d) {
    for (int i=0; i<m_data.isize(); i++)
      m_data[i] = d;
  }

  //================================================

  void resize(int x, int y = 1, int z = 1, int w = 1) {
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
    m_data.resize(x*y*z*w);
  }
 
  //-------------------------------------------
  int isize() const {return m_data.size();}
  long long lsize() const {return m_data.isize();}
  int isize_x() const {return m_x;}
  int isize_y() const {return m_y;}
  int isize_z() const {return m_z;}
  int isize_w() const {return m_w;}

  //============================================
  const T & operator[] (long long i) const {
    return m_data[i];
  }

  T & operator[] (long long i) {
    return m_data[i];
  }

 //============================================
  const T & operator() (int x, int y = 0, int z = 0, int w = 0) const {
    return m_data[index(x, y, z, w)];
  }
  T & operator() (int x, int y = 0, int z = 0, int w = 0) {
    return m_data[index(x, y, z, w)];
  }

  int index(int x, int y = 0, int z = 0, int w = 0) const {
    int i = w * m_x * m_y * m_z + z * m_x * m_y + y * m_x + x;
    return i;
  }

  bool valid(int x, int y = 0, int z = 0, int w = 0) const {
    if (x < 0 || y < 0 || z < 0 || w < 0)
      return false;
    if (x >= m_x || y >= m_y || z >= m_z || w >= m_z)
      return false;
    return true;
  }

 private:
  void Coords(int & x, int & y, int & z, int & w, int index) {
    throw;
  }


  svec<T> m_data;
  int m_x;
  int m_y;
  int m_z;
  int m_w;
};







#endif

