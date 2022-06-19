// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include "Luau/Ast.h"
#include "Luau/Module.h"

namespace Luau
{

void lua_check(const SourceModule& sourceModule, Module* module);

} // namespace Luau
