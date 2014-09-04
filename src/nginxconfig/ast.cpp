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
#include <nginxconfig/ast.hpp>
#include <nginxconfig/encode.hpp>

#include <algorithm>
#include <sstream>

namespace nginxconfig
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helpers                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void check_kind(std::initializer_list<ast_entry_kind> expected, ast_entry_kind actual)
{
    if (std::none_of(expected.begin(), expected.end(), [actual] (ast_entry_kind x) { return x == actual; }))
    {
        std::ostringstream stream;
        stream << "Unexpected kind: expected ";
        std::size_t num = 1;
        for (ast_entry_kind k : expected)
        {
            stream << k;
            if (num + 1 < expected.size())
                stream << ", ";
            else if (num < expected.size())
                stream << " or ";
            ++num;
        }
        throw kind_error(stream.str());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ast_entry_kind                                                                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const ast_entry_kind& kind)
{
    switch (kind)
    {
    case ast_entry_kind::complex:  return os << "complex";
    case ast_entry_kind::comment:  return os << "comment";
    case ast_entry_kind::document: return os << "document";
    case ast_entry_kind::simple:   return os << "simple";
    default:                       return os << "???";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kind_error                                                                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

kind_error::kind_error(const std::string& description) :
        std::logic_error(description)
{ }

kind_error::~kind_error() noexcept = default;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ast_entry                                                                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ast_entry::ast_entry(ast_entry_kind kind_) :
        _kind(kind_)
{ }

ast_entry::ast_entry(const ast_entry&) = default;
ast_entry& ast_entry::operator=(const ast_entry&) = default;

ast_entry::ast_entry(ast_entry&& src) noexcept :
        _kind(src._kind),
        _name(std::move(src._name)),
        _attributes(std::move(src._attributes)),
        _children(std::move(src._children)),
        _comment(std::move(src._comment))
{ }

ast_entry& ast_entry::operator=(ast_entry&& src) noexcept
{
    _kind = src._kind;
    _name = std::move(src._name);
    _attributes = std::move(src._attributes);
    _children = std::move(src._children);
    _comment = std::move(src._comment);
    return *this;
}

ast_entry::~ast_entry() noexcept = default;

void swap(ast_entry& a, ast_entry& b) noexcept
{
    using std::swap;
    swap(a._kind, b._kind);
    swap(a._name, b._name);
    swap(a._attributes, b._attributes);
    swap(a._children, b._children);
}

ast_entry ast_entry::make_complex(std::string    name,
                                  attribute_list attributes,
                                  child_list     children
                                 )
{
    ast_entry out(ast_entry_kind::complex);
    out.name()       = std::move(name);
    out.attributes() = std::move(attributes);
    out.children()   = std::move(children);
    return out;
}

ast_entry ast_entry::make_document(child_list children)
{
    ast_entry out(ast_entry_kind::document);
    out.children()   = std::move(children);
    return out;
}

ast_entry ast_entry::make_simple(std::string    name,
                                 attribute_list attributes,
                                 std::string    comment_text
                                )
{
    ast_entry out(ast_entry_kind::simple);
    out.name()       = std::move(name);
    out.attributes() = std::move(attributes);
    out.comment()    = std::move(comment_text);
    return out;
}

ast_entry ast_entry::make_comment(std::string comment_text)
{
    ast_entry out(ast_entry_kind::comment);
    out.comment() = std::move(comment_text);
    return out;
}

ast_entry_kind ast_entry::kind() const
{
    return _kind;
}

const ast_entry::attribute_list& ast_entry::attributes() const
{
    check_kind({ ast_entry_kind::complex, ast_entry_kind::simple }, kind());
    return _attributes;
}

ast_entry::attribute_list& ast_entry::attributes()
{
    check_kind({ ast_entry_kind::complex, ast_entry_kind::simple }, kind());
    return _attributes;
}

const ast_entry::child_list& ast_entry::children() const
{
    check_kind({ ast_entry_kind::complex, ast_entry_kind::document }, kind());
    return _children;
}

ast_entry::child_list& ast_entry::children()
{
    check_kind({ ast_entry_kind::complex, ast_entry_kind::document }, kind());
    return _children;
}

const std::string& ast_entry::comment() const
{
    check_kind({ ast_entry_kind::comment, ast_entry_kind::simple, ast_entry_kind::complex }, kind());
    return _comment;
}

std::string& ast_entry::comment()
{
    check_kind({ ast_entry_kind::comment, ast_entry_kind::simple, ast_entry_kind::complex }, kind());
    return _comment;
}

const std::string& ast_entry::name() const
{
    check_kind({ ast_entry_kind::complex, ast_entry_kind::simple }, kind());
    return _name;
}

std::string& ast_entry::name()
{
    check_kind({ ast_entry_kind::complex, ast_entry_kind::simple }, kind());
    return _name;
}

std::ostream& operator<<(std::ostream& os, const ast_entry& ast)
{
    encode(ast, os);
    return os;
}

}
