#ifndef CHIP_TOOLS_COMMON_HPP_
#define CHIP_TOOLS_COMMON_HPP_

#include <type_traits>

namespace chip_tools
{

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

} // namespace chip_tools

#endif // #ifndef CHIP_TOOLS_COMMON_HPP_
