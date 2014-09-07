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
     - GCC 4.8 (with [Boost.Regex][Boost.Regex])
     - Clang 3.3+ (with [Boost.Regex][Boost.Regex])
 - Unplanned
     - MSVC

While GCC 4.8 and Clang 3.3 are supported, you must compile them with `make USE_BOOST_REGEX=1`.
Early versions will happy compile regular expressions, but will fail at runtime with a `regex_error`, which is not very
 useful.
However, you can use [Boost.Regex][Boost.Regex] as the regular expression engine for compilers with incomplete `<regex>`
 implementations.
The downside of this is your application must link with the Boost libraries (`-lboost_regex -lboost_system`).

There is currently no plan for supporting MSVC, since nobody uses nginx on Windows.

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
