#ifndef CACHED_VEC
#define CACHED_VEC

#include "base/SVector.h"

class VecIntBase
{
 public:
  VecIntBase() {
    m_modCount = 0;
    m_reset = 0;
    m_size = 0;
  }

  const int & operator[] (int i) const {return m_data[i];}
  int isize() const {return m_size;}
  void resize(int n, int v = 0) {
    if (n > m_data.isize()) {
      //cout << "Stupid 1" << " n=" << n << " isize=" << m_data.isize() << endl;
      m_data.resize(n, v);
      m_mod.resize(n);
    }
    m_size = n;
    if (v != m_reset) {
      //cout << "Stupid 2" << " v=" << v << " m_reset=" << m_reset << endl;
      for (int i=0; i<m_size; i++)
	m_data[i] = v;
    }
    m_reset = v;
  }

  void hard_clear() {
    m_modCount = 0;
    m_reset = 0;
    m_size = 0;
    m_data.clear();
    m_mod.clear();
  }


  void clear() {
    int i;
    //cout << m_modCount << " " <<  m_mod.isize() << endl;
    if (m_modCount < m_mod.isize()) {
      for (i=0; i<m_modCount; i++)
     	m_data[m_mod[i]] = m_reset;
    } else {
      for (i=0; i<m_data.isize(); i++)
	m_data[i] = m_reset;
    }
    m_modCount = 0;
  }


 protected:
  svec<int> m_data;
  svec<int> m_mod;
  int m_modCount;
  int m_reset;
  int m_size;
};

//================================================
class VecInt : public VecIntBase
{
 public:
 VecInt() : VecIntBase() {
  }
  void Set(int i, int v) {
    //if (m_data[i] == 0 && v != m_reset) {
    // Let's not be too smart here...
    if (m_modCount < m_mod.isize()) {
      m_mod[m_modCount] = i;
      m_modCount++;
    }
      //}
    m_data[i] = v;
  }
};




//===============================================
class VecIntInc : public VecIntBase
{
 public:
  VecIntInc() : VecIntBase() {
    m_totalSet = 0;
  }


  void Set(int i, int v) {
    if (m_data[i] == m_reset && v != m_reset) {
      m_mod[m_modCount] = i;
      m_modCount++;
      m_totalSet++;
    }
    m_data[i] = v;
  }
  void Reset() {
    m_modCount = 0;
  }
  int NumTotal() const {return m_totalSet;}
  int NumMod() const {return m_modCount;}
  int GetMod(int i) {return m_mod[i];}
  int From() const {return m_totalSet - m_modCount;}

 private:
  int m_totalSet;
};



template <class T> 
class svec_buff
{
 public:
  svec_buff() {
    m_size = 0;
  }

  int isize() const {return m_size;}
  long long lsize() const {return m_size;}

  void reserve(int n) {
    if (n > m_data.isize())
      m_data.resize(n);
  }

  void resize(int n) {
    if (n > m_data.isize()) {
      m_data.resize(n);
    }
    m_size = n;
  }

  void clear() {m_size = 0;}

  void push_back(const T & t) {
    if (m_data.isize() >= m_size) {
      m_data.resize(m_size + 65536);
    }
    m_data[m_size] = t;
    m_size++;
  }

  const T & operator[] (long long i) const {
    return m_data[i];
  }

  T & operator[] (long long i) {
    return m_data[i];
  }

  T & begin() {return m_data[0];}
  T & end() {return m_data[m_size-1];}

  void Sort() {
    if (m_size == 0)
      return;
    //sort(m_data.begin(), m_data.at(m_size-1));
    sort(m_data.begin(), m_data.begin()+m_size);
  }

  int BinSearch(const T & item) {
   if (m_size == 0)
      return -1;
   typename svec<T>::iterator iter;    
   typename svec<T>::iterator begin = m_data.begin();
   typename svec<T>::iterator end = m_data.begin()+m_size;
   iter = lower_bound(begin, end, item);
   long long pos = distance(begin, iter);
   
   if (pos < 0)
     return -1;
   
   if (pos == m_size)
     return -1;
   
   if (m_data[pos] < item || item < m_data[pos])
     return -1;
   
   return pos;
 }

 private:
  svec<T> m_data;
  int m_size;
};

/*
template<class T> 
void Sort(svec_buff<T>& v)
{
  sort(v.begin(), v.end());
  }*/



#endif //CACHED_VEC

