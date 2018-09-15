
template <class T>
C_array<T>::C_array()
  : m_sz(0), m_val(0)
{
}

template <class T>
C_array<T>::C_array(T* val, int sz)
  : m_sz(0), m_val(0)
{
  set(val, sz);
}

template <class T>
C_array<T>::~C_array()
{
  delete [] m_val;
}

template <class T>
C_array<T>::C_array(const C_array<T>& oth)
  : m_sz(0), m_val(0)
{
  equate(oth);
}

template <class T>
C_array<T>&
C_array<T>::operator=(const C_array<T>& oth)
{
  if (this != &oth)
    equate(oth);
  return *this;
}

template <class T>
void
C_array<T>::equate(const C_array<T>& oth)
{
  size(oth.m_sz);
  for (int i=0; i<m_sz; ++i)
    m_val[i] = oth.m_val[i];
}

#ifndef _ODI_OSSG_
#include "IO_stream.h"

template <class T>
void C_array<T>::write(O_stream& st, const Persist_method<T>& pers) const
{
  st.write(m_sz);
  for (int i=0; i<m_sz; ++i)
    pers.write(st, m_val[i]);
}

template <class T>
int C_array<T>::read(I_stream& st, const Persist_method<T>& pers)
{
  int sz, res=1;
  if( !st.read(sz) )
    return 0;
  size(sz);
  for (int i=0; i<m_sz; ++i)
    if (!pers.read(st, m_val[i]))
      {res=0; break;}
  return res;
}
#endif // _ODI_OSSG_

template <class T>
void C_array<T>::set(T* val, int sz)
{
  size(sz);
  for (int i=0; i<sz; ++i)
    m_val[i] = val[i];
}
