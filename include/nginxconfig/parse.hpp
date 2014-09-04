/** \file nginxconfig/parse.hpp
 *  Include this file if you're trying to parse an nginx configuration file.
 *  
 *  Copyright (c) 2014 by Travis Gockel. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of the Apache License
 *  as published by the Apache Software Foundation, either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  \author Travis Gockel (travis@gockelhut.com)
**/
#ifndef __NGINXCONFIG_PARSE_HPP_INCLUDED__
#define __NGINXCONFIG_PARSE_HPP_INCLUDED__

#include <nginxconfig/config.hpp>

#include <iosfwd>
#include <string>
#include <stdexcept>

namespace nginxconfig
{

class ast_entry;

class NGINXCONFIG_PUBLIC parse_error :
        public std::runtime_error
{
public:
    using size_type = std::size_t;
    
    /** In some cases, problems do not occur at a specific column (such as EOF). In these cases, the value of \c column
     *  will be set to \c no_column.
    **/
    static constexpr size_type no_column = ~0;
    
public:
    explicit parse_error(size_type line, size_type column, size_type character, std::string message);
    
    virtual ~parse_error() noexcept;
    
    /** The line of input this error was encountered on. **/
    size_type line() const { return _line; }
    
    /** The character index on the current line this error was encountered on. **/
    size_type column() const { return _column; }
    
    /** The character index into the entire input this error was encountered on. **/
    size_type character() const { return _character; }
    
    /** A message from the parser which has user-readable details about the encountered problem. **/
    const std::string& message() const { return _message; }
    
private:
    size_type   _line;
    size_type   _column;
    size_type   _character;
    std::string _message;
};

/** Parse the given input. The root entry will always be have \c ast_entry_kind::document. **/
ast_entry parse(std::istream& input);

/** Convenience function to parse a given file. **/
ast_entry parse_file(const std::string& filename);

}

#endif/*__NGINXCONFIG_PARSE_HPP_INCLUDED__*/
