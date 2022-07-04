/*
cartotype_iter.h
Copyright (C) 2004-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_types.h>
#include <cartotype_errors.h>

namespace CartoTypeCore
{

/** The iterator interface provides a sequence of objects of any type. */
template<class T> class MIter
    {
    public:
    /**
    Gets the next object. Returns KErrorNone if an object was retrieved,
    KErrorEndOfData if there are no more objects, or some other result code
    if there was a different type of error.
    */
    virtual Result Next(T& aValue) = 0;
    /**
    Back up one position if possible so that the next call to Next
    returns the same object as the previous call.
    */
    virtual void Back() = 0;
    };

} // namespace CartoTypeCore

