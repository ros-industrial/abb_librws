#ifndef ABB_LIBRWS__VISIBILITY_CONTROL_H_
#define ABB_LIBRWS__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ABB_LIBRWS_EXPORT __attribute__ ((dllexport))
    #define ABB_LIBRWS_IMPORT __attribute__ ((dllimport))
  #else
    #define ABB_LIBRWS_EXPORT __declspec(dllexport)
    #define ABB_LIBRWS_IMPORT __declspec(dllimport)
  #endif
  #ifdef ABB_LIBRWS_BUILDING_LIBRARY
    #define ABB_LIBRWS_PUBLIC ABB_LIBRWS_EXPORT
  #else
    #define ABB_LIBRWS_PUBLIC ABB_LIBRWS_IMPORT
  #endif
  #define ABB_LIBRWS_PUBLIC_TYPE ABB_LIBRWS_PUBLIC
  #define ABB_LIBRWS_LOCAL
#else
  #define ABB_LIBRWS_EXPORT __attribute__ ((visibility("default")))
  #define ABB_LIBRWS_IMPORT
  #if __GNUC__ >= 4
    #define ABB_LIBRWS_PUBLIC __attribute__ ((visibility("default")))
    #define ABB_LIBRWS_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define ABB_LIBRWS_PUBLIC
    #define ABB_LIBRWS_LOCAL
  #endif
  #define ABB_LIBRWS_PUBLIC_TYPE
#endif

#endif  // ABB_LIBRWS__VISIBILITY_CONTROL_H_
