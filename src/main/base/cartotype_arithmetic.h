/*
cartotype_arithmetic.h
Copyright (C) 2004-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_base.h>
#include <cartotype_errors.h>

#include <math.h>

namespace CartoTypeCore
{

/** The intersection place of a line segment on another. */
enum class IntersectionPlace
    {
    /** Lines are parallel or coincident. */
    None = 0,
    /** The intersection is before the start of the segment. */
    Before,
    /** The intersection is on the segment. */
    On,
    /** The intersection is after the segment. */
    After
    };

/** The type of intersection of two line segments. */
class IntersectionType
    {
    public:
    /** Returns true if the lines, extended to infinity, do not intersect: that is, they are coincident or parallel. */
    bool None() const { return FirstSegment == IntersectionPlace::None && SecondSegment == IntersectionPlace::None; }
    /** Returns true if the line segments intersect each other within their lengths. */
    bool Both() const { return FirstSegment == IntersectionPlace::On && SecondSegment == IntersectionPlace::On; }

    /** The intersection place of the first line segment. */
    IntersectionPlace FirstSegment = IntersectionPlace::None;
    /** The intersection place of the second line segment. */
    IntersectionPlace SecondSegment = IntersectionPlace::None;
    };

/**
Rounds a floating-point value to the nearest integer.
Does not use floor() because it is said to be slow on some platforms.
*/
inline int32_t Round(double aValue)
    {
    return int32_t(aValue < 0.0 ? aValue - 0.5 : aValue + 0.5);
    }

/**
A non-locale-dependent version of the standard library strtod function.

Converts a string to a floating-point number and optionally returns a pointer to the position
after the portion of the string converted. The decimal point is always a full stop,
unlike the standard library function, which uses that of the current locale (e.g., a comma
in the German locale.)

If aLength is UINT32_MAX the string must be null-terminated.
*/
double Strtod(const char* aString,size_t aLength = UINT32_MAX,const char** aEndPtr = nullptr) noexcept;

/**
A non-locale-dependent version of the standard library strtod function, but
for UTF-16 characters.

Converts a string to a floating-point number and optionally returns a pointer to the position
after the portion of the string converted. The decimal point is always a full stop,
unlike the standard library function, which uses that of the current locale (e.g., a comma
in the German locale.)

If aLength is UINT32_MAX the string must be null-terminated.
*/
double Strtod(const uint16_t* aString,size_t aLength = UINT32_MAX,const uint16_t** aEndPtr = nullptr) noexcept;

/** A straight line or line segment defined using floating-point numbers. */
class LineFP
    {
    public:
    LineFP() = default;
    /** Creates a line from aStart to aEnd. */
    LineFP(const PointFP& aStart,const PointFP& aEnd):
        iStart(aStart),
        iEnd(aEnd)
        {
        }
    /** Returns the length of the line segment. */
    double Length() const
        {
        if (iLength < 0)
            {
            PointFP p = iEnd; p -= iStart; iLength = p.VectorLength();
            }
        return iLength;
        }
    /** Returns the start of the line segment. */
    const PointFP& Start() const { return iStart; }
    /** Returns the end of the line segment. */
    const PointFP& End() const { return iEnd; }
    /**
    Returns true if the distance from the line segment of either of the one or two supplied points
    exceeds aMaxDistance. This function is used when flattening curved paths, for finding
    whether the path is flat enough, which is when all the control points are
    near enough to the line segments.
    */
    bool DistanceExceeds(double aX1,double aY1,double aX2,double aY2,int32_t aPoints,double aMaxDistance) const;
    /**
    Returns a point at a certain length along a line in aPoint.
    aLength can be negative, or beyond the end of the line.
    In these cases the line is extended in the same direction.
    */
    PointFP PointAtLength(double aLength) const;
    /** Returns a reversed copy of this line. */
    LineFP Reverse() const { LineFP l; l.iStart = iEnd; l.iEnd = iStart; l.iLength = iLength; return l; }

    private:
    PointFP iStart;
    PointFP iEnd;
    mutable double iLength = -1;
    };

/**
An arctangent function which checks for two zero arguments and returns zero in that case.
In the standard library atan2(0,0) is undefined, and on Embarcadero C++ Builder it throws an exception.
*/
inline double Atan2(double aY,double aX)
    {
    if (aY == 0 && aX == 0)
        return 0;
    return atan2(aY,aX);
    }

} // namespace CartoTypeCore
