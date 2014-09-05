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

#include "test.hpp"

using namespace nginxconfig;

TEST(ast_equality)
{
    ast_entry x = ast_entry::make_simple("name", { "x", "y", "z" }, "blah");
    ast_entry y = ast_entry::make_simple("name", { "x", "y", "z" }, "blah");
    ensure_eq(x, y);
    y.attributes().emplace_back("w");
    ensure_ne(x, y);
    
    x = ast_entry::make_complex("container", { "some_thing" }, { y });
    ensure_ne(x, y);
    ensure_eq(x.children().at(0), y);
    
    swap(x, y);
    ensure_ne(x, y);
    ensure_eq(y.children().at(0), x);
}
