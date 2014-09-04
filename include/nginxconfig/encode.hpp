/** \file nginxconfig/encode.hpp
 *  Support for advanced encoding options.
 *  
 *  Copyright (c) 2014 by Travis Gockel. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of the Apache License
 *  as published by the Apache Software Foundation, either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  \author Travis Gockel (travis@gockelhut.com)
**/
#ifndef __NGINXCONFIG_ENCODE_HPP_INCLUDED__
#define __NGINXCONFIG_ENCODE_HPP_INCLUDED__

#include <nginxconfig/config.hpp>

#include <deque>
#include <iosfwd>
#include <string>

namespace nginxconfig
{

class ast_entry;

/** Write the provided \c ast to the \c output stream. Each complex entry increases the indentation level, which means
 *  another column of \c indent.
**/
NGINXCONFIG_PUBLIC void encode(const ast_entry& ast, std::ostream& output, std::string indent);
NGINXCONFIG_PUBLIC void encode(const ast_entry& ast, std::ostream& output);

/** An encoder is responsible to writing to some form of output. **/
class NGINXCONFIG_PUBLIC encoder
{
public:
    virtual ~encoder() noexcept;
    
    void encode(const ast_entry& ast);
    
protected:
    class context
    {
    public:
        using path_type = std::deque<const ast_entry*>;
        using size_type = path_type::size_type;
        
    public:
        const path_type& path() const;
        
        size_type indent_level() const;
        
    private:
        friend class encoder;
        
        path_type _path;
    };
    
    virtual void write_simple(const context& cxt, const ast_entry& ast) = 0;
    
    virtual void write_complex_begin(const context& cxt, const ast_entry& ast) = 0;
    
    virtual void write_complex_end(const context& cxt, const ast_entry& ast) = 0;
    
    virtual void write_document_begin(const context& cxt, const ast_entry& ast);
    
    virtual void write_document_end(const context& cxt, const ast_entry& ast);
    
    virtual void write_comment(const context& cxt, const ast_entry& ast) = 0;
    
private:
    void encode_impl(context& cxt, const ast_entry& ast);
};

class NGINXCONFIG_PUBLIC ostream_encoder :
        public encoder
{
public:
    explicit ostream_encoder(std::ostream& output);
    explicit ostream_encoder(std::ostream& output, std::string indent);
    
    virtual ~ostream_encoder() noexcept;
    
protected:
    virtual void write_simple(const context& cxt, const ast_entry& ast) override;
    
    virtual void write_complex_begin(const context& cxt, const ast_entry& ast) override;
    
    virtual void write_complex_end(const context& cxt, const ast_entry& ast) override;
    
    virtual void write_comment(const context& cxt, const ast_entry& ast) override;
    
private:
    void write_indent(const context& cxt);
    
    void write_attributes(const ast_entry& ast);
    
private:
    std::ostream& _output;
    std::string   _indent;
};

}

#endif/*__NGINXCONFIG_ENCODE_HPP_INCLUDED__*/
