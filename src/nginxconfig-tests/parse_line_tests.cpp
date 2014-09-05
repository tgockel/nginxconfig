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

#include <nginxconfig/all.hpp>

#include <nginxconfig/parse_types.hpp>

#include "test.hpp"

using namespace nginxconfig::parser;

using attribute_list = nginxconfig::ast_entry::attribute_list;

TEST(line_components_empty)
{
    auto x = line_components::create_from_line("");
    ensure(line_kind::comment == x.category);
}

TEST(line_components_comment_only)
{
    auto x = line_components::create_from_line("#user html");
    ensure(line_kind::comment == x.category);
    ensure_eq(x.comment, "user html");
}


TEST(line_components_comment_only_whitespace_prefix)
{
    auto x = line_components::create_from_line("  \t#user html");
    ensure(line_kind::comment == x.category);
    ensure_eq(x.comment, "user html");
}

TEST(line_components_simple_no_comment)
{
    for (auto source : {
                         "worker_processes 1 blah;",
                         "\tworker_processes 1\tblah   ;",
                         "  \t worker_processes\t        1            blah;             ",
                       })
    {
        auto x = line_components::create_from_line(source);
        ensure(line_kind::simple == x.category);
        ensure_eq(x.name, "worker_processes");
        ensure(x.attributes == attribute_list({ "1", "blah" }));
    }
}

TEST(line_components_simple_comment)
{
    for (auto source : {
                         "worker_processes 1 bl/ah;# comment",
                         "\tworker_processes 1\tbl/ah   ;   # comment",
                         "  \t worker_processes\t        1            bl/ah;\t\t# comment",
                       })
    {
        auto x = line_components::create_from_line(source);
        ensure(line_kind::simple == x.category);
        ensure_eq(x.name, "worker_processes");
        ensure(x.attributes == attribute_list({ "1", "bl/ah" }));
        ensure_eq(x.comment, " comment");
    }
}
