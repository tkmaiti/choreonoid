#ifndef CNOID_ASSIMPPLUGIN_EXPORTDECL_H_INCLUDED
# define CNOID_ASSIMPPLUGIN_EXPORTDECL_H_INCLUDED

# if defined _WIN32 || defined __CYGWIN__
#  define CNOID_ASSIMPPLUGIN_DLLIMPORT __declspec(dllimport)
#  define CNOID_ASSIMPPLUGIN_DLLEXPORT __declspec(dllexport)
#  define CNOID_ASSIMPPLUGIN_DLLLOCAL
# else
#  if __GNUC__ >= 4
#   define CNOID_ASSIMPPLUGIN_DLLIMPORT __attribute__ ((visibility("default")))
#   define CNOID_ASSIMPPLUGIN_DLLEXPORT __attribute__ ((visibility("default")))
#   define CNOID_ASSIMPPLUGIN_DLLLOCAL  __attribute__ ((visibility("hidden")))
#  else
#   define CNOID_ASSIMPPLUGIN_DLLIMPORT
#   define CNOID_ASSIMPPLUGIN_DLLEXPORT
#   define CNOID_ASSIMPPLUGIN_DLLLOCAL
#  endif
# endif

# ifdef CNOID_ASSIMPPLUGIN_STATIC
#  define CNOID_ASSIMPPLUGIN_DLLAPI
#  define CNOID_ASSIMPPLUGIN_LOCAL
# else
#  if defined(CnoidAssimpPlugin_EXPORTS) || defined(CnoidAssimpSceneLoader_EXPORTS)
#   define CNOID_ASSIMPPLUGIN_DLLAPI CNOID_ASSIMPPLUGIN_DLLEXPORT
#  else
#   define CNOID_ASSIMPPLUGIN_DLLAPI CNOID_ASSIMPPLUGIN_DLLIMPORT
#  endif
#  define CNOID_ASSIMPPLUGIN_LOCAL CNOID_ASSIMPPLUGIN_DLLLOCAL
# endif

#endif

#ifdef CNOID_EXPORT
# undef CNOID_EXPORT
#endif
#define CNOID_EXPORT CNOID_ASSIMPPLUGIN_DLLAPI
