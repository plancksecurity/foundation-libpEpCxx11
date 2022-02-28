# libpEpCxx11

A collection of commonly re-usable C++ code   
Important: No dependencies allowed except c++ standard libraries   

## Resources

* Repository: https://gitea.pep.foundation/pEp.foundation/libpEpCxx11/
* Change management: <TODO: JIRA link here>


## Requirements & Platforms

* License: GPL3
* C++ standard: C++11
* supported compilers: at least gcc and clang (both have to work)

* Build sytem: GNU Make – Platforms: Linux, macOS
  ** Target: static library libpEpCxx.a
* Windows and Android builds will be done separatly


## Public API

* defined in `namespace pEp::CXX`
* header files are installed in `PREFIX/include/pEp/` and are used via `#include <pEp/filename.hh>`



## Coding Conventions

The coding conventions are expressed in the top level:
* .clang-format
* .clang-tidy

Code committed or submitted via PR will be rejected if not adhering to these coding conventions

Additionally:
* Include guards: #ifdef/#define style (not #pragma once), naming key: `LIBPEPCXX1_<FILENAME>_{HH|HXX}`

