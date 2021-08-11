#ifndef RANDOMSTUFF_H_
#define RANDOMSTUFF_H_


#include <math.h>
#include "base/SVector.h"

inline long long RandomInt(long long n)
{    
  long long r =   ((long long)( rand( ) ) << 31 ) ^ (long long)( rand( ) );
  //cout << "r=" << r;
  r = r % n;
  //cout << " " << r << endl;
  return r;
}

inline double RandomFloat(double n)
{    
  long long r =   ((long long)( rand( ) ) << 31 ) ^ (long long)( rand( ) );
  //cout << "r=" << r;
  r = r % 0x7FFFFFFF;
  //cout << " " << r << endl;
  return n*((double)r)/(double)0x7FFFFFFF;
}




template<class T> 
void SRandomize(svec<T>& v)
{
  //random_shuffle(v.begin(), v.end());

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
  
}


template<class T> 
void Randomize(vector<T>& v)
{
  SRandomize(v);
  //random_shuffle(v.begin(), v.end());
}


class RandomCache
{
 public:
  RandomCache() {
    m_readcounter = 0;
    m_writecounter = 0;
  }
  
  void SetSize(int n) {
    cout << "Set size " << n << endl;
    m_cache.resize(n, 0);
    m_readcounter = 0;
    m_writecounter = 0;
  }

  long long RandomInt(long long n) {
    if (m_readcounter < m_writecounter 
	&& m_readcounter < m_cache.isize()) {      
      m_readcounter++;
      return m_cache[m_readcounter-1];      
    }
    long long m = ::RandomInt(n);
    if (m_writecounter < m_cache.isize()) {
      m_cache[m_writecounter] = m;
      m_writecounter++;
      m_readcounter++;
    }
    return m;
  }

  void FillCache() {
    m_writecounter = 0;
    m_readcounter = 0;
  }

  // Resets the cache counter
  void FromCache() {
    m_readcounter = 0;
  }

  int isize() const {return m_cache.isize();}

 private:
  svec<long long> m_cache;
  int m_readcounter;
  int m_writecounter;
};




#endif //RANDOMSTUFF_H_

