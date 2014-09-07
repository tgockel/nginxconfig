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
    /** A \c comment has only the comment string. **/
    comment,
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
    /** Create a \c simple AST entry. **/
    static ast_entry make_simple(std::string    name,
                                 attribute_list attributes   = attribute_list(),
                                 std::string    comment_text = std::string()
                                );
    
    /** Create a \c complex AST entry. **/
    static ast_entry make_complex(std::string    name,
                                  attribute_list attributes = attribute_list(),
                                  child_list     children   = child_list()
                                 );
    
    /** Create a \c document. **/
    static ast_entry make_document(child_list children   = child_list());
    
    /** Create a \c comment AST entry. **/
    static ast_entry make_comment(std::string comment_text);
    
    ast_entry(const ast_entry&);
    ast_entry& operator=(const ast_entry&);
    
    ast_entry(ast_entry&&) noexcept;
    ast_entry& operator=(ast_entry&&) noexcept;
    
    ~ast_entry() noexcept;
    
    /** Swap the contents of \a a with \a b. This will never throw. **/
    friend void swap(ast_entry& a, ast_entry& b) noexcept;
    
    /** Get the kind of entry this is.
     *  
     *  \see ast_entry_kind
    **/
    ast_entry_kind kind() const;
    
    /** The name of the entry is the first part of a \c simple or \c complex entry. This is typically something like
     *  "http", "location" or "root". The only allowed characters according to nginx are alphanumeric and underscores,
     *  but your input is not validated.
     *  
     *  \throws kind_error if \c kind is not \c simple or \c complex.
    **/
    const std::string& name() const;
    std::string&       name();
    
    /** The attributes immediately follow the \c name of a \c simple or \c complex entry. This is used to specify
     *  additional options to an entry.
     *  
     *  \throws kind_error if \c kind is not \c simple or \c complex.
    **/
    const attribute_list& attributes() const;
    attribute_list&       attributes();
    
    /** Get the children of a \c complex or \c document entry.
     * 
     *  \throws kind_error if \c kind is not \c complex or \c document.
    **/
    const child_list& children() const;
    child_list&       children();
    
    /** Get the comment of a \c comment, \c simple or \c complex entry. Comments are appended \e after the closing \c ;
     *  or \c {. For \c document comments, add them as a child.
     *  
     *  \throws kind_error if \c kind is not \c comment, \c simple or \c complex.
    **/
    const std::string& comment() const;
    std::string&       comment();
    
    /** Compare the AST for equality. **/
    bool operator==(const ast_entry& other) const;
    bool operator!=(const ast_entry& other) const;
    
private:
    explicit ast_entry(ast_entry_kind kind);
    
private:
    ast_entry_kind _kind;
    std::string    _name;
    attribute_list _attributes;
    child_list     _children;
    std::string    _comment;
};

NGINXCONFIG_PUBLIC std::ostream& operator<<(std::ostream&, const ast_entry&);

}

#endif/*__NGINXCONFIG_AST_HPP_INCLUDED__*/
