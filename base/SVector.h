#ifndef SVECTOR_H_
#define SVECTOR_H_

using namespace std;

#include <vector>
#include <iterator>
#include <assert.h>
#include <algorithm>
#include "base/ErrorHandling.h"
//#include "base/RandomStuff.h"

#define FORCE_DEBUG

template <class T> 
class svec : public vector<T> 
{
 public:

  int isize() const {return (int)vector<T>::size();}
  long long lsize() const {return vector<T>::size();}

#ifdef FORCE_DEBUG
  const T & operator[] (long long i) const {
    if (i>= lsize()) {
      cout << "ERROR: i=" << i << " lsize=" << lsize() << endl;
      ThrowError("i>=lsize()", "svec");
    }
    if (i<0) {
      cout << "ERROR: i=" << i << " lsize=" << lsize() << endl;
      ThrowError("i<0", "svec");
    }
    //assert(i<lsize());
    //assert(i>=0);
    return vector<T>::operator[](i);
  }

  T & operator[] (long long i) {

    if (i>= lsize()) {
      cout << "ERROR: i=" << i << " lsize=" << lsize() << endl;
      ThrowError("i>=lsize()", "svec");
    }
    if (i<0) {
      cout << "ERROR: i=" << i << " lsize=" << lsize() << endl;
      ThrowError("i<0", "svec");
    }
    //assert(i<lsize());
    //assert(i>=0);
    return vector<T>::operator[](i);
  }
#endif


};



template <class T> 
class svec_a
{
   public:

  int isize() const {
    if (m_data.isize() == 0)
      return 0;
    return m_data.isize() * m_data[0].isize();
  }

  long long lsize() const {
    if (m_data.lsize() == 0)
      return 0;
    return m_data.lsize() * m_data[0].lsize();
  }

  void push_back(const T & t) {
    m_data.push_back(t);
  }

  const double & operator[] (long long i) const {
    long long a = (i % m_data[0].isize());
    long long b = i - a;
    return (m_data[a])[b];
  }
  
  double & operator[] (long long i) {
    long long a = (i % m_data[0].isize());
    long long b = i - a;
    return (m_data[a])[b];
  }

 protected:
  svec<T> m_data;
};


template <class T> 
class vec : public svec<T> 
{
};

template<class T> 
void Sort(svec<T>& v)
{
  sort(v.begin(), v.end());
}


template<class T> 
void UniqueSort(svec<T>& v)
{
  sort(v.begin(), v.end());

  long long i;
  long long k = 0;
  for (i=0; i<v.lsize(); i++) {
    v[k] = v[i];
    while (i+1<v.lsize() &&  !(v[k] < v[i+1]))
      i++;
  
    k++;    
  }
  v.resize(k);
}

template<class T> 
long long BinSearch(svec<T> & v, const T & item) 
{
  typename svec<T>::iterator iter;    
  typename svec<T>::iterator begin = v.begin();
  typename svec<T>::iterator end = v.end();
  iter = lower_bound(begin, end, item);
  long long pos = distance(begin, iter);
  
  if (pos < 0)
    return -1;

  if (pos == v.isize())
    return -1;
  
  if (v[pos] < item || item < v[pos])
    return -1;

  return pos;

}

template<class T> 
long long BinSearchFuzzy(svec<T> & v, const T & item) 
{
  typename svec<T>::iterator iter;    
  typename svec<T>::iterator begin = v.begin();
  typename svec<T>::iterator end = v.end();
  iter = lower_bound(begin, end, item);
  long pos = distance(begin, iter);
  
  if (pos < 0)
    return -1;

  return pos;

}

 
template<class H, class T>
class SPair
{
 public:
  bool operator < (const SPair & s) const {
    if (m_h < s.m_h || m_h > s.m_h)
      return m_h < s.m_h;
    return m_t < s.m_t;
  }
  T & GetT() {return m_t;}
  H & GetH() {return m_h;}
  const T & GetT() const {return m_t;}
  const H & GetH() const {return m_h;}
  
 protected:
  T m_t;
  H m_h;
};

/*
template<class T> 
void Randomize(svec<T> & v)
{
  int i;
  svec < SPair <double, T> > arr;
  arr.resize(v.isize());
  
  for (i=0; i<v.isize(); i++) {
    arr[i].GetT() = v[i];
    arr[i].GetH() = RandomFloat(1.);
  }
  Sort(arr);
  for (i=0; i<v.isize(); i++) {
    v[i] = arr[i].GetT();
  }  
  }*/




#endif //SVECTOR_H_


