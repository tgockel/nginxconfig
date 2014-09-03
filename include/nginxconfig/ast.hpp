/** \file nginxconfig/ast.hpp
 *  Raw manipulation of the nginx configuration AST.
 *  
 *  Copyright (c) 2014 by Travis Gockel. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of the Apache License
 *  as published by the Apache Software Foundation, either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  \author Travis Gockel (travis@gockelhut.com)
**/
#ifndef __NGINXCONFIG_AST_HPP_INCLUDED__
#define __NGINXCONFIG_AST_HPP_INCLUDED__

#include <nginxconfig/config.hpp>

#include <deque>
#include <iosfwd>
#include <stdexcept>
#include <string>

namespace nginxconfig
{

enum class ast_entry_kind : unsigned int
{
    /** A \c simple entry is a name and a list of atrributes. **/
    simple,
    /** A \c complex entry has a name, a list of attributes and a list of child entries. **/
    complex,
    /** A \c document has child entries and only exists as the root entry of a configuration file. **/
    document,
};

NGINXCONFIG_PUBLIC std::ostream& operator<<(std::ostream& os, const ast_entry_kind& kind);

/** Thrown when attempting to perform an operation on an \c ast_entry of the wrong kind. **/
class NGINXCONFIG_PUBLIC kind_error :
        public std::logic_error
{
public:
    explicit kind_error(const std::string& description);
    
    virtual ~kind_error() noexcept;
};

/** Represents an entry in an nginx configuration file. **/
class NGINXCONFIG_PUBLIC ast_entry
{
public:
    using attribute_list = std::deque<std::string>;
    using child_list     = std::deque<ast_entry>;

public:
    static ast_entry make_simple(std::string    name,
                                 attribute_list attributes = attribute_list()
                                );
    
    static ast_entry make_complex(std::string    name,
                                  attribute_list attributes = attribute_list(),
                                  child_list     children   = child_list()
                                 );
    
    static ast_entry make_document(child_list children   = child_list());
    
    ast_entry(const ast_entry&);
    ast_entry& operator=(const ast_entry&);
    
    ast_entry(ast_entry&&) noexcept;
    ast_entry& operator=(ast_entry&&) noexcept;
    
    ~ast_entry() noexcept;
    
    ast_entry_kind kind() const;
    
    const std::string& name() const;
    std::string&       name();
    
    const attribute_list& attributes() const;
    attribute_list&       attributes();
    
    const child_list& children() const;
    child_list&       children();
    
private:
    explicit ast_entry(ast_entry_kind kind);
    
private:
    ast_entry_kind _kind;
    std::string    _name;
    attribute_list _attributes;
    child_list     _children;
};

}

#endif/*__NGINXCONFIG_AST_HPP_INCLUDED__*/
