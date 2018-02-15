// ---------------------------------------------------------------------
// pion:  a Boost C++ framework for building lightweight HTTP interfaces
// ---------------------------------------------------------------------
// Copyright (C) 2007-2014 Splunk Inc.  (https://github.com/splunk/pion)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//

#ifndef __PION_PIONCONFIG_HEADER__
#define __PION_PIONCONFIG_HEADER__

// DO NOT USE autoheader ; this file is not automanaged!!!

// Other libraries should be added here as they become part of the configuration 
// used for binary releases.
#ifdef PION_FULL
#define PION_USE_LOG4CPLUS
#define PION_HAVE_SSL
#define PION_HAVE_JSON
#define PION_HAVE_PYTHON
#endif

/* Define to the version number of pion. */
#define PION_VERSION "5.0.7"

/* Define to the directory where Pion plug-ins are installed. */
//#undef PION_PLUGINS_DIRECTORY
#define PION_PLUGINS_DIRECTORY "."

/* Define to the directory where cygwin is installed. */
#undef PION_CYGWIN_DIRECTORY

/* Define to 1 if C library supports malloc_trim() */
#undef PION_HAVE_MALLOC_TRIM

// -----------------------------------------------------------------------
// hash_map support
//
// At least one of the following options should be defined.

/* Define to 1 if you have the <ext/hash_map> header file. */
#undef PION_HAVE_EXT_HASH_MAP

/* Define to 1 if you have the <hash_map> header file. */
//#undef PION_HAVE_HASH_MAP
#define PION_HAVE_HASH_MAP 1

/* Define to 1 if you have the <tr1/unordered_map> header file. */
#undef PION_HAVE_TR1_UNORDERED_MAP

/* Define to 1 if you have the <unordered_map> header file. */
#undef PION_HAVE_UNORDERED_MAP
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#define PION_HAVE_UNORDERED_MAP 1
#endif

// -----------------------------------------------------------------------
// Logging Options
//
// At most one of the logging options below should be defined.  If none of 
// them are defined, std::cout and std::cerr will be used for logging.

/* To use the `log4cplus' library for logging, include PION_USE_LOG4CPLUS or PION_FULL
   in Preprocessor Definitions, or uncomment the following line. */
//#define PION_USE_LOG4CPLUS

/* To use the `log4cxx' library for logging, include PION_USE_LOG4CXX
   in Preprocessor Definitions, or uncomment the following line. */
//#define PION_USE_LOG4CXX

/* To use the `log4cpp' library for logging, include PION_USE_LOG4CPP
   in Preprocessor Definitions, or uncomment the following line. */
//#define PION_USE_LOG4CPP

/* To disable logging, include PION_DISABLE_LOGGING in Preprocessor Definitions, 
   or uncomment the following line. */
#define PION_DISABLE_LOGGING

// -----------------------------------------------------------------------

/* Define to 1 if you have the `zlib' library. */
//#undef PION_HAVE_ZLIB
#define PION_HAVE_ZLIB

/* Define to 1 if you have the `bzlib' library. */
#undef PION_HAVE_BZLIB

/* If you have the `yajl' library installed, include PION_HAVE_JSON or PION_FULL
   in Preprocessor Definitions, or uncomment the following line, to use yajl. */
//#define PION_HAVE_JSON

/* If you have the `python' library installed, include PION_HAVE_PYTHON or PION_FULL
   in Preprocessor Definitions, or uncomment the following line, to use python. */
//#define PION_HAVE_PYTHON


#if defined(_WIN32) || defined(_WINDOWS)
    #define PION_WIN32  1
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT    0x0501
    #endif
#else
    #error PionConfig.hpp.win is for Win32 only.
#endif // _WIN32

// Использование новой версии "boost::filesystem"
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED

// Использование новой версии "boost::thread"
#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_USES_DATETIME
#define BOOST_THREAD_DONT_PROVIDE_DEPRECATED_FEATURES_SINCE_V3_0_0
#define BOOST_THREAD_PROVIDES_NESTED_LOCKS
#define BOOST_THREAD_PROVIDES_CONDITION

// Отключение возможности перевода блокировки из shared в unique/upgrade.
#define BOOST_THREAD_DONT_PROVIDE_SHARED_MUTEX_UPWARDS_CONVERSIONS
// Использование специальной версии shared_mutex для windows
#define BOOST_THREAD_DONT_PROVIDE_GENERIC_SHARED_MUTEX_ON_WIN

// Использование новой версии "boost::chrono"
#define BOOST_CHRONO_VERSION 2

// Лексические преобразования в соответствии с локалью
#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE

#include <boost/config.hpp>

#define PION_STATIC_LINKING

#ifdef _MSC_VER

    #ifdef PION_EXPORTS
        #define PION_API __declspec(dllexport)
    #elif defined PION_STATIC_LINKING
        #define PION_API
    #else
        #define PION_API __declspec(dllimport)
    #endif

    #ifdef PION_STATIC_LINKING
        #define PION_PLUGIN 
    #else
        #define PION_PLUGIN __declspec(dllexport)
    #endif

    /*
    Verify correctness of the PION_STATIC_LINKING setup
    */
    #ifdef PION_STATIC_LINKING
        #ifdef _USRDLL
            #error Need to be compiled as a static library for PION_STATIC_LINKING
        #endif
    #endif

#endif // _MSC_VER

#endif //__PION_PIONCONFIG_HEADER__
