/** \file
 *  Types used for the parsing code, intended to help with unit testing.
 *  
 *  Copyright (c) 2014 by Travis Gockel. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of the Apache License
 *  as published by the Apache Software Foundation, either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  \author Travis Gockel (travis@gockelhut.com)
**/
#ifndef __NGINXCONFIG_PARSE_TYPES_HPP_INCLUDED__
#define __NGINXCONFIG_PARSE_TYPES_HPP_INCLUDED__

#include <nginxconfig/config.hpp>
#include <nginxconfig/parse.hpp>

#include <sstream>

namespace nginxconfig
{
namespace parser
{

struct context
{
public:
    using size_type = std::string::size_type;
    
    std::istream& input;
    size_type     line_no           = 0;
    size_type     character_no      = 0;
    size_type     character_no_next = 0;
    std::string   current;
    
    explicit context(std::istream& input) :
            input(input)
    { }
    
    bool next();
    
    template <typename... T>
    parse_error create_parse_error(size_type column, T&&... message)
    {
        std::ostringstream stream;
        return create_parse_error_impl(stream, column, std::forward<T>(message)...);
    }
    
private:
    parse_error create_parse_error_impl(std::ostringstream& stream, size_type column)
    {
        size_type real_char_no = character_no + (column == nginxconfig::parse_error::no_column ? 0 : column);
        return parse_error(line_no, column, real_char_no, stream.str());
    }
    
    template <typename T, typename... TRest>
    parse_error create_parse_error_impl(std::ostringstream& stream, size_type column, T&& current, TRest&&... rest)
    {
        stream << std::forward<T>(current);
        return create_parse_error_impl(stream, column, std::forward<TRest>(rest)...);
    }
};

enum class line_kind
{
    simple,
    complex_start,
    complex_end,
    comment,
    unknown,
};

struct line_components
{
    using attribute_list = ast_entry::attribute_list;
    
    static line_components create_from_line(const std::string& line);
    
    line_kind      category = line_kind::unknown;
    std::string    name;
    attribute_list attributes;
    std::string    comment;
};

}
}

#endif/*__NGINXCONFIG_PARSE_TYPES_HPP_INCLUDED__*/
