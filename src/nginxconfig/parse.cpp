/** \file
 *  
 *  Copyright (c) 2014 by Travis Gockel. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of the Apache License
 *  as published by the Apache Software Foundation, either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  \author Travis Gockel (travis@gockelhut.com)
**/
#include <nginxconfig/config.hpp>

#ifndef NGINXCONFIG_DEBUG
#   define NGINXCONFIG_DEBUG 0
#endif

/** \def NGINXCONFIG_USE_BOOST_REGEX
 *  Should the parser use the regex implementation from Boost instead of the C++ Standard Library? GCC versions below
 *  4.8 will happy compile regular expressions, but will fail at runtime, so this must be set if you are using GCC under
 *  4.9!
**/
#ifndef NGINXCONFIG_USE_BOOST_REGEX
#   define NGINXCONFIG_USE_BOOST_REGEX 0
#endif

#include <nginxconfig/ast.hpp>
#include <nginxconfig/parse.hpp>
#include <nginxconfig/parse_types.hpp>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <sstream>

#if NGINXCONFIG_DEBUG
#   include <iostream>
#   define NGINXCONFIG_DEBUG_PRINT(x) std::cerr << x << std::endl;
#else
#   define NGINXCONFIG_DEBUG_PRINT(x)
#endif

#if NGINXCONFIG_USE_BOOST_REGEX
#   include <boost/regex.hpp>
#else
#   ifdef __GNUC__
#       if (__GNUC__ == 4) && (__GNUC_MINOR__ < 9)
#           error "Cannot use Standard Library regex implementation with GCC < 4.9! Please compile with NGINXCONFIG_USE_BOOST_REGEX=1."
#       endif
#   endif
#   include <regex>
#endif

namespace nginxconfig
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// parse_error                                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static std::string parse_error_what(parse_error::size_type line,
                                    parse_error::size_type column,
                                    parse_error::size_type character,
                                    const std::string&     message
                                   )
{
    std::ostringstream stream;
    stream << "On line " << line;
    if (column != parse_error::no_column)
        stream << " column " << column;
    
    stream << " (char " << character << "): "
           << message;
    return stream.str();
}

parse_error::parse_error(size_type line_, size_type column_, size_type character_, std::string message_) :
        std::runtime_error(parse_error_what(line_, column_, character_, message_)),
        _line(line_),
        _column(column_),
        _character(character_),
        _message(std::move(message_))
{ }

parse_error::~parse_error() noexcept = default;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parser Implementation                                                                                              //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace parser
{

bool context::next()
{
    character_no = character_no_next;
    if (std::getline(input, current))
    {
        NGINXCONFIG_DEBUG_PRINT("LINE:\t" << current);
        character_no_next = character_no + current.size();
        ++line_no;
        return true;
    }
    else
    {
        return false;
    }
}

ast_entry::attribute_list split_attributes(const std::string& attrs)
{
    ast_entry::attribute_list out;
    std::string::size_type pos = 0;
    while ((pos = attrs.find_first_not_of(" \t", pos)) != std::string::npos)
    {
        auto pos_end = attrs.find_first_of(" \t", pos);
        out.emplace_back(attrs.substr(pos, pos_end - pos));
        pos = pos_end;
    }
    return out;
}

line_components line_components::create_from_line(const std::string& line)
{
    #if NGINXCONFIG_USE_BOOST_REGEX
    using regex = boost::regex;
    using smatch = boost::smatch;
    using boost::regex_match;
    namespace regex_constants = boost::regex_constants;
    #else
    using regex = std::regex;
    using smatch = std::smatch;
    using std::regex_match;
    namespace regex_constants = std::regex_constants;
    #endif
    
    // All lines follow the same basic format:
    //  0) whole match
    //  1) name
    //  2) attributes
    //  3) either {, } or ;
    //  4) comment including #
    //  5) comment not including #
    static const regex line_regex(R"([ \t]*([A-Za-z_][A-Za-z0-9_]*)?[ \t]*([^{};#]*)([{};]?)[ \t]*(#(.*))?)",
                                  regex_constants::syntax_option_type(regex_constants::ECMAScript | regex_constants::optimize)
                                 );
    
    smatch results;
    line_components out;
    if (regex_match(line, results, line_regex))
    {
        #if NGINXCONFIG_DEBUG
        NGINXCONFIG_DEBUG_PRINT("REGEX MATCH sz=" << results.size());
        for (const auto& x : results)
        {
            NGINXCONFIG_DEBUG_PRINT("\t|" << x << '|');
        }
        #endif
        assert(results.size() == 6);
        out.name = results[1];
        out.attributes = split_attributes(results[2]);
        const std::string& tok = results[3];
        out.category = tok.size() == 0 ? line_kind::comment
                     : tok[0] == '{'    ? line_kind::complex_start
                     : tok[0] == '}'    ? line_kind::complex_end
                     : tok[0] == ';'    ? line_kind::simple
                     :                    line_kind::unknown;
        out.comment = results[5];
        NGINXCONFIG_DEBUG_PRINT(out.comment);
    }
    return out;
}

bool parse_generic(context& cxt, ast_entry& owner)
{
    while (cxt.next())
    {
        line_components components = line_components::create_from_line(cxt.current);
        switch (components.category)
        {
            case line_kind::comment:
                owner.children().emplace_back(ast_entry::make_comment(std::move(components.comment)));
                break;
            case line_kind::simple:
                owner.children().emplace_back(ast_entry::make_simple(std::move(components.name),
                                                                     std::move(components.attributes),
                                                                     std::move(components.comment)
                                                                    )
                                             );
                break;
            case line_kind::complex_start:
            {
                ast_entry child = ast_entry::make_complex(std::move(components.name),
                                                          std::move(components.attributes)
                                                         );
                while (parse_generic(cxt, child))
                {
                    // do nothing
                }
                owner.children().emplace_back(std::move(child));
                break;
            }
            case line_kind::complex_end:
                if (owner.kind() == ast_entry_kind::document)
                    throw cxt.create_parse_error(parse_error::no_column, "Unmatched end of nested entry");
                else
                    return false;
            case line_kind::unknown:
            default:
                throw cxt.create_parse_error(parse_error::no_column, "Indecipherable line: \"", cxt.current, '\"');
        }
    }
    if (owner.kind() == ast_entry_kind::complex)
        throw cxt.create_parse_error(parse_error::no_column, "EOF reached while inside nested entry");
    else
        return false;
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entry Points                                                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ast_entry parse(std::istream& input)
{
    parser::context cxt(input);
    auto out = ast_entry::make_document({});
    parser::parse_generic(cxt, out);
    return out;
}

/** Convenience function to parse a given file. **/
ast_entry parse_file(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    return parse(file);
}

}
