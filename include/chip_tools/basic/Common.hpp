#ifndef CHIP_TOOLS_COMMON_HPP_
#define CHIP_TOOLS_COMMON_HPP_

/*! \file Common.hpp
 *  \brief Functions and class that are widely used throughout the library
 */

#include <type_traits>

/*! \defgroup Common
 *  \brief Common definitions
 *  \addtogroup Common
 *  @{
 */

/*! \brief Common namespace for the ChipTools lib
 */
namespace chip_tools
{

/*! \addtogroup Common
 *  @{
 */

/*! \brief Checks if the type T has the method named func
 *  \param[in] func mathod name
 *  \param[in] rettype type of the method return value
 *  \param[in] ... method args (if any)
 * 
 *  Usage:
 *  \code ...
 *  class A
 *  {
 *  public:
 *     int foo();
 *  };
 * 
 *  HAS_MEM_FUNC(foo, void);
 *  bool has = has_foo<A>::value; // true
 *  \endcode
 */
#define HAS_MEM_FUNC(func, rettype, ...)\
template<typename T> \
struct has_##func \
{ \
   typedef rettype (T::*Method)(__VA_ARGS__); \
\
   template<typename U, U> \
   struct checker; \
\
   template<typename V> \
   static std::true_type check(checker<Method, &V::func> *); \
\
   template<typename> \
   static std::false_type check(...); \
\
   static bool const value = \
      std::is_same<decltype(check<T>(nullptr)), std::true_type>::value;\
}

/*! \brief if the type T has the method named func, its call will be performed, otherwise it will return return_type().
 *  \param[in] func mathod name
 *  \param[in] return_type type of the method return value
 *
 *  Usage:
 *  \code ...
 *  class A
 *  {
 *  public:
 *     int foo();
 *  };
 * 
 *  CALL_IFDEF1(foo, int);
 * 
 *  void test()
 *  {
 *     A a;
 *     int res = try_foo(&a);
 *  }
 *  \endcode
 */

#define CALL_IFDEF(func, return_type) \
class try_##func \
{ \
   HAS_MEM_FUNC(func, return_type); \
public: \
   template<typename U> \
   static return_type call(typename std::enable_if<has_##func<U>::value, U>::type *var) \
   { return var->func(); } \
   \
   template<typename> \
   static return_type call(...) \
   { return return_type(); } \
}

/*! \brief Similar to CALL_IFDEF, but used for unary methods
 */
#define CALL_IFDEF1(func, return_type, argtype1) \
class try_##func \
{ \
   HAS_MEM_FUNC(func, return_type, argtype1); \
public: \
   template<typename U> \
   static return_type call(typename std::enable_if<has_##func<U>::value, U>::type *var, argtype1 a1) \
   { return var->func(a1); } \
   \
   template<typename> \
   static return_type call(...) \
   { return return_type(); } \
}

/*! \brief Similar to CALL_IFDEF1, but used for binary methods
 */
#define CALL_IFDEF2(func, return_type, argtype1, argtype2) \
class try_##func \
{ \
   HAS_MEM_FUNC(func, return_type, argtype1, argtype2); \
public: \
   template<typename U> \
   static return_type call(typename std::enable_if<has_##func<U>::value, U>::type *var, argtype1 a1, argtype2 a2) \
   { return var->func(a1, a2); } \
   \
   template<typename> \
   static return_type call(...) \
   { return return_type(); } \
}

/** @} */

} // namespace chip_tools

/** @} */

#endif // #ifndef CHIP_TOOLS_COMMON_HPP_
