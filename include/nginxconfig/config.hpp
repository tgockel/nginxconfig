/** \file nginxconfig/config.hpp
 *  
 *  Copyright (c) 2014 by Travis Gockel. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of the Apache License
 *  as published by the Apache Software Foundation, either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  \author Travis Gockel (travis@gockelhut.com)
**/
#ifndef __NGINXCONFIG_CONFIG_HPP_INCLUDED__
#define __NGINXCONFIG_CONFIG_HPP_INCLUDED__

/** \def NGINXCONFIG_USER_CONFIG
 *  \brief A user-defined configuration file to be included before all other nginxconfig content.
**/
#ifdef NGINXCONFIG_USER_CONFIG
#   include NGINXCONFIG_USER_CONFIG
#endif

/** \def NGINXCONFIG_SO
 *  \brief Are you using shared objects (DLLs in Windows)?
**/
#ifndef NGINXCONFIG_SO
#   define NGINXCONFIG_SO 1
#endif

/** \def NGINXCONFIG_COMPILING
 *  \brief Is nginxconfig currently compiling?
 *  You probably do not want to set this by hand. It is set by the Makefile when the library is compiled.
**/
#ifndef NGINXCONFIG_COMPILING
#   define NGINXCONFIG_COMPILING 0
#endif

/** \def NGINXCONFIG_EXPORT
 *  If using shared objects, this class or function should be exported.
 *  
 *  \def NGINXCONFIG_IMPORT
 *  If using shared objects, this class or function should be imported.
 *  
 *  \def NGINXCONFIG_HIDDEN
 *  This symbol is only visible within the same shared object in which the translation unit will end up. Symbols which
 *  are "hidden" will \e not be put into the global offset table, which means code can be more optimal when it involves
 *  hidden symbols at the cost that nothing outside of the SO can access it.
**/
#if NGINXCONFIG_SO
#   if defined(__GNUC__)
#       define NGINXCONFIG_EXPORT          __attribute__((visibility("default")))
#       define NGINXCONFIG_IMPORT
#       define NGINXCONFIG_HIDDEN          __attribute__((visibility("hidden")))
#   elif defined(__MSC_VER)
#       define NGINXCONFIG_EXPORT          __declspec(dllexport)
#       define NGINXCONFIG_IMPORT          __declspec(dllimport)
#       define NGINXCONFIG_HIDDEN
#   else
#       error "Unknown shared object semantics for this compiler."
#   endif
#else
#   define NGINXCONFIG_EXPORT
#   define NGINXCONFIG_IMPORT
#   define NGINXCONFIG_HIDDEN
#endif

/** \def NGINXCONFIG_PUBLIC
 *  \brief This function or class is part of the public API for nginxconfig.
 *  If you are including nginxconfig for another library, this will have import semantics (\c NGINXCONFIG_IMPORT); if
 *  you are building nginxconfig, this will have export semantics (\c NGINXCONFIG_EXPORT).
 *  
 *  \def NGINXCONFIG_LOCAL
 *  \brief This function or class is internal-use only.
 *  \see NGINXCONFIG_HIDDEN
**/
#if NGINXCONFIG_COMPILING
#   define NGINXCONFIG_PUBLIC NGINXCONFIG_EXPORT
#   define NGINXCONFIG_LOCAL  NGINXCONFIG_HIDDEN
#else
#   define NGINXCONFIG_PUBLIC NGINXCONFIG_IMPORT
#   define NGINXCONFIG_LOCAL  NGINXCONFIG_HIDDEN
#endif

/** \def NGINXCONFIG_NO_RETURN
 *  \brief Mark that a given function will never return control to the caller, either by exiting or throwing an
 *  exception.
**/
#ifndef NGINXCONFIG_NO_RETURN
#   if defined(__GNUC__)
#       define NGINXCONFIG_NO_RETURN __attribute__((noreturn))
#   else
#       define NGINXCONFIG_NO_RETURN
#   endif
#endif

#endif/*__NGINXCONFIG_CONFIG_HPP_INCLUDED__*/
