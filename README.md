Introduction
============

A simple C++ library for dealing with [nginx](http://nginx.com/) server configuration files.
Since your `nginx.conf` file is not written using a standard-issue file format (JSON, XML, YAML), editing it in software
 can be a little obnoxious.
This library is meant to help you parse and output an `nginx.conf` file in C++.

[![Build Status](https://travis-ci.org/tgockel/nginxconfig.svg?branch=master)](https://travis-ci.org/tgockel/nginxconfig)

Compiler Support
----------------

 - Supported
     - GCC 4.9+
 - Potential
     - GCC 4.8
     - Clang 3.4+
 - Unplanned
     - MSVC

Currently, the only compiler supported is GCC 4.9.
While you can *compile* with GCC 4.8, the `std::regex` implementation always throws when used, so it is not very useful.
In the future, I might add support for building against the [Boost.Regex][Boost.Regex] library instead of the C++
 Standard Library's version.
Support for Clang would certainly not be frowned upon, but I am not planning on doing so, as I only need to control
 nginx from an application compiled with GCC.
Support for MSVC probably would be frowned upon, since nobody uses nginx on Windows.

Future
======

This library is meant to be extremely simple, so anything more than manipulation of the AST, parsing and encoding will
 probably not be added by me.
If you are a developer that wishes to extend this library with helper functions, I would not be upset with your
 contributions.

License
=======

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at

  [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

 [Boost.Regex]: http://www.boost.org/doc/libs/1_56_0/libs/regex/doc/html/index.html
