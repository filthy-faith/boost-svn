// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef FROM_PYTHON_DWA2002127_HPP
# define FROM_PYTHON_DWA2002127_HPP

# include <boost/python/converter/find_from_python.hpp>
# include <boost/python/detail/wrap_python.hpp>
# include <boost/python/detail/destroy.hpp>
# include <boost/type_traits/transform_traits.hpp>
# include <boost/python/converter/pointer_type_id.hpp>
# include <boost/python/converter/from_python_data.hpp>
# include <boost/mpl/select_type.hpp>
# include <boost/python/converter/registry.hpp>

namespace boost { namespace python { namespace converter {

struct from_python_base
{
 public: // member functions
    from_python_base(void* result);
    from_python_base(PyObject*, lvalue_from_python_registration const* chain);
    bool convertible() const;
    
 protected: // member functions
    void*const& result() const;
    
 private: // data members
    void* m_result;
};

// Used when T == U*const&
template <class T>
struct pointer_const_reference_from_python
{
    pointer_const_reference_from_python(PyObject*);
    T operator()(PyObject*) const;
    bool convertible() const;
    
 private:
    typename detail::referent_storage<T>::type m_result;
    
    static lvalue_from_python_registration*& chain;
};

// Used when T == U* 
template <class T>
struct pointer_from_python : from_python_base
{
    pointer_from_python(PyObject*);
    T operator()(PyObject*) const;
    
    static lvalue_from_python_registration*& chain;
};

// Used when T == U& and (T != V const& or T == W volatile&)
template <class T>
struct reference_from_python : from_python_base
{
    reference_from_python(PyObject*);
    T operator()(PyObject*) const;
    
    static lvalue_from_python_registration*& chain;
};

// ------- rvalue converters ---------

// Used for the case where T is a non-pointer, non-reference type OR
// is a const non-volatile reference to a non-pointer type.
template <class T>
class rvalue_from_python
{
    typedef typename boost::add_reference<
        typename boost::add_const<T>::type
    >::type result_type;
    
 public:
    rvalue_from_python(PyObject*);
    ~rvalue_from_python();
    bool convertible() const;
    
    result_type operator()(PyObject*);
    
 private:
    rvalue_data<result_type> m_data;
    static rvalue_from_python_registration*& chain;
};

template <class T>
struct select_from_python
{
    BOOST_STATIC_CONSTANT(
        bool, ptr = is_pointer<T>::value);
    
    BOOST_STATIC_CONSTANT(
        bool, ptr_cref
            = boost::python::detail::is_reference_to_pointer<T>::value
            && boost::python::detail::is_reference_to_const<T>::value
            && !boost::python::detail::is_reference_to_volatile<T>::value);
    
    
    BOOST_STATIC_CONSTANT(
        bool, ref =
        boost::python::detail::is_reference_to_non_const<T>::value
        || boost::python::detail::is_reference_to_volatile<T>::value);

    typedef typename mpl::select_type<
        ptr
        , pointer_from_python<T>
        , typename mpl::select_type<
             ptr_cref
             , pointer_const_reference_from_python<T>
             , typename mpl::select_type<
                   ref
                   , reference_from_python<T>
                   , rvalue_from_python<T>
                >::type
          >::type
    >::type type;
};

//
// implementations
//
inline from_python_base::from_python_base(void* result)
    : m_result(result)
{
}

inline from_python_base::from_python_base(
    PyObject* const source
    , lvalue_from_python_registration const* chain)
    : m_result(find(source, chain))
{
}

inline bool from_python_base::convertible() const
{
    return m_result != 0;
}

inline void*const& from_python_base::result() const
{
    return m_result;
}

// --------

namespace detail
{
  template <class U>
  inline U& void_ptr_to_reference(void const volatile* p, U&(*)())
  {
      return *(U*)p;
  }

  template <class T>
  struct null_ptr_owner
  {
      static T value;
  };
  template <class T> T null_ptr_owner<T>::value = 0;
  
  template <class U>
  inline U& null_ptr_reference(U&(*)())
  {
      return null_ptr_owner<U>::value;
  }

  template <class T>
  inline void write_void_ptr(void const volatile* storage, void* ptr, T*)
  {
      *(T**)storage = (T*)ptr;
  }

  // writes U(ptr) into the storage
  template <class U>
  inline void write_void_ptr_reference(void const volatile* storage, void* ptr, U&(*)())
  {
      write_void_ptr(storage, ptr, U(0));
  }
}

template <class T>
inline pointer_const_reference_from_python<T>::pointer_const_reference_from_python(PyObject* p)
{
    detail::write_void_ptr_reference(
        m_result.bytes
        , p == Py_None ? p : find(p, chain)
        , (T(*)())0);
}

template <class T>
inline bool pointer_const_reference_from_python<T>::convertible() const
{
    return detail::void_ptr_to_reference(m_result.bytes, (T(*)())0) != 0;
}
template <class T>
inline T pointer_const_reference_from_python<T>::operator()(PyObject* p) const
{
    return (p == Py_None)
        ? detail::null_ptr_reference((T(*)())0)
        : detail::void_ptr_to_reference(m_result.bytes, (T(*)())0);
}

template <class T>
lvalue_from_python_registration*& pointer_const_reference_from_python<T>::chain
        = registry::lvalue_converters(pointer_type_id<T>());

// --------

template <class T>
inline pointer_from_python<T>::pointer_from_python(PyObject* p)
    : from_python_base(p == Py_None ? p : find(p, chain))
{
}

template <class T>
inline T pointer_from_python<T>::operator()(PyObject* p) const
{
    return (p == Py_None) ? 0 : T(result());
}

template <class T>
lvalue_from_python_registration*& pointer_from_python<T>::chain
        = registry::lvalue_converters(pointer_type_id<T>());

// --------

template <class T>
inline reference_from_python<T>::reference_from_python(PyObject* p)
    : from_python_base(find(p,chain))
{
}

template <class T>
inline T reference_from_python<T>::operator()(PyObject*) const
{
    return detail::void_ptr_to_reference(result(), (T(*)())0);
}

template <class T>
lvalue_from_python_registration*& reference_from_python<T>::chain
        = registry::lvalue_converters(undecorated_type_id<T>());

// -------

template <class T>
inline rvalue_from_python<T>::rvalue_from_python(PyObject* obj)
{
    find(obj, chain, m_data.stage1);
}

template <class T>
inline rvalue_from_python<T>::~rvalue_from_python()
{
    if (m_data.stage1.convertible == m_data.storage.bytes)
        python::detail::destroy_reference<result_type>(m_data.storage.bytes);
}

template <class T>
inline bool rvalue_from_python<T>::convertible() const
{
    return m_data.stage1.convertible != 0;
}

template <class T>
inline typename rvalue_from_python<T>::result_type
rvalue_from_python<T>::operator()(PyObject* p)
{
    if (m_data.stage1.construct != 0)
        m_data.stage1.construct(p, &m_data.stage1);
    
    return detail::void_ptr_to_reference(m_data.stage1.convertible, (result_type(*)())0);
}

template <class T>
rvalue_from_python_registration*& rvalue_from_python<T>::chain
        = registry::rvalue_converters(undecorated_type_id<T>());

}}} // namespace boost::python::converter

#endif // FROM_PYTHON_DWA2002127_HPP
