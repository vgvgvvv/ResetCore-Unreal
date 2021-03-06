// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#pragma once

#include "Luau/Location.h"
#include "Luau/NotNull.h"
#include "Luau/Variant.h"

#include <memory>
#include <vector>

namespace Luau
{

struct Scope2;
struct TypeVar;
using TypeId = const TypeVar*;

struct TypePackVar;
using TypePackId = const TypePackVar*;

// subType <: superType
struct SubtypeConstraint
{
    TypeId subType;
    TypeId superType;
};

// subPack <: superPack
struct PackSubtypeConstraint
{
    TypePackId subPack;
    TypePackId superPack;
};

// subType ~ gen superType
struct GeneralizationConstraint
{
    TypeId generalizedType;
    TypeId sourceType;
    Scope2* scope;
};

// subType ~ inst superType
struct InstantiationConstraint
{
    TypeId subType;
    TypeId superType;
};

using ConstraintV = Variant<SubtypeConstraint, PackSubtypeConstraint, GeneralizationConstraint, InstantiationConstraint>;
using ConstraintPtr = std::unique_ptr<struct Constraint>;

struct Constraint
{
    Constraint(ConstraintV&& c, Location location);

    Constraint(const Constraint&) = delete;
    Constraint& operator=(const Constraint&) = delete;

    ConstraintV c;
    Location location;
    std::vector<NotNull<Constraint>> dependencies;
};

inline Constraint& asMutable(const Constraint& c)
{
    return const_cast<Constraint&>(c);
}

template<typename T>
T* getMutable(Constraint& c)
{
    return ::Luau::get_if<T>(&c.c);
}

template<typename T>
const T* get(const Constraint& c)
{
    return getMutable<T>(asMutable(c));
}

} // namespace Luau
