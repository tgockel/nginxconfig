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

#include <cassert>
#include <ostream>

namespace nginxconfig
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// encoder                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const encoder::context::path_type& encoder::context::path() const
{
    return _path;
}

encoder::context::size_type encoder::context::indent_level() const
{
    return _path.empty() ? 0 : _path.size() - 1;
}

encoder::~encoder() noexcept = default;

void encoder::encode(const ast_entry& ast)
{
    context cxt;
    return encode_impl(cxt, ast);
}

void encoder::encode_impl(encoder::context& cxt, const ast_entry& ast)
{
    switch (ast.kind())
    {
        case ast_entry_kind::comment:
            write_comment(cxt, ast);
            break;
        case ast_entry_kind::complex:
        case ast_entry_kind::document:
            if (ast.kind() == ast_entry_kind::complex)
                write_complex_begin(cxt, ast);
            else
                write_document_begin(cxt, ast);
            
            cxt._path.push_back(&ast);
            for (const ast_entry& sub : ast.children())
            {
                encode_impl(cxt, sub);
            }
            cxt._path.pop_back();
            
            if (ast.kind() == ast_entry_kind::complex)
                write_complex_end(cxt, ast);
            else
                write_document_end(cxt, ast);
            break;
        case ast_entry_kind::simple:
            write_simple(cxt, ast);
            break;
        default:
            assert(false && "Memory corruption?");
            break;
    }
}

void encoder::write_document_begin(const context&, const ast_entry&)
{ }

void encoder::write_document_end(const context&, const ast_entry&)
{ }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ostream_encoder                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const std::string default_indent = "  ";

ostream_encoder::ostream_encoder(std::ostream& output, std::string indent) :
        _output(output),
        _indent(indent)
{ }

ostream_encoder::ostream_encoder(std::ostream& output) :
        ostream_encoder(output, default_indent)
{ }

ostream_encoder::~ostream_encoder() noexcept = default;

void ostream_encoder::write_attributes(const ast_entry& ast)
{
    for (const auto& attr : ast.attributes())
        _output << ' ' << attr;
}

void ostream_encoder::write_comment(const context& cxt, const ast_entry& ast)
{
    write_indent(cxt);
    if (!ast.comment().empty())
        _output << '#' << ast.comment();
    _output << std::endl;
}

void ostream_encoder::write_complex_begin(const context& cxt, const ast_entry& ast)
{
    write_indent(cxt);
    _output << ast.name();
    write_attributes(ast);
    _output << " {";
    if (!ast.comment().empty())
        _output << '#' << ast.comment();
    _output << std::endl;
}

void ostream_encoder::write_complex_end(const context& cxt, const ast_entry&)
{
    write_indent(cxt);
    _output << '}';
    _output << std::endl;
}

void ostream_encoder::write_indent(const context& cxt)
{
    for (context::size_type x = 0; x < cxt.indent_level(); ++x)
        _output << _indent;
}

void ostream_encoder::write_simple(const encoder::context& cxt, const ast_entry& ast)
{
    write_indent(cxt);
    _output << ast.name();
    write_attributes(ast);
    _output << " ;";
    if (!ast.comment().empty())
        _output << '#' << ast.comment();
    _output << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Free Functions                                                                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void encode(const ast_entry& ast, std::ostream& output, std::string indent)
{
    ostream_encoder encoder(output, std::move(indent));
    encoder.encode(ast);
}

void encode(const ast_entry& ast, std::ostream& output)
{
    return encode(ast, output, default_indent);
}

}
