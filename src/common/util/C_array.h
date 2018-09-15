
#ifndef C_ARRAY_H
#define C_ARRAY_H

#ifndef E_ASSERT_H
#include "e_assert.h"
#endif

#ifndef PERSIST_METHOD_H
#include "Persist_method.h"
#endif

template <class T> class C_array
{
public:
  typedef T value_type;

  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;

  typedef pointer iterator;
  typedef const_pointer const_iterator;

  iterator begin() { return m_val; }
  iterator end() { return m_val + m_sz; }

  C_array();
  C_array(T* val, int sz); 
  ~C_array();

  C_array(const C_array<T>&);
  C_array<T>& operator=(const C_array<T>&);

  void set(T* val, int sz);

  reference operator [] (size_type ndx) {
    b_assert(ndx>=0); 
    b_assert(ndx<m_sz);
    return m_val[ndx];
  }

  const_reference operator [] (size_type ndx) const {
    b_assert(ndx>=0); 
    b_assert(ndx<m_sz);
    return m_val[ndx];
  }

  void size(size_type sz) { 
    if (m_sz<sz) 
      {
	// re-allocate if necessary
	delete [] m_val;
	m_val = new T[sz];
      }
    m_sz = sz; 
  }

  size_type size() const { return m_sz; }

  void write(O_stream& st, const Persist_method<T>& pers
	     =Persist_method<T>()) const;
  int read(I_stream& st, const Persist_method<T>& pers
	    =Persist_method<T>());

private:
  void equate(const C_array<T>& oth);
#ifndef _ODI_OSSG_
  friend class C_array_persist<T>;
#endif
  int m_sz;
  T* m_val;
};

#include "C_array.cpp"

typedef C_array<int> C_ARRAY_INT;
typedef C_array<double> C_ARRAY_DBL;

#endif // C_ARRAY_H
