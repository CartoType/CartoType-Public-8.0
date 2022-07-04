/*
cartotype_base.h
Copyright (C) 2004-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_errors.h>

#include <memory>
#include <vector>
#include <cmath>
#include <functional>
#include <array>

// Undefine the min and max macros so that std::min and std::max work on Windows, etc.
#undef min
#undef max

namespace CartoTypeCore
{

const char* Version();
const char* Build();

class RectFP;

/** A point in two-dimensional space. */
class Point
    {
    public:
    /** Create a point with coordinates (0,0). */
    constexpr Point() noexcept { }
    /** Create a point, specifying coordinates. */
    constexpr Point(int32_t aX,int32_t aY) noexcept : X(aX), Y(aY) { }
    /** The equality operator. */
    constexpr bool operator==(const Point& aPoint) const noexcept { return X == aPoint.X && Y == aPoint.Y; }
    /** The inequality operator. */
    constexpr bool operator!=(const Point& aPoint) const noexcept { return !(*this == aPoint); }
    /** Offset a point by another point, treated as a positive vector. */
    void operator+=(const Point& aPoint) noexcept { X += aPoint.X; Y += aPoint.Y; }
    /** Offset a point by another point, treated as a negative vector. */
    void operator-=(const Point& aPoint) noexcept { X -= aPoint.X; Y -= aPoint.Y; }
    /** The less-than operator, ordering points by x coordinate, then y coordinate. */
    constexpr bool operator<(const Point& aPoint) const noexcept
        {
        return (X < aPoint.X) ||
               ((X == aPoint.X) && Y < aPoint.Y);
        }
    /** The greater-than operator, ordering points by x coordinate, then y coordinate. */
    constexpr bool operator>(const Point& aPoint) const noexcept
        {
        return (X > aPoint.X) ||
               ((X == aPoint.X) && Y > aPoint.Y);
        }

    /** Return a single number which can be used to sort points into order. */
    constexpr uint64_t ComparisonValue() const noexcept { return (uint64_t(X) << 32) + uint64_t(Y); }

    /** The x coordinate. */
    int32_t X { 0 };
    /** The y coordinate. */
    int32_t Y { 0 };
    };

/**
A templated 2D point or vector class.
In the vector functions, left and right are defined using x increasing right and y increasing up.
*/
template<class T> class Point2
    {
    public:
    /** Create a point with coordinates (0,0). */
    constexpr Point2() noexcept { }
    /** Create a point, specifying coordinates. */
    constexpr Point2(T aX,T aY) noexcept : X(aX), Y(aY) { }
    /** Create a point from a Point object. */
    constexpr Point2(const Point& aPoint) noexcept : X(aPoint.X), Y(aPoint.Y) { }
    /** The equality operator. */
    bool operator==(const Point2<T>& aPoint) const noexcept { return X == aPoint.X && Y == aPoint.Y; }
    /** The inequality operator. */
    bool operator!=(const Point2<T>& aPoint) const noexcept { return !(*this == aPoint); }
    /** Offset a point by another point, treated as a positive vector. */
    void operator+=(const Point2<T>& aPoint) noexcept { X += aPoint.X; Y += aPoint.Y; }
    /** Offset a point by another point, treated as a negative vector. */
    void operator-=(const Point2<T>& aPoint) noexcept { X -= aPoint.X; Y -= aPoint.Y; }
    /** Scale a point by a factor. */
    void operator*=(T aFactor) noexcept { X *= aFactor; Y *= aFactor; }
    /** The less-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator<(const Point2<T>& aPoint) const noexcept
        {
        if (X < aPoint.X)
            return true;
        if (X > aPoint.X)
            return false;
        if (Y < aPoint.Y)
            return true;
        return false;
        }
    /** The greater-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator>(const Point2<T>& aPoint) const noexcept
        {
        if (X > aPoint.X)
            return true;
        if (X < aPoint.X)
            return false;
        if (Y > aPoint.Y)
            return true;
        return false;
        }
    /** Returns the length of the vector represented by this point: its distance from the origin (0,0). */
    T VectorLength() const noexcept
        {
        return sqrt(X * X + Y * Y);
        }
    /** Returns the distance of this point from another point. */
    T DistanceFrom(const Point2<T>& aOther) const noexcept
        {
        Point2<T> v { *this };
        v -= aOther;
        return v.VectorLength();
        }
    /**
    Returns the cross product of two vectors.
    The cross product of two unit vectors is the sine of the angle swept out
    by going from this vector to the other vector in an anticlockwise direction,
    and may be positive or negative.
    */
    T CrossProduct(const Point2<T>& aOther) const noexcept
        {
        return X * aOther.Y - Y * aOther.X;
        }
    /** Returns a unit vector pointing in the same direction as this point treated as a vector. */
    Point2 UnitVector() const noexcept
        {
        T length = VectorLength();
        if (length > 0)
            {
            Point2<T> p(*this);
            p.X /= length;
            p.Y /= length;
            return p;
            }
        return Point2<T>(1,0);
        }

    /** Returns a unit vector pointing 90 degrees left of this vector. */
    Point2 LeftUnitVector() const noexcept
        {
        Point2<T> u { UnitVector() };
        return Point2<T>(-u.Y,u.X);
        }
    /** Returns a unit vector pointing 90 degrees right of this vector. */
    Point2 RightUnitVector() const noexcept
        {
        Point2<T> u { UnitVector() };
        return Point2<T>(u.Y,-u.X);
        }
    /** Returns true if aPoint is left of this vector. */
    bool LeftOfVector(const Point2<T>& aPoint) const noexcept
        {
        return CrossProduct(aPoint) > 0;
        }
    /** Returns true if aPoint is right of this vector. */
    bool RightOfVector(const Point2<T>& aPoint) const noexcept
        {
        return CrossProduct(aPoint) < 0;
        }
    /** Returns an integer point by rounding the coordinates to the nearest integer. Coordinates with a fractional part of 0.5 are rounded away from zero. */
    Point Rounded() const noexcept
        {
        return Point((int32_t)(X < 0.0 ? X - 0.5 : X + 0.5),(int32_t)(Y < 0.0 ? Y - 0.5 : Y + 0.5));
        }
    /** Returns an integer point in rounded 64ths of the values in this point. */
    Point Rounded64ths() const noexcept
        {
        Point2<T> p { X * 64, Y * 64 };
        return p.Rounded();
        }
    /** Rotates the point about the origin by an angle given in radians. */
    void Rotate(double aAngle)
        {
        double c = cos(aAngle);
        double s = sin(aAngle);
        double x = c * X - s * Y;
        double y = s * X + c * Y;
        X = x;
        Y = y;
        }

    /** The x coordinate. */
    T X { 0 };
    /** The y coordinate. */
    T Y { 0 };
    };

/** A templated 3D point class. */
template<class T> class Point3
    {
    public:
    /** Create a point with coordinates (0,0,0). */
    Point3() noexcept { }
    /** Create a point, specifying coordinates. */
    Point3(T aX,T aY,T aZ) noexcept : X(aX), Y(aY), Z(aZ) { }
    /** Create a 3D point from a 2D point. */
    Point3(Point2<T> aPoint) noexcept : X(aPoint.X), Y(aPoint.Y), Z(0) { }
    /** The equality operator. */
    bool operator==(const Point3<T>& aPoint) const noexcept { return X == aPoint.X && Y == aPoint.Y && Z == aPoint.Z; }
    /** The inequality operator. */
    bool operator!=(const Point3<T>& aPoint) const noexcept { return !(*this == aPoint); }
    /** Offset a point by another point, treated as a positive vector. */
    void operator+=(const Point3<T>& aPoint) noexcept { X += aPoint.X; Y += aPoint.Y; Z += aPoint.Z; }
    /** Offset a point by another point, treated as a negative vector. */
    void operator-=(const Point3<T>& aPoint) noexcept { X -= aPoint.X; Y -= aPoint.Y; Z -= aPoint.Z; }
    /** Scale a point by a factor. */
    void operator*=(T aFactor) noexcept { X *= aFactor; Y *= aFactor; Z *= aFactor; }
    /** The less-than operator, ordering points by x coordinate, then y coordinate, then z coordinate. */
    bool operator<(const Point3<T>& aPoint) const noexcept
        {
        if (X < aPoint.X)
            return true;
        if (X > aPoint.X)
            return false;
        if (Y < aPoint.Y)
            return true;
        if (Y > aPoint.Y)
            return false;
        if (Z < aPoint.Z)
            return true;
        return false;
        }
    /** The greater-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator>(const Point2<T>& aPoint) const noexcept
        {
        if (X > aPoint.X)
            return true;
        if (X < aPoint.X)
            return false;
        if (Y > aPoint.Y)
            return true;
        if (Y < aPoint.Y)
            return false;
        if (Z > aPoint.Z)
            return true;
        return false;
        }
    /** Returns the length of the vector represented by this point: its distance from the origin (0,0). */
    T VectorLength() const noexcept
        {
        return sqrt(X * X + Y * Y + Z * Z);
        }
    /** Returns the distance of this point from another point. */
    T DistanceFrom(const Point3<T>& aOther) const noexcept
        {
        Point3<T> v { *this };
        v -= aOther;
        return v.VectorLength();
        }
    /** Returns a unit vector pointing in the same direction as this point treated as a vector. */
    Point3 UnitVector() const noexcept
        {
        T length = VectorLength();
        if (length > 0)
            {
            Point3 p(*this);
            p.X /= length;
            p.Y /= length;
            p.Z /= length;
            return p;
            }
        return Point3<T>(1,0,0);
        }

    /** Returns the surface normal as a unit vector. */
    static Point3<T> Normal(const Point3<T>& aA,const Point3<T>& aB,const Point3<T>& aC) noexcept
        {
        Point3<T> b { aB }; b -= aA;
        Point3<T> c { aC }; c -= aA;
        Point3<T> normal { b.Y * c.Z - b.Z * c.Y,
                            b.Z * c.X - b.X * c.Z,
                            b.X * c.Y - b.Y * c.X };
        return normal.UnitVector();
        }

    /** The x coordinate. */
    T X { 0 };
    /** The y coordinate. */
    T Y { 0 };
    /** The z coordinate. */
    T Z { 0 };
    };

/** A double-precision floating-point 2D point class. */
using PointFP = Point2<double>;

/** A double-precision floating-point 3D point class. */
using Point3FP = Point3<double>;

/** A non-owned ordered set of 2D coordinates accessed via pointers to the first X and first Y coordinate. */
class CoordSet
    {
    public:
    /**
    Creates a coordinate set from two separate contiguous arrays of points.
    aX points to a contiguous array of X coordinates.
    aY points to a contiguous array of Y coordinates.
    aCount is the number of coordinate pairs.
    */
    CoordSet(const double* aX,const double* aY,size_t aCount) noexcept:
        m_x(aX),
        m_y(aY),
        m_stride(sizeof(double)),
        m_count(aCount)
        {
        }
    /** Creates a coordinate set from an array of aCount numbers, arranged in x,y pairs; aCount must be an even number. */
    CoordSet(const double* aCoordArray,size_t aCount) noexcept:
        m_x(aCoordArray),
        m_y(aCoordArray + 1),
        m_stride(sizeof(double) * 2),
        m_count(aCount / 2)
        {
        }
    /** Creates a coordinate set from an array of aCount point objects. */
    template<class point_t> CoordSet(const point_t* aPointArray,size_t aCount) noexcept:
        m_x(&aPointArray->X),
        m_y(&aPointArray->Y),
        m_stride(sizeof(point_t)),
        m_count(aCount)
        {
        }
    /** Creates a coordinate set from a vector of point objects. */
    template<class point_t> CoordSet(const std::vector<point_t>& aPointArray) noexcept:
        m_x(&aPointArray.front().X),
        m_y(&aPointArray.front().Y),
        m_stride(sizeof(point_t)),
        m_count(aPointArray.size())
        {
        }

    /** Returns the X coordinate at a specified index. */
    double X(size_t aIndex) const noexcept { return *(const double*)((char*)m_x + m_stride * aIndex); }
    /** Returns the Y coordinate at a specified index. */
    double Y(size_t aIndex) const noexcept { return *(const double*)((char*)m_y + m_stride * aIndex); }
    /** Returns the point at a specified index. */
    PointFP Point(size_t aIndex) const noexcept
        {
        return PointFP { X(aIndex),Y(aIndex) };
        }
    /** Returns the number of points in the coordinate set. */
    size_t Count() const noexcept { return m_count; }

    double DistanceFromPoint(bool aIsPolygon,double aX,double aY,double& aNearestX,double& aNearestY) const noexcept;
    bool PolygonContains(double aX,double aY) const noexcept;

    protected:        
    /** Creates an empty coordinate set. */
    constexpr CoordSet() noexcept {}

    /** A pointer to the first X coordinate. */
    const double* m_x { nullptr };
    /** A pointer to the first Y coordinate. */
    const double* m_y { nullptr };
    /** The distance in bytes between successive X or Y coordinates. */
    size_t m_stride { 0 };
    /** The number of points (pairs of coordinates). */
    size_t m_count { 0 };
    };

/** A set of modifiable coordinate pairs. */
class WritableCoordSet: public CoordSet
    {
    public:
    using CoordSet::CoordSet;

    /** Returns a writable reference to the X coordinate at a specified index. */
    double& X(size_t aIndex) const { return *(double*)((char*)m_x + m_stride * aIndex); }
    /** Returns a writable reference to the Y coordinate at a specified index. */
    double& Y(size_t aIndex) const { return *(double*)((char*)m_y + m_stride * aIndex); }
    };

/** A CoordSet consisting of a single point. */
class CoordPair: public CoordSet
    {
    public:
    /** Creates a CoordPair from an X and a Y coordinate. */
    CoordPair(double aX,double aY) noexcept :
        OwnX(aX),
        OwnY(aY)
        {
        m_x = &OwnX;
        m_y = &OwnY;
        m_stride = 0;
        m_count = 1;
        }

    /** The X coordinate. */
    double OwnX;
    /** The Y coordinate. */
    double OwnY;
    };

/** A CoordSet consisting of two points. */
class CoordSetOfTwoPoints: public CoordSet
    {
    public:
    /** Creates a CoordSetOfTwoPoints from two pairs of coordinates. */
    CoordSetOfTwoPoints(double aX0,double aY0,double aX1,double aY1) noexcept
        {
        OwnX[0] = aX0;
        OwnX[1] = aX1;
        OwnY[0] = aY0;
        OwnY[1] = aY1;
        m_x = OwnX;
        m_y = OwnY;
        m_stride = sizeof(double);
        m_count = 2;
        }

    /** The X coordinates. */
    double OwnX[2] = { };
    /** The Y coordinates. */
    double OwnY[2] = { };
    };

/** Coordinate types. */
enum class CoordType
    {
    /** Longitude (x) and latitude (y) in degrees. */
    Degree,
    /** Pixels on the display: x increases to the right and y increases downwards. */
    Display,
    /** A synonym for Display: pixels on the display: x increases to the right and y increases downwards.. */
    Screen = Display,
    /** Map coordinates: projected meters, 32nds of meters or 64ths of meters, depending on the map. */
    Map,
    /** Map meters: projected meters, not correcting for distortion introduced by the projection. */
    MapMeter
    };

/** A line segment in two-dimensional space. */
class Line
    {
    public:
    /** The start of the line segment. */
    Point Start;
    /** The end of the line segment. */
    Point End;
    };

/** Types used by OutlinePoint. */
enum class PointType
    {
    /** A point on the curve. */
    OnCurve,
    /** A control point for a quadratic (conic) Bezier spline curve. */
    Quadratic,
    /** A control point for a cubic Bezier spline curve. */
    Cubic
    };

/** A point on a path. Paths are made from straight line segments and Bezier splines. */
class OutlinePoint: public Point
    {
    public:
    /** Create an on-curve outline point with the coordinates (0,0). */
    constexpr OutlinePoint() noexcept { }
    /** Create an on-curve outline point from a Point. */
    constexpr OutlinePoint(const Point& aPoint) noexcept : CartoTypeCore::Point(aPoint) { }
    /** Create an outline point from a Point and a type. */
    constexpr OutlinePoint(const Point& aPoint,PointType aType) noexcept : CartoTypeCore::Point(aPoint), Type(aType) { }
    /** Create an on-curve outline point, specifying coordinates. */
    constexpr OutlinePoint(int32_t aX,int32_t aY) noexcept: CartoTypeCore::Point(aX,aY) { }
    /** Create an outline point, specifying coordinates and type. */
    constexpr OutlinePoint(int32_t aX,int32_t aY,PointType aType) noexcept : CartoTypeCore::Point(aX,aY), Type(aType) { }
    /** The equality operator. */
    constexpr bool operator==(const OutlinePoint& aPoint) const noexcept { return X == aPoint.X && Y == aPoint.Y && Type == aPoint.Type; }
    /** The inequality operator. */
    constexpr bool operator!=(const OutlinePoint& aPoint) const noexcept { return !(*this == aPoint); }
    /** Create a Point by ignoring the point type. */
    Point Point() const noexcept { return { X, Y }; }

    /** The type of an outline point: on-curve, or a quadratic or cubic control point. */
    PointType Type { PointType::OnCurve };
    };

/** A point on a path, using floating-point coordinates. Paths are made from straight line segments and Bezier splines. */
class OutlinePointFP: public PointFP
    {
    public:
    /** Create an on-curve outline point with the coordinates (0,0). */
    constexpr OutlinePointFP() noexcept { }
    /** The copy constructor. */
    constexpr OutlinePointFP(const OutlinePointFP& aPoint) = default;
    /** Create an on-curve outline point from a Point. */
    constexpr OutlinePointFP(const Point& aPoint) noexcept : PointFP(aPoint) { }
    /** Create an outline point from an OutlinePoint. */
    constexpr OutlinePointFP(const OutlinePoint& aPoint) noexcept : PointFP(aPoint), Type(aPoint.Type) { }
    /** Create an outline point from a Point and a type. */
    constexpr OutlinePointFP(const Point& aPoint,PointType aType) noexcept: PointFP(aPoint), Type(aType) { }
    /** Create an on-curve outline point from a Point. */
    constexpr OutlinePointFP(const PointFP& aPoint) noexcept: PointFP(aPoint) { }
    /** Create an outline point from a Point and a type. */
    constexpr OutlinePointFP(const PointFP& aPoint,PointType aType) noexcept: PointFP(aPoint), Type(aType) { }
    /** Create an on-curve outline point, specifying coordinates. */
    constexpr OutlinePointFP(double aX,double aY) noexcept: PointFP(aX,aY) { }
    /** Create an outline point, specifying coordinates and type. */
    constexpr OutlinePointFP(double aX,double aY,PointType aType) noexcept: PointFP(aX,aY), Type(aType) { }
    /** The equality operator. */
    constexpr bool operator==(const OutlinePointFP& aPoint) const noexcept { return X == aPoint.X && Y == aPoint.Y && Type == aPoint.Type; }
    /** The inequality operator. */
    constexpr bool operator!=(const OutlinePointFP& aPoint) const noexcept { return !(*this == aPoint); }

    /** The type of an outline point: on-curve, or a quadratic or cubic control point. */
    PointType Type { PointType::OnCurve };
    };

/**
A track point, used when recording points traversed. It combines a point, which by convention is in degrees of
longitude and latitude, with a timestamp.
@see TrackGeometry
*/
class TrackPoint: public PointFP
    {
    public:
    /** Creates a trackpoint with zero coordinates and a timestamp of zero indicating that the time is unknown. */
    TrackPoint() = default;
    /** Creates a track point from a point, giving it a timestamp of zero indicating that the time is unknown. */
    TrackPoint(const PointFP& aPoint): PointFP(aPoint) { }

    /** If non-zero, the time in seconds: the number of seconds since 00:00 on the 1st January 1970, UTC. */
    double Time = 0;
    };

/**
A rectangle in two-dimensional space, aligned with the coordinate system and defined by
opposite corners with minimum and maximum coordinates.
*/
class Rect
    {
    public:
    /** Creates an empty rectangle with both corners at the point (0,0). */
    Rect() { }
    /** Creates a rectangle with the specified edges. */
    Rect(int32_t aMinX,int32_t aMinY,int32_t aMaxX,int32_t aMaxY) :
        Min(aMinX,aMinY),
        Max(aMaxX,aMaxY)
        {
        }
    explicit Rect(const RectFP& aRectFP) noexcept;
    /** The equality operator. */
    bool operator==(const Rect& aRect) const { return Min == aRect.Min && Max == aRect.Max; }
    /** The inequality operator. */
    bool operator!=(const Rect& aRect) const { return !(*this == aRect); }
    /** Returns the minimum X coordinate. */
    int32_t MinX() const { return Min.X; }
    /** Returns the minimum Y coordinate. */
    int32_t MinY() const { return Min.Y; }
    /** Returns the maximum X coordinate.. */
    int32_t MaxX() const { return Max.X; }
    /** Returns the maximum Y coordinate.. */
    int32_t MaxY() const { return Max.Y; }
    /**
    Returns true if the rectangle is empty, defined as having a width or height
    less than or equal to zero.
    */
    bool IsEmpty() const { return MinX() >= MaxX() || MinY() >= MaxY(); }
    /**
    Returns true if the rectangle is maximal, defined as having minimum coords of INT32_MIN
    and maximum coords of INT32_MAX.
    */
    bool IsMaximal() const { return Min.X == INT32_MIN && Min.Y == INT32_MIN && Max.X == INT32_MAX && Max.Y == INT32_MAX; }
    /** Returns the width. */
    int32_t Width() const { return MaxX() - MinX(); }
    /** Returns the height. */
    int32_t Height() const { return MaxY() - MinY(); }
    /** Returns the point (Max.X,Min.Y). */
    Point MaxMin() const { return Point(Max.X,Min.Y); }
    /** Returns the point (Min.X,Max.Y). */
    Point MinMax() const { return Point(Min.X,Max.Y); }
    /** Returns the center. */
    Point Center() const { return Point((Min.X + Max.X) / 2,(Min.Y + Max.Y) / 2); }
    /**
    Returns true if the rectangle contains the point, where containment is defined
    using half-open intervals: the rectangle includes points on its minimum X and
    Y edges but not its maximum X and Y edges.
    */
    bool Contains(const Point& aPoint) const
        { return aPoint.X >= Min.X && aPoint.Y >= Min.Y &&
                 aPoint.X < Max.X && aPoint.Y < Max.Y; }
    /** Returns true if the rectangle contains another rectangle. */
    bool Contains(const Rect& aRect) const
        {
        return Min.X <= aRect.Min.X &&
               Min.Y <= aRect.Min.Y &&
               Max.X >= aRect.Max.X &&
               Max.Y >= aRect.Max.Y;
        }
    bool IsOnEdge(const Point& aPoint) const noexcept;
    bool Intersects(const Rect& aRect) const noexcept;
    bool Intersects(const Point& aStart,const Point& aEnd,
                    Point* aIntersectionStart = nullptr,Point* aIntersectionEnd = nullptr) const noexcept;
    void Intersection(const Rect& aRect) noexcept;
    void Combine(const Rect& aRect) noexcept;
    void Combine(const Point& aPoint) noexcept;

    /** The corner with minimum X and Y coordinates. */
    Point Min;
    /** The corner with maximum X and Y coordinates. */
    Point Max;
    };

/**
A floating-point rectangle in two-dimensional space, aligned with the coordinate system and defined by
opposite corners with minimum and maximum coordinates.
*/
class RectFP
    {
    public:
    /** Creates an empty rectangle with both corners at the point (0,0). */
    RectFP() { }
    /** Creates a rectangle with the specified edges. */
    RectFP(double aMinX,double aMinY,double aMaxX,double aMaxY):
        Min(aMinX,aMinY),
        Max(aMaxX,aMaxY)
        {
        }
    /** Creates a rectangle from an integer rectangle. */
    RectFP(const Rect& aRect):
        Min(aRect.Min),
        Max(aRect.Max)
        {
        }
    /** The equality operator. */
    bool operator==(const RectFP& aRect) const { return Min == aRect.Min && Max == aRect.Max; }
    /** The inequality operator. */
    bool operator!=(const RectFP& aRect) const { return !(*this == aRect); }
    /** The less-than operator. */
    bool operator<(const RectFP& aRect) const { return Min < aRect.Min || (Min == aRect.Min && Max < aRect.Max); }
    /** Returns the minimum X coordinate. */
    double MinX() const noexcept { return Min.X; }
    /** Returns the maximum X coordinate. */
    double MinY() const noexcept { return Min.Y; }
    /** Returns the minimum Y coordinate. */
    double MaxX() const noexcept { return Max.X; }
    /** Returns the maximum Y coordinate. */
    double MaxY() const noexcept { return Max.Y; }
    /**
    Return true if the rectangle is empty, defined as having a width or height
    less than or equal to zero.
    */
    bool IsEmpty() const noexcept { return MinX() >= MaxX() || MinY() >= MaxY(); }
    /** Return the width. */
    double Width() const noexcept { return MaxX() - MinX(); }
    /** Return the height. */
    double Height() const noexcept { return MaxY() - MinY(); }
    /** Returns the point (Max.X,Min.Y). */
    PointFP MaxMin() const noexcept { return PointFP(Max.X,Min.Y); }
    /** Returns the point (Min.X,Max.Y). */
    PointFP MinMax() const noexcept { return PointFP(Min.X,Max.Y); }
    /** Return the center. */
    PointFP Center() const noexcept { return PointFP((Min.X + Max.X) / 2.0,(Min.Y + Max.Y) / 2.0); }
    /**
    Returns true if the rectangle contains the point, where containment is defined
    using half-open intervals: the rectangle includes points on its minimum X and
    Y edges but not its maximum X and Y edges.
    */
    bool Contains(const PointFP& aPoint) const noexcept
        { return aPoint.X >= Min.X && aPoint.Y >= Min.Y &&
                 aPoint.X < Max.X && aPoint.Y < Max.Y; }
    /** Returns true if the rectangle contains another rectangle. */
    bool Contains(const RectFP& aRect) const noexcept
        {
        return Min.X <= aRect.Min.X &&
               Min.Y <= aRect.Min.Y &&
               Max.X >= aRect.Max.X &&
               Max.Y >= aRect.Max.Y;
        }
    /** Updates a rectangle such that it contains the specified point. */
    void Combine(const PointFP& aPoint)
        {
        if (aPoint.X < Min.X)
            Min.X = aPoint.X;
        if (aPoint.Y < Min.Y)
            Min.Y = aPoint.Y;
        if (aPoint.X > Max.X)
            Max.X = aPoint.X;
        if (aPoint.Y > Max.Y)
            Max.Y = aPoint.Y;
        }
    /** Sets a rectangle to the smallest new rectangle that contains itself and aRect. */
    void Combine(const RectFP& aRect)
        {
        if (!aRect.IsEmpty())
            {
            if (IsEmpty())
                *this = aRect;
            else
                {
                if (aRect.Min.X < Min.X)
                    Min.X = aRect.Min.X;
                if (aRect.Min.Y < Min.Y)
                    Min.Y = aRect.Min.Y;
                if (aRect.Max.X > Max.X)
                    Max.X = aRect.Max.X;
                if (aRect.Max.Y > Max.Y)
                    Max.Y = aRect.Max.Y;
                }
            }
        }
    /** Sets a rectangle to its intersection with aRect. */
    void Intersection(const RectFP& aRect)
        {
        if (Min.X < aRect.Min.X)
            Min.X = aRect.Min.X;
        if (Max.X > aRect.Max.X)
            Max.X = aRect.Max.X;
        if (Min.Y < aRect.Min.Y)
            Min.Y = aRect.Min.Y;
        if (Max.Y > aRect.Max.Y)
            Max.Y = aRect.Max.Y;
        }
    /**
    Returns true if the rectangle and aRect have an intersection.
    If both rectangles are non-empty, returns true only
    if the intersection is non-empy.
    */
    bool Intersects(const RectFP& aRect) const noexcept
        {
        return Min.X < aRect.Max.X &&
               Max.X > aRect.Min.X &&
               Min.Y < aRect.Max.Y &&
               Max.Y > aRect.Min.Y;
        }
    bool Intersects(const PointFP& aStart,const PointFP& aEnd,
                    PointFP* aIntersectionStart = nullptr,PointFP* aIntersectionEnd = nullptr) const noexcept;
    Rect Rounded() const noexcept;

    /** The corner with minimum X and Y coordinates. */
    PointFP Min;
    /** The corner with maximum X and Y coordinates. */
    PointFP Max;
    };

/**
A templated interface class defining dictionaries in
which a key type is used to access a value.
*/
template <class Key,class Value> class MDictionary
    {
    public:
    /**
    Although pointers to this class are not owned,
    a virtual destructor is provided for safety and
    to avoid compiler warnings.
    */
    virtual ~MDictionary() { }

    /**
    Return true if aKey exists, false if not.
    If aKey exists in the dictionary, set aValue to
    the value associated with aKey.
    */
    virtual bool Find(const Key& aKey,Value& aValue) const = 0;
    };

/**
A templated interface class defining dictionaries in
which a key is used to access a pointer to a value.
*/
template <class Key,class Value> class MPointerDictionary
    {
    public:
    /**
    Although pointers to this class are not owned,
    a virtual destructor is provided for safety and
    to avoid compiler warnings.
    */
    virtual ~MPointerDictionary() { }

    /**
    Return true if aKey exists, false if not.
    If aKey exists in the dictionary, set aValue to point to
    the value associated with aKey.
    */
    virtual bool Find(const Key& aKey,const Value*& aValue) const = 0;
    };

/** File types of interest to CartoTypeCore. */
enum class FileType
    {
    /** PNG (Portable Network Graphics) image files. */
    PNG,
    /** JPEG (Joint Photographic Expert Group) image files. */
    JPEG,
    /** TIFF (Tagged Image File Format) image files. */
    TIFF,
    /** CTM1 (CartoTypeCore Map Data Type 1) map data files. */
    CTM1,
    /** CTMS (CartoTypeCore Map Data, Serialized) map data files. */
    CTMS,
    /** KML (Keyhole Markup Language) map data files. */
    KML,
    /** CTSQL (CartoTypeCore SQLite format) map data files. */
    CTSQL,
    /** CTROUTE (CartoTypeCore XML route) files. */
    CTROUTE,
    /** GPX (GPS Exchange) files. */
    GPX,

    /** Unknown or unspecified file type. */
    None = -1
    };

/** A file location: used for returning the location of an XML or other parsing error. */
class FileLocation
    {
    public:
    /** The column number. */
    size_t ColumnNumber { size_t(-1) };
    /** The line number. */
    size_t LineNumber { size_t(-1) };
    /** The byte number, counting from the start of the file. */
    size_t ByteIndex { size_t(-1) };
    };

/** Types of writable maps. */
enum class WritableMapType
    {
    /** A writable map stored in memory. */
    Memory,

    /** A map stored as an SQLite database. */
    SQLite,

    /** Not a writable map type but the number of different writable map types. */
    Count,

    /** A value to use where a null type is needed. */
    None = -1
    };

/** An enumerated type for map objects. */
enum class MapObjectType
    {
    /** A point in two-dimensional space. */
    Point,

    /** A line in two-dimensional space, consisting of one or more open contours. */
    Line,

    /** A polygon in two-dimensional space, consisting of one or more closed contours. */
    Polygon,

    /** Information arranged as an array of values, such as an image or height data. */
    Array,

    /** The total number of map object types. This is not an actual type. */
    Count,

    /** A value to use where a null type is needed. */
    None = -1
    };

/** Values used in the user-data value of objects in the vector tile drawing system. */
enum class VectorTileObjectClipType
    {
    /** This object can be drawn normally, */
    Standard,
    /** This is a polygon that has been clipped and only the interior should be drawn, not the border. */
    Fill,
    /** This is a polyline representing the clipped border of a polygon. */
    Border,
    /** This is a highlight using the first highlight style of a line style. Subsequent highlight styles use the successive values 0x10001, 0x10002, etc.  */
    Highlight = 0x10000
    };

/** Positions for notices like the legend or scale bar. Use ExtendedNoticePosition for greater control. */
enum class NoticePosition
    {
    /** The top left corner of the display. */
    TopLeft,
    /** The top right corner of the display. */
    TopRight,
    /** The bottom left corner of the display. */
    BottomLeft,
    /** The bottom right corner of the display. */
    BottomRight,
    /** The center of the top edge of the display. */
    Top,
    /** The center of the right side of the display. */
    Right,
    /** The center of the left side of the display. */
    Left,
    /** The center of the bottom edge of the display. */
    Bottom,
    /** The center of the display. */
    Center
    };

/**
Positions for notices like the legend or scale bar.

Notices are positioned by moving them inwards from the designated position
by the specified insets. For central positions (e.g., the X inset in Top,
or both insets in Center) X insets move the position right and Y insets move it down.
Units may be 'cm', 'mm', 'in', 'pt' (points: 1/72in), or 'pc' (picas: 1/6in);
anything else, including null, means pixels.

The default inset is 3mm, or zero for central positions.
*/
class ExtendedNoticePosition
    {
    public:
    ExtendedNoticePosition(NoticePosition aBasePosition);
    ExtendedNoticePosition(NoticePosition aBasePosition,double aXInset,const char* aXInsetUnit,double aYInset,const char* aYInsetUnit);
    /** Returns the base position. */
    NoticePosition BasePosition() const { return m_base_position; }
    /** Returns the X (horizontal) inset. */
    double XInset() const { return m_x_inset; }
    /** Returns the unit (e.g., "mm") used for the X inset. */
    const char* XInsetUnit() const { return m_x_inset_unit.data(); }
    /** Returns the Y (vertical) inset. */
    double YInset() const { return m_y_inset; }
    /** Returns the unit (e.g., "mm") used for the Y inset. */
    const char* YInsetUnit() const { return m_y_inset_unit.data(); }

    private:
    NoticePosition m_base_position = NoticePosition::TopLeft;
    double m_x_inset = 0;
    std::array<char,3> m_x_inset_unit = { };
    double m_y_inset = 0;
    std::array<char,3> m_y_inset_unit = { };
    };

/** A holder for arbitrary data. */
class MUserData
    {
    public:
    virtual ~MUserData() { }
    };

/**
The maximum length for a map label in characters.
The large size allows the use of complex multi-line labels.
There has to be a fixed maximum length because of the fixed size
of the glyph cache (see CEngine::EGlyphLogEntries).
*/
constexpr int32_t KMaxLabelLength = 1024;

/**
A factor to convert radians to metres for the Mercator and other projections.
It is the semi-major axis (equatorial radius) used by the WGS 84 datum (see http://en.wikipedia.org/wiki/WGS84).
*/
constexpr int32_t KEquatorialRadiusInMetres = 6378137;

/** The flattening constant used by the WGS84 reference ellipsoid. */
constexpr double KWGS84Flattening = 1.0 / 298.257223563;

/**
A factor to convert radians to metres for the Mercator and other projections.
It is the semi-major axis (equatorial radius) used by the WGS 84 datum (see http://en.wikipedia.org/wiki/WGS84).
A synonym of KEquatorialRadiusInMetres.
*/
constexpr double KRadiansToMetres = KEquatorialRadiusInMetres;
/** The value of pi as a double-precision floating-point number. */
constexpr double KPiDouble = 3.1415926535897932384626433832795;
/** The value of pi as a single-precision floating-point number. */
constexpr float KPiFloat = 3.1415926535897932384626433832795f;
/** A factor to convert radians to degrees: the number of degrees in a radian. */
constexpr double KRadiansToDegreesDouble = 180.0 / KPiDouble;
/** A factor to convert degrees to radians: the number of radians in a degree. */
constexpr double KDegreesToRadiansDouble = KPiDouble / 180.0;
/** A factor to convert degrees to metres, using the semi-major axis (equatorial radius) used by the WGS 84 datum. */
constexpr double KDegreesToMetres = KRadiansToMetres * KDegreesToRadiansDouble;
/** A factor to convert scaled degrees, represented in 11.21 fixed-point format, to metres, using the semi-major axis (equatorial radius) used by the WGS 84 datum. */
constexpr double KScaledDegreesToMetres = KDegreesToMetres / 2097152.0;

/**
Finds the great-circle distance in metres, assuming a spherical earth, between two lat-long points in degrees.
@see GreatCircleDistanceInMetersUsingEllipsoid, which is a little slower but uses the WGS84 ellipsoid.
*/
inline double GreatCircleDistanceInMeters(double aLong1,double aLat1,double aLong2,double aLat2)
    {
    aLong1 *= KDegreesToRadiansDouble;
    aLat1 *= KDegreesToRadiansDouble;
    aLong2 *= KDegreesToRadiansDouble;
    aLat2 *= KDegreesToRadiansDouble;
    double cos_angle = std::sin(aLat1) * std::sin(aLat2) + std::cos(aLat1) * std::cos(aLat2) * std::cos(aLong2 - aLong1);

    /*
    Inaccurate trig functions can cause cos_angle to be a tiny amount greater than 1 if the two positions are very close.
    That in turn causes acos to gives a domain error and return the special floating point value -1.#IND000000000000,
    meaning 'indefinite', which becomes INT32_MIN when converted to an integer. Observed on 64-bit Windows. See case 1496.
    We fix the problem by means of the following check.
    */
    if (cos_angle >= 1)
        return 0;

    double angle = std::acos(cos_angle);
    return angle * KEquatorialRadiusInMetres;
    }

/** Reverses an array of objects of type T. */
template<class T> inline void Reverse(T* aStart,size_t aLength)
    {
    if (aLength > 1)
        {
        T* end = aStart + aLength - 1;
        while (aStart < end)
            {
            T temp = *aStart;
            *aStart = *end;
            *end = temp;
            aStart++;
            end--;
            }
        }
    }

/** Returns the area of a polygon made of points with members X and Y. */
template<class point_t> inline double Area(const point_t* aPointArray,size_t aPointCount)
    {
    if (aPointCount < 3)
        return 0;
    double area = 0;
    const point_t* p = aPointArray;
    const point_t* end = aPointArray + aPointCount;
    const point_t* prev = end - 1;
    double origin_x = prev->X;
    double origin_y = prev->Y;
    double prev_x = 0;
    double prev_y = 0;
    while (p < end)
        {
        double x = p->X - origin_x;
        double y = p->Y - origin_y;
        area += (x + prev_x) * (y - prev_y);
        prev_x = x;
        prev_y = y;
        p++;
        }
    return area / 2.0;
    }

/** Returns the area of a polygon represented as a vector of points with members X and Y. */
template<class contour_t> inline double Area(const contour_t& aContour)
    {
    return Area(aContour.data(),aContour.size());
    }

double SphericalPolygonArea(const CoordSet& aCoordSet) noexcept;
double SphericalPolylineLength(const CoordSet& aCoordSet) noexcept;
double SphericalPolygonArea(std::function<const PointFP*()> aNextPoint);
double SphericalPolylineLength(std::function<const PointFP* ()> aNextPoint);
void GetSphericalAreaAndLength(std::function<const PointFP* ()> aNextPoint,bool aIsPolyline,double* aArea,double* aLength);
void GetSphericalAreaAndLength(const CoordSet& aCoordSet,bool aIsPolyline,double* aArea,double* aLength) noexcept;
double AzimuthInDegrees(double aLong1,double aLat1,double aLong2,double aLat2) noexcept;
PointFP PointAtAzimuth(const PointFP& aPoint,double aDir,double aDistanceInMetres) noexcept;
double GreatCircleDistanceInMetersUsingEllipsoid(double aLong1,double aLat1,double aLong2,double aLat2) noexcept;

/** The standard number of levels of the text index to load into RAM when loading a CTM1 file. */
constexpr int32_t KDefaultTextIndexLevels = 1;

class MapObject;
/** A type for arrays of map objects returned by search functions. */
using MapObjectArray = std::vector<std::unique_ptr<MapObject>>;
/** A type for internal functions to handle objects returned by search functions. */
using FindHandler = std::function<bool(std::unique_ptr<MapObject>)>;

/** Reads an 8-bit integer from aP. Used by InterpolatedValue, which requires a function of this name even when endianness is irrelevant. */
inline uint8_t ReadBigEndian(const uint8_t* aP)
    {
    return *aP;
    }
/** Writes an 8-bit integer to aP. Used by InterpolatedValue, which requires a function of this name even when endianness is irrelevant. */
inline void WriteBigEndian(uint8_t* aP,uint8_t aValue)
    {
    *aP = aValue;
    }
/** Reads a big-endian 16-bit integer from aP. */
inline int16_t ReadBigEndian(const int16_t* aP)
    {
    return (int16_t)( (((const uint8_t*)aP)[0] << 8) | ((const uint8_t*)aP)[1] );
    }
/** Writes a big-endian 16-bit integer to aP. */
inline void WriteBigEndian(int16_t* aP,int16_t aValue)
    {
    ((uint8_t*)aP)[0] = (uint8_t)(aValue >> 8);
    ((uint8_t*)aP)[1] = (uint8_t)aValue;
    }

/**
Uses bilinear interpolation to get the value at (aX,aY) from a rectangular table of data.
Each data item has aChannels channels, and each channel takes up aWidth * sizeof(DataType) bytes.
The value aUnknownValue is ignored when interpolating.
*/
template<class DataType> double InterpolatedValue(const DataType* aDataStart,int32_t aWidth,int32_t aHeight,int32_t aStride,int32_t aChannels,
                                                  double aX,double aY,int aChannel,int32_t aUnknownValue)
    {
    double x_fraction = 1 - (aX - std::floor(aX));
    if ((int)aX == aWidth - 1)
        x_fraction = 1;
    double y_fraction = 1 - (aY - std::floor(aY));
    if ((int)aY == aHeight - 1)
        y_fraction = 1;
    int index = (int)aY * aWidth * aChannels + (int)aX * aChannels + aChannel;
    double top_value = ReadBigEndian(aDataStart + index);
    if (x_fraction < 1)
        {
        double top_right_value = ReadBigEndian(aDataStart + index + aChannels);
        if (top_right_value != aUnknownValue)
            top_value = top_value * x_fraction + top_right_value * (1.0 - x_fraction);
        }
    double value = top_value;
    if (y_fraction < 1 && value != aUnknownValue)
        {
        index += aStride * aChannels;
        double bottom_value = ReadBigEndian(aDataStart + index);
        if (x_fraction < 1)
            {
            double bottom_right_value = ReadBigEndian(aDataStart + index + aChannels);
            if (bottom_right_value != aUnknownValue)
                bottom_value = bottom_value * x_fraction + bottom_right_value * (1.0 - x_fraction);
            }
        if (bottom_value != aUnknownValue)
            value = top_value * y_fraction + bottom_value * (1.0 - y_fraction);
        }
    return value;
    }

/** The minimum legal map scale denominator. */
constexpr double KMinScaleDenominator = 100;

/** The maximum legal map scale denominator. */
constexpr double KMaxScaleDenominator = 1000000000;

/** A class to create CRCs (cyclical redundancy check hash values). */
class CRCGenerator
    {
    public:
    /** Creates a CRC generator. */
    CRCGenerator();
    /** Generates a CRC from a block of data using a specified start value, which may be the CRC of a previous block. */
    uint32_t Generate(uint32_t aStartValue,const uint8_t* aData,size_t aLength) const;

    private:
    std::array<uint32_t,256> m_table;
    };

} // namespace CartoTypeCore

#ifdef _WIN32_WCE
#ifdef __cplusplus
extern "C" {
#endif
char* getenv(const char* name);
void abort(void);
#ifdef __cplusplus
}
#endif
#endif
