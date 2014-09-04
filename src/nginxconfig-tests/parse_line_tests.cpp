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
    auto x = line_components::create_from_line("worker_processes  1;");
    ensure(line_kind::simple == x.category);
    ensure_eq(x.name, "worker_processes");
}
