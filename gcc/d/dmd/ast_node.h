
/* Compiler implementation of the D programming language
 * Copyright (C) 1999-2023 by The D Language Foundation, All Rights Reserved
 * written by Walter Bright
 * https://www.digitalmars.com
 * Distributed under the Boost Software License, Version 1.0.
 * https://www.boost.org/LICENSE_1_0.txt
 * https://github.com/dlang/dmd/blob/master/src/dmd/ast_node.h
 */

#pragma once

#include "root/object.h"

class Visitor;

class ASTNode : public RootObject
{
    virtual void accept(Visitor*) = 0;
};
