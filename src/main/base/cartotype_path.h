/*
cartotype_path.h
Copyright (C) 2013-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_types.h>
#include <cartotype_stream.h>
#include <algorithm>
#include <utility>

namespace CartoTypeCore
{
class MContour;
class Contour;
class ContourView;
class Outline;
class CEngine;
class MapObject;
class CircularPen;
class Projection;
class ClipRegion;
class AffineTransform;

/** A type to label different relationships a clip rectangle has with a path, to decide what sort of clipping is needed. */
enum class ClipType
    {
    /** The path is completely inside the clip rectangle. */
    Inside,
    /** The path is not completely inside the clip rectangle, and has no curves. */
    MayIntersectAndHasNoCurves,
    /** The path is not completely inside the clip rectangle, and has curves. */
    MayIntersectAndHasCurves
    };

/** Types of clipping done by the general clip function Outline MPath::Clip(ClipOperation aClipOperation,const MPath& aClip) const. */
enum class ClipOperation // must be semantically the same as ClipperLib::ClipType
    {
    /** Returns the intersection of two paths; commutative. */
    Intersection,
    /** Returns the union of two paths; commutative. */
    Union,
    /** Returns the difference of two paths; non-commutative. */
    Difference,
    /** Returns the exclusive-or of the two paths; that is, any regions which are in neither path; commutative. */
    Xor
    };

/**
Traverses a sequence of Point objects, extracting lines and curves and calling functions to process them.

The functions called are

To start a new contour, move to aPoint without drawing a line and set the current point to aPoint:

void MoveTo(const Point& aPoint)

To draw a line from the current point to aPoint and set the current point to aPoint:

void LineTo(const Point& aPoint)
*/
template<class MTraverser> void Traverse(MTraverser& aTraverser,const Point* aPoint,size_t aPoints,bool aClosed)
    {
    if (aPoints < 2)
        return;

    const Point* point = aPoint;
    const Point* last = aPoint + aPoints - 1;
    aTraverser.MoveTo(*point++);
    while (point <= last)
        aTraverser.LineTo(*point++);
    if (aClosed && *aPoint != *last)
        aTraverser.LineTo(*aPoint);
    }

/**
Traverses a sequence of outline point objects, extracting lines and curves and calling functions to process them.

The functions called are

To start a new contour, move to aPoint without drawing a line and set the current point to aPoint:

void MoveTo(const Point& aPoint)

To draw a line from the current point to aPoint and set the current point to aPoint:

void LineTo(const Point& aPoint)

To draw a quadratic spline from the current point to aPoint2, using aPoint1 as the off-curve control point,
and set the current point to aPoint2:

void QuadraticTo(const Point& aPoint1,const Point& aPoint2)

To draw a cubic spline from the current point to aPoint3, using aPoint1 and aPoint2 as the
off-curve control points, and set the current point to aPoint3:

void CubicTo(const Point& aPoint1,const Point& aPoint2,const Point& aPoint3)
*/
template<class traverser_t,class point_t> void Traverse(traverser_t& aTraverser,const point_t* aPoint,size_t aPoints,bool aClosed)
    {
    if (aPoints < 2)
        return;

    // A contour cannot start with a cubic control point.
    assert(aPoint->Type != PointType::Cubic);

    size_t last = aPoints - 1;
    const point_t* limit = aPoint + last;
    point_t v_start = aPoint[0];
    point_t v_last = aPoint[last];
    const point_t* point = aPoint;

    /* Check first point to determine origin. */
    if (point->Type == PointType::Quadratic)
        {
        /* First point is conic control. Yes, this happens. */
        if (aPoint[last].Type == PointType::OnCurve)
            {
            /* start at last point if it is on the curve */
            v_start = v_last;
            limit--;
            }
        else
            {
            /*
            If both first and last points are conic,
            start at their middle and record its position
            for closure.
            */
            v_start.X = (v_start.X + v_last.X) / 2;
            v_start.Y = (v_start.Y + v_last.Y) / 2;
            v_last = v_start;
            }
        point--;
        }

    aTraverser.MoveTo(v_start);

    while (point < limit)
        {
        point++;
        switch (point->Type)
            {
            case PointType::OnCurve:
                aTraverser.LineTo(*point);
                continue;
                
            case PointType::Quadratic:
                {
                const point_t* v_control = point;
                while (point < limit)
                    {
                    point++;
                    const point_t* cur_point = point;
                    if (point->Type == PointType::OnCurve)
                        {
                        aTraverser.QuadraticTo(*v_control,*cur_point);
                        break;
                        }
                    if (point->Type != PointType::Quadratic)
                        return; // invalid outline
                    point_t v_middle((v_control->X + cur_point->X) / 2,(v_control->Y + cur_point->Y) / 2);
                    aTraverser.QuadraticTo(*v_control,v_middle);
                    v_control = cur_point;
                    }
                if (aPoint == limit)
                    {
                    aTraverser.QuadraticTo(*v_control,v_start);
                    return;
                    }
                }
                continue;

            default: // cubic
                {
                if (point + 1 > limit || point->Type != PointType::Cubic)
                    return; // invalid outline
                const point_t* vec1 = point++;
                const point_t* vec2 = point++;
                if (point <= limit)
                    {
                    aTraverser.CubicTo(*vec1,*vec2,*point);
                    continue;
                    }
                aTraverser.CubicTo(*vec1,*vec2,v_start);
                return;
                }
            }
        }

    // Close the contour with a line segment.
    if (aClosed && v_last != v_start)
        aTraverser.LineTo(v_start);
    }

/** An interface class to transform a point. */
class MPointTransformer
    {
    public:
    /** A virtual function to transform a point representing pixels using integers with a certain number of fractional bits. */
    virtual DrawResult Transform(Point& aPoint,int32_t aFractionalBits) = 0;
    };

/** The ways two paths can intersect. */
enum class PathIntersectionType
    {
    /** The intersection type is unknown. */
    Unknown,
    /** The paths do not intersect. */
    Separate,
    /** The paths intersect or are identical. */
    Intersects,
    /** The first path contains the second. */
    Contains,
    /** The second path contains the first. */
    Contained
    };

/** Information about the intersection of two paths and their distance apart. */
class PathIntersectionInfo
    {
    public:
    /** The intersection type. */
    PathIntersectionType Type = PathIntersectionType::Unknown;
    /**
    The distance between the paths. The units are meters for functions involving
    map objects, otherwise they are the units used by the paths.
    */
    double Distance = 0;
    /** 
    The nearest point on the first path. The units are latitude and longitude for functions involving
    map objects, otherwise they are the units used by the paths.
    */
    PointFP Nearest1;
    /**
    The nearest point on the second path. The units are latitude and longitude for functions involving
    map objects, otherwise they are the units used by the paths.
    */
    PointFP Nearest2;
    };

/**
Path objects, which are sequences of contours, must implement the
MPath interface class.
*/
class MPath
    {
    public:
    /**
    A virtual destructor: needed in case paths returned by ClippedPath
    are not the same as the path passed in and must therefore be deleted by the caller.
    */
    virtual ~MPath() { }

    /** Returns the number of contours. */
    virtual size_t Contours() const = 0;

    /** Returns the contour indexed by aIndex. */
    virtual ContourView ContourByIndex(size_t aIndex) const = 0;

    /** Returns true if the path may have off-curve points. */
    virtual bool MayHaveCurves() const = 0;

    template<class MPathTraverser> void Traverse(MPathTraverser& aTraverser,const Rect& aClip) const;
    template<class MPathTraverser> void Traverse(MPathTraverser& aTraverser,const Rect* aClip = nullptr) const;

    bool operator==(const MPath& aOther) const;
    Rect CBox() const;
    bool CBoxBiggerThan(int32_t aSize) const;
    bool IsContainedIn(const Rect& aRect) const;
    bool Contains(double aX,double aY) const;
    /** Returns true if this path contains aPoint. */
    bool Contains(const Point& aPoint) const { return Contains(aPoint.X,aPoint.Y); }
    /** Returns true if this path contains aPoint. */
    bool Contains(const PointFP& aPoint) const { return Contains(aPoint.X,aPoint.Y); }
    /** Returns true if this path contains aPath. */
    bool MayIntersect(const Rect& aRect) const;
    bool MayIntersect(const Rect& aRect,int32_t aBorder) const;
    bool Intersects(const Rect& aRect) const;
    bool Intersects(const MPath& aPath,const Rect* aBounds = nullptr) const;
    int32_t MaxDistanceFromOrigin() const;
    PathIntersectionType IntersectionType(const MPath& aOther,double* aDistance = nullptr,PointFP* aNearest1 = nullptr,PointFP* aNearest2 = nullptr) const;
    PathIntersectionInfo IntersectionInfo(const MPath& aOther) const;
    double DistanceFrom(const MPath& aOther,PointFP* aNearest1 = nullptr,PointFP* aNearest2 = nullptr,PathIntersectionType* aType = nullptr) const;
    double DistanceFromPoint(const PointFP& aPoint,PointFP* aNearest = nullptr,size_t* aContourIndex = nullptr,size_t* aLineIndex = nullptr,double* aFractionaLineIndex = nullptr,bool aTreatAsOpen = false) const;
    bool IsClippingNeeded(const Rect& aClip) const;
    Outline Copy() const;
    Outline ClippedPath(const Rect& aClip) const;
    Outline ClippedPath(const MPath& aClip) const;
    Outline ClippedPath(const ClipRegion& aClip) const;
    Outline Clip(ClipOperation aClipOperation,const MPath& aClip) const;
    Outline Envelope(double aOffset) const;
    bool IsSmoothingNeeded() const;
    Outline SmoothPath() const;
    Outline FlatPath(double aMaxDistance) const;
    Outline TruncatedPath(double aStart,double aEnd) const;
    Outline OffsetPath(double aOffset) const;
    Outline TransformedPath(const AffineTransform& aTransform) const;
    Outline TransformedPath(DrawResult& aError,MPointTransformer& aTransformer,int32_t aFractionalBits) const;
    std::array<Line,3> HorizontalPaths(int32_t aPathLength,int32_t aLabelHeight,const PointFP& aUpVector,const Rect* aBounds,const Rect* aClip) const;
    PointFP CenterOfGravity() const;
    void GetCenterOfGravity(Point& aCenter) const;
    double Length() const;
    double Area() const;
    PointFP PointAtLength(double aPos) const;
    std::pair<Point,bool> End() const;
    void Write(DataOutputStream& aOutput) const;
    bool IsEmpty() const;
    bool IsPoint() const;
    bool IsGridOrientedRectangle(Rect* aRect = nullptr) const;
    void GetSphericalAreaAndLength(const Projection& aProjection,double* aArea,double* aLength) const;
    CartoTypeCore::ClipType ClipType(const Rect& aRect) const;
    bool HasCurves() const;

    /** A constant iterator class to iterate over the contours of a path; used by begin() and end(). */
    class ConstIter
        {
        public:
        /** Creates a constant iterator over a path, starting at contour number aIndex. */
        ConstIter(const MPath& aPath,size_t aIndex):
            m_path(aPath),
            m_index(aIndex)
            {
            }
        /** Increments this iterator to refer to the next contour. */
        ConstIter operator++() { m_index++; return *this; }
        /** The inequality operator. */
        bool operator!=(const ConstIter& aOther) const { return m_index != aOther.m_index; }
        ContourView operator*() const;

        private:
        const MPath& m_path;
        size_t m_index;
        };

    /** Returns a constant iterator positioned at the start of the contours. */
    ConstIter begin() const { return ConstIter(*this,0); }
    /** Returns a constant iterator positioned at the end of the contours. */
    ConstIter end() const { return ConstIter(*this,Contours()); }

    private:
    double LengthHelper(double aPos,PointFP* aPoint) const;
    bool SmoothPathHelper(Outline* aNewPath) const;
    };

/**
Flags used for appending ellipses to determine the angle between
start and end point or start and end angle.
*/
enum class EllipseAngleType
    {
    /** Select the shortest arc between start and end. */
    Shortest,
    /** Select the longest arc between start and end. */
    Longest,
    /** Select the arc from start to end in positive direction. */
    Positive,
    /** Select the arc from start to end in negative direction. */
    Negative
    };

/**
An MContour is an abstract interface to a sequence of integer points representing a line or polygon. The points may be on-curve or off-curve.
Off-curve points are control points for quadratic or cubic spline curves. A contour may be open or closed.
*/
class MContour: public MPath
    {
    public:
    /** Returns the number of points in the contour. */
    virtual size_t Points() const = 0;
    /** Returns a point selected by its index. */
    virtual OutlinePoint Point(size_t aIndex) const = 0;
    /** Returns true if this is a closed contour. */
    virtual bool Closed() const = 0;

    operator ContourView() const;

    // virtual function from MPath
    size_t Contours() const { return 1; };

    /** Returns the last point of the contour. */
    OutlinePoint LastPoint() const { return Point(Points() - 1); }
    OutlinePoint FractionalPoint(double aIndex) const;
    bool IsGridOrientedRectangle(Rect* aRect = nullptr) const;
    bool Anticlockwise() const;
    bool Contains(double aX,double aY) const;
    /** Returns true if this contour contains aPoint. */
    bool Contains(const CartoTypeCore::Point& aPoint) const { return Contains(aPoint.X,aPoint.Y); }
    Outline ClippedContour(const Rect& aClip) const;
    void AppendClippedContour(Outline& aDest,const Rect& aClip) const;
    size_t AppendSplitContour(Outline& aDest,const PointFP& aLineStart,const PointFP& aLineVector);
    Contour TruncatedContour(double aStart,double aEnd) const;
    Contour SubContourBetweenFractionalPoints(double aStartIndex,double aEndIndex) const;
    Contour SubContourBetweenNearestPoints(const PointFP* aStartPoint,const PointFP* aEndPoint) const;
    Contour CentralPath(std::shared_ptr<CEngine> aEngine,const Rect& aClip,bool aFractionalPixels,
                         Line& aFallbackLine,bool aFallbackMustBeHorizontal) const;
    Contour Smooth(double aRadius) const;
    bool MayIntersect(const Rect& aRect) const;
    bool Intersects(const Rect& aRect) const;
    PathIntersectionType IntersectionType(const MContour& aOther,double* aDistance = nullptr,PointFP* aNearest1 = nullptr,PointFP* aNearest2 = nullptr) const;
    PathIntersectionInfo IntersectionInfo(const MContour& aOther) const;
    double DistanceFrom(const MContour& aOther,PointFP* aNearest1 = nullptr,PointFP* aNearest2 = nullptr,PathIntersectionType* aType = nullptr) const;
    double DistanceFromPoint(const PointFP& aPoint,PointFP* aNearest = nullptr,
                             double* aNearestLength = nullptr,bool* aLeft = nullptr,size_t* aLineIndex = nullptr,
                             double* aFractionalLineIndex = nullptr,
                             bool aTreatAsOpen = false) const;
    PointFP PointAtLength(double aLength,double aOffset = 0,int32_t* aLineIndex = nullptr) const;
    CartoTypeCore::Point Orientation(const CartoTypeCore::Point& aCenter) const;
    void GetPrincipalAxis(PointFP& aCenter,PointFP& aVector) const;
    void Write(DataOutputStream& aOutput) const;
    void GetAngles(double aDistance,double& aStartAngle,double& aEndAngle);

    private:
    void GetLongestLineIntersection(const CartoTypeCore::Point& aLineStart,const CartoTypeCore::Point& aLineEnd,CartoTypeCore::Point& aStart,CartoTypeCore::Point& aEnd,const Rect& aClip) const;
    };

/**
A ContourView is a concrete class implementing MContour and designed to be returned
by implementations of MPath. 
*/
class ContourView: public MContour
    {
    public:
    /** Creates an empty contour. */
    ContourView()
       {
       }

    /** Creates a contour from an array of OutlinePoint objects. */
    ContourView(const OutlinePoint* aPoint,size_t aPoints,bool aClosed):
        m_point(aPoint),
        m_points(aPoints),
        m_closed(aClosed),
        m_has_off_curve_points(true)
        {
        }

    /** Creates a contour from an array of Point objects. */
    ContourView(const CartoTypeCore::Point* aPoint,size_t aPoints,bool aClosed):
        m_point(aPoint),
        m_points(aPoints),
        m_closed(aClosed),
        m_has_off_curve_points(false)
        {
        }

    // Virtual functions from MPath.
    ContourView ContourByIndex(size_t /*aIndex*/) const override { return *this; }
    bool MayHaveCurves() const override { return m_has_off_curve_points; }

    // virtual functions from MContour
    size_t Points() const override { return m_points; }
    OutlinePoint Point(size_t aIndex) const override
        {
        assert(aIndex < m_points);
        if (m_has_off_curve_points) return ((const OutlinePoint*)m_point)[aIndex];
        return OutlinePoint(m_point[aIndex]);
        }
    bool Closed() const override { return m_closed; }

    /** Returns a sub-contour of this contour, starting at the point indexed by aStart, and containing aPoints points. */
    ContourView SubContour(size_t aStart,size_t aPoints)
        {
        assert(aStart <= m_points && aStart + aPoints <= m_points);
        ContourView c = *this;
        c.m_points = aPoints;
        if (m_has_off_curve_points)
            c.m_point = (CartoTypeCore::Point*)((OutlinePoint*)m_point + aStart);
        else
            c.m_point += aStart;
        return c;
        }
    /** Returns a pointer to start of the Point data if this contour has only on-curve points, otherwise returns null. */
    const CartoTypeCore::Point* PointData() const { return m_has_off_curve_points ? nullptr : m_point; }
    /** Returns a pointer to start of the OutlinePoint data if this contour may have off-curve points, otherwise returns null. */
    const OutlinePoint* OutlinePointData() const { return m_has_off_curve_points ? (const OutlinePoint*)m_point : nullptr; }

    template<class MTraverser> void Traverse(MTraverser& aTraverser,const Rect* aClip = nullptr) const;

    /** An iterator to traverse the points of a contour. */
    class Iter
        {
        public:
        /** Creates an iterator over a sequence of points in a contour. */
        Iter(const CartoTypeCore::Point* aPoint,size_t aIndex,bool aHasOffCurvePoints):
            m_ptr(aHasOffCurvePoints ? ((OutlinePoint*)aPoint) + aIndex : aPoint + aIndex),
            m_has_off_curve_points(aHasOffCurvePoints)
            {
            }
        /** Increments this iterator to refer to the next point. */
        Iter& operator++() { if (m_has_off_curve_points) m_ptr = ((OutlinePoint*)m_ptr) + 1; else m_ptr++; return *this; }
        /** The inequality operator. */
        bool operator!=(const Iter& aOther) const { return m_ptr != aOther.m_ptr; }
        /** Returns a reference to the point to which this iterator refers. */
        OutlinePoint operator*() const { if (m_has_off_curve_points) return *((OutlinePoint*)m_ptr); else { return OutlinePoint(*m_ptr); } }

        private:
        const CartoTypeCore::Point* m_ptr = nullptr;
        bool m_has_off_curve_points = false;
        };

    /** Returns a constant iterator positioned at the start of the points. */
    Iter begin() const { return Iter(m_point,0,m_has_off_curve_points); }
    /** Returns a constant iterator positioned at the end of the points. */
    Iter end() const { return Iter(m_point,m_points,m_has_off_curve_points); }

    /** An iterator to traverse the lines (sections between successive points) of a contour. */
    class LineIter
        {
        public:
        /** Creates an iterator over the lines in a contour. */
        LineIter(const ContourView& aContour,size_t aIndex):
            m_contour(aContour),
            m_index(aIndex)
            {
            if (m_index == 0)
                {
                if (m_contour.Points() < 2)
                    {
                    m_index = m_contour.Points();
                    return;
                    }
                m_line.Start = m_contour.LastPoint().Point();
                m_line.End = m_contour.Point(0).Point();
                if (!m_contour.Closed() || m_line.Start == m_line.End)
                    ++(*this);
                }
            }
        /** Increments this iterator to refer to the next line. */
        LineIter& operator++() { if (++m_index < m_contour.Points()) { m_line.Start = m_line.End; m_line.End = m_contour.Point(m_index).Point(); } return *this; }
        /** The inequality operator. */
        bool operator!=(const LineIter& aOther) const { return m_index != aOther.m_index; }
        /** Returns a reference to the line to which this iterator refers. */
        const Line& operator*() const { return m_line; }
        /** Returns a pointer to the line to which this iterator refers. */
        const Line* operator->() const { return &m_line; }
        /** Returns the number of lines remaining to be traversed by this iterator. */
        size_t LinesRemaining() const { return m_contour.Points() - m_index; }

        private:
        const ContourView& m_contour;
        size_t m_index;
        Line m_line;
        };

    /** Returns a constant iterator positioned at the start of the lines. */
    LineIter begin_lines() const { return LineIter(*this,0); }
    /** Returns a constant iterator positioned at the end of the lines. */
    LineIter end_lines() const { return LineIter(*this,m_points); }

    private:
    const CartoTypeCore::Point* m_point = nullptr;
    size_t m_points = 0;
    bool m_closed = false;
    bool m_has_off_curve_points = false;
    };

/** Returns a ContourView object representing this contour. */
inline MContour::operator ContourView() const
    {
    return ContourByIndex(0);
    }

/** An interface class for writable contour data. */
class MWritableContour: public MContour
    {
    public:
    /** The assignment operator. */
    MWritableContour& operator=(const MContour& aContour)
        {
        SetClosed(aContour.Closed());
        ReduceSizeTo(0);
        AppendContour(aContour);
        return *this;
        }

    /** Sets a point. */
    virtual void SetPoint(size_t aIndex,const OutlinePoint& aPoint) = 0;
    /** Reduces the number of points to aPoints. The address of the points must not change. */
    virtual void ReduceSizeTo(size_t aPoints) = 0;
    /** Sets the number of points to aPoints. The address of the points may change. */
    virtual void SetSize(size_t aPoints) = 0;
    /** Sets this contour's closed attribute. Does nothing if that is not possible. */
    virtual void SetClosed(bool aClosed) = 0;
    /** Appends a point. */
    virtual void AppendPoint(const OutlinePoint& aPoint) = 0;
    /** Returns a writable pointer to OutlinePoint data if possible. */
    virtual OutlinePoint* OutlinePointData() = 0;
    /** Returns a writable pointer to Point data if possible. */
    virtual CartoTypeCore::Point* PointData() = 0;

    /** Offsets all the points by (aDx,aDy). */
    void Offset(int32_t aDx,int32_t aDy)
        {
        size_t i = 0;
        size_t points = Points();
        while (i < points)
            {
            auto p = Point(i);
            p.X += aDx;
            p.Y += aDy;
            SetPoint(i++,p);
            }
        }

    void AppendContour(const ContourView& aContour);
    /** Sets the last point to aPoint. */
    void SetLastPoint(const OutlinePoint& aPoint) { SetPoint(Points() - 1,aPoint); }
    void Simplify(double aResolutionArea);
    void AppendCircularArc(const CartoTypeCore::Point& aCenter,const CartoTypeCore::Point& aStart,
                           const CartoTypeCore::Point& aEnd,EllipseAngleType aAngleType = EllipseAngleType::Shortest,bool aAppendStart = false);
    void AppendHalfCircle(const CartoTypeCore::Point& aCenter,const CartoTypeCore::Point& aStart,
                          const CartoTypeCore::Point& aEnd,EllipseAngleType aAngleType = EllipseAngleType::Shortest,bool aAppendStart = false);
    void AppendHalfCircle(double aCx,double aCy,double aSx,double aSy,double aEx,double aEy,
                          double aRadius,bool aAppendStart,bool aIsExactHalfCircle,bool aClockwise);
    void AppendQuadrant(double aCx,double aCy,double aSx,double aSy,double aEx,double aEy,double aRadius,
                        bool aAppendStart,bool aIsExactQuadrant,bool aClockwise);
    };

/** The data for a contour. The simplest implementation of writable contour data. */
class SimpleContourData: public MWritableContour
    {
    public:
    SimpleContourData() = default;
    /** Creates a SimpleContourData object. */
    SimpleContourData(CartoTypeCore::Point* aPoint,size_t aPoints,bool aClosed): Data(aPoint), Size(aPoints), IsClosed(aClosed) {  }

    ContourView ContourByIndex(size_t /*aIndex*/) const override { return ContourView(Data,Size,IsClosed); }
    bool MayHaveCurves() const override { return false; }
    OutlinePoint Point(size_t aIndex) const override { assert(aIndex < Size); return Data[aIndex]; }
    /** Returns a pointer to the first point. */
    CartoTypeCore::Point* PointData() override { return Data; }
    OutlinePoint* OutlinePointData() override { return nullptr; }
    /** Returns the number of points. */
    size_t Points() const override { return Size; }
    /** Reduces the number of points. */
    void ReduceSizeTo(size_t aPoints) override { assert(aPoints <= Size); Size = aPoints; }
    /** Sets the number of points. */
    void SetSize(size_t aPoints) override { assert(aPoints <= Size); Size = aPoints; }
    /** Returns true if the contour is closed. */
    bool Closed() const override { return IsClosed; }
    /** Changes whether the contour is closed. */
    void SetClosed(bool aClosed) override { IsClosed = aClosed; }
    /** Appends a point; does nothing in fact because the operation is not possible for this class. */
    void AppendPoint(const OutlinePoint&) override { } // not possible
    void SetPoint(size_t aIndex,const OutlinePoint& aPoint) override { assert(aIndex < Size); assert(aPoint.Type == PointType::OnCurve); Data[aIndex] = aPoint.Point(); }

    /** A pointer to the start of the array of points. */
    CartoTypeCore::Point* Data = nullptr;
    /** The number of points in the contour. */
    size_t Size = 0;
    /** True if the contour is closed. */
    bool IsClosed = false;
    };

/** An abstract base class for the writable contour classes Contour and OnCurveContour. */
template<class T> class ContourBase: public MWritableContour
    {
    public:
    // virtual functions from MPath

    /** Returns the number of contours, which is one. */
    size_t Contours() const override { return 1; }

    /** Returns the single contour of this object. */
    ContourView ContourByIndex(size_t /*aIndex*/) const override { return ContourView(iPoint.data(),iPoint.size(),iClosed); }

    // virtual functions from MContour

    /** Returns the number of points. */
    size_t Points() const override { return iPoint.size(); }

    /** Returns a point selected by its index. */
    OutlinePoint Point(size_t aIndex) const override { return iPoint[aIndex]; }

    /** Returns true if this contour is closed. */
    bool Closed() const override { return iClosed; }

    // virtual functions from MWritableContour

    /** Sets a point. */
    void SetPoint(size_t aIndex,const OutlinePoint& aPoint) override { iPoint[aIndex] = aPoint; }

    /** Reduces the number of points to aPoints. The address of the points does not change. */
    void ReduceSizeTo(size_t aPoints) override
        {
        assert(aPoints <= iPoint.size());
        iPoint.resize(aPoints);
        }

    /** Sets the number of points to aPoints. The address of the points may change. */
    void SetSize(size_t aSize) override { iPoint.resize(aSize); }

    /** Sets this contour's closed attribute. Does nothing if that is not possible. */
    void SetClosed(bool aClosed) override { iClosed = aClosed; }

    /** Appends a point. */
    void AppendPoint(const OutlinePoint& aPoint) override = 0;

    /** Appends a point to the contour whether or not it differs from the previous point. */
    void AppendPointEvenIfSame(const T& aPoint)
        {
        iPoint.push_back(aPoint);
        }
    /** Appends a point to the contour whether or not it differs from the previous point. */
    void AppendPoint(double aX,double aY)
        {
        iPoint.emplace_back(Round(aX),Round(aY));
        }
    /** Appends some points to the contour. */
    void AppendPoints(const T* aPoint,size_t aPoints) { iPoint.insert(iPoint.end(),aPoint,aPoint + aPoints); }
    /** Inserts a point at the specified index. */
    void InsertPoint(const T& aPoint,size_t aIndex) { iPoint.insert(iPoint.begin() + aIndex,aPoint); }
    /** Inserts some points at the specified index. */
    void InsertPoints(const T* aPoint,size_t aPoints,size_t aIndex) { iPoint.insert(iPoint.begin() + aIndex,aPoint,aPoint + aPoints); }
    /** Sets the path to its newly constructed state: empty and open. */
    void Clear() { iPoint.clear(); iClosed = false; }
    /** Reduces the number of points to zero. */
    void SetSizeToZero() { iPoint.clear(); }
    /** Removes a point specified by an index. */
    void RemovePoint(size_t aIndex) { iPoint.erase(iPoint.begin() + aIndex); }
    /** Removes a series of aCount points starting at aIndex. */
    void RemovePoints(size_t aIndex,size_t aCount) { iPoint.erase(iPoint.begin() + aIndex, iPoint.begin() + aIndex + aCount); }

    /** Applies an offset to every point in a contour. */
    void Offset(int32_t aDx,int32_t aDy)
        {
        for (auto& p : iPoint)
            {
            p.X += aDx;
            p.Y += aDy;
            }
        }

    /**
    Pre-allocates enough space to hold at least aCount points. This function has no effect on behavior
    but may increase speed.
    */
    void ReservePoints(size_t aCount) { iPoint.reserve(aCount); }
    /** Reverses the order of the points in the contour. */
    void Reverse() { std::reverse(iPoint.begin(),iPoint.end()); }

    /** Returns an iterator pointing to the first point of the contour. */
    using vector_t = std::vector<T>;

    /** Returns an iterator pointing to the first point of the contour. */
    typename vector_t::iterator begin() { return iPoint.begin(); }
    /** Returns an iterator pointing just after the last point of the contour. */
    typename vector_t::iterator end() { return iPoint.end(); }
    /** Returns a constant iterator pointing to the first point of the contour. */
    typename vector_t::const_iterator begin() const { return iPoint.begin(); }
    /** Returns a constant iterator pointing just after the last point of the contour. */
    typename vector_t::const_iterator end() const { return iPoint.end(); }

    protected:
    /** An array containing the points of the contour. */
    vector_t iPoint;
    /** True if the contour is closed. */
    bool iClosed = false;
    };

/**
A contour consisting of owned points with integer coordinates, which may be on-curve or off-curve.
Off-curve points are control points for quadratic or cubic spline curves. A contour may be open or closed.
*/
class Contour: public ContourBase<OutlinePoint>
    {
    public:
    Contour() = default;

    /** Creates a contour by copying another contour. */
    explicit Contour(const MContour& aContour)
        {
        iClosed = aContour.Closed();
        AppendContour(aContour);
        }

    /** The assignment operator. */
    Contour& operator=(const MContour& aContour) { MWritableContour::operator=(aContour); return *this; }

    // virtual functions from MPath
    bool MayHaveCurves() const override { return true; }

    // virtual functions from MContour
    CartoTypeCore::Point* PointData() override { return nullptr; }
    OutlinePoint* OutlinePointData() override { return iPoint.data(); }

    /** Returns a constant pointer to the start of the points. */
    const OutlinePoint* OutlinePointData() const { return iPoint.data(); }

    /** Appends a point to the contour, but only if it differs from the previous point or is a control point. */
    void AppendPoint(const OutlinePoint& aPoint) override
        {
        if (iPoint.size() && aPoint.Type == PointType::OnCurve && aPoint == iPoint.back())
            return;
        iPoint.push_back(aPoint);
        }
    /** Appends a point to the contour whether or not it differs from the previous point. */
    void AppendPoint(double aX,double aY,PointType aPointType = PointType::OnCurve)
        {
        iPoint.emplace_back(Round(aX),Round(aY),aPointType);
        }
    void AppendTransformedPoint64ths(const AffineTransform& aTransform,const PointFP& aPoint,PointType aPointType);
    /** Converts a point from floating-point to point in 64ths and appends it. */
    void AppendPoint64ths(const PointFP& aPoint,PointType aPointType)
        {
        iPoint.emplace_back(aPoint.Rounded64ths(),aPointType);
        }

    void MakeCircle(double aX,double aY,double aRadius);
    void MakePolygon(double aX,double aY,double aRadius,int32_t aSides);
    void MakeRoundedRectangle(double aX1,double aY1,double aX2,double aY2,double aWidth,double aRadius);
    static Contour RoundedRectangle(const CartoTypeCore::Point& aTopLeft,double aWidth,double aHeight,double aRX,double aRY);
    static Contour Rectangle(const CartoTypeCore::Point& aTopLeft,double aWidth,double aHeight);
    static Contour Read(DataInputStream& aInput);
    };

/**
A contour consisting of owned points with integer coordinates, which are all on the curve; the contour thus consists of straight line segments.
For contours with off-curve points use Contour.
*/
class OnCurveContour: public ContourBase<Point>
    {
    public:
    OnCurveContour() = default;

    /** Creates a contour by copying another contour. */
    explicit OnCurveContour(const MContour& aContour)
        {
        iClosed = aContour.Closed();
        AppendContour(aContour);
        }

    /** Creates a contour from an axis-aligned rectangle. */
    OnCurveContour(const Rect& aRect)
        {
        *this = aRect;
        }

    /** The assignment operator. */
    OnCurveContour& operator=(const MContour& aContour) { MWritableContour::operator=(aContour); return *this; }

    /** An assignment operator which takes an axis-aligned rectangle. */
    OnCurveContour& operator=(const Rect& aRect)
        {
        iClosed = true;
        iPoint.resize(4);
        iPoint[0] = aRect.Min;
        iPoint[1] = aRect.MaxMin();
        iPoint[2] = aRect.Max;
        iPoint[3] = aRect.MinMax();
        return *this;
        }

    // virtual functions from MPath
    bool MayHaveCurves() const override { return false; }

    // virtual functions from MContour
    CartoTypeCore::Point* PointData() override { return iPoint.data(); }
    OutlinePoint* OutlinePointData() override { return nullptr; }

    /** Returns a constant pointer to the point data. */
    const CartoTypeCore::Point* PointData() const { return iPoint.data(); }

    /** Appends a point to the contour, but only if it differs from the previous point. */
    void AppendPoint(const OutlinePoint& aPoint) override
        {
        if (iPoint.size() && aPoint.Point() == iPoint.back())
            return;
        iPoint.push_back(aPoint);
        }
    /** Appends a point to the contour whether or not it differs from the previous point. */
    void AppendPoint(double aX,double aY)
        {
        iPoint.emplace_back(Round(aX),Round(aY));
        }
    void AppendTransformedPoint64ths(const AffineTransform& aTransform,const PointFP& aPoint);
    /** Converts a point from floating-point to point in 64ths and appends it. */
    void AppendPoint64ths(const PointFP& aPoint)
        {
        iPoint.emplace_back(aPoint.Rounded64ths());
        }
    };

/**
A writable contour view returned by MapObject::WritableContour. It forwards MWritableContour functions either to an external interface or an owned object.
The latter option makes it possible for small objects holding a single point to return a writable contour object, in which case the point is changeable but the number of
points is not, nor the closed status.
*/
class WritableContourView: public MWritableContour
    {
    public:
    /** Creates a WritableContourView from an MWritableContour. */
    WritableContourView(MWritableContour& aWritableContour): m_ptr(&aWritableContour) {  }
    /** Creates a WritableContourView from a sequence of points. */
    WritableContourView(CartoTypeCore::Point* aPoint,size_t aPoints,bool aClosed): m_data(aPoint,aPoints,aClosed), m_ptr(&m_data) {  }

    ContourView ContourByIndex(size_t /*aIndex*/) const override { return m_ptr->ContourByIndex(0); }
    bool MayHaveCurves() const override { return m_ptr->MayHaveCurves(); }

    size_t Points() const override { return m_ptr->Points(); }
    OutlinePoint Point(size_t aIndex) const override { return m_ptr->Point(aIndex); }
    bool Closed() const override { return m_ptr->Closed(); }

    void SetPoint(size_t aIndex,const OutlinePoint& aPoint) override { m_ptr->SetPoint(aIndex,aPoint); }
    void ReduceSizeTo(size_t aPoints) override { m_ptr->ReduceSizeTo(aPoints); }
    void SetSize(size_t aPoints) override { m_ptr->SetSize(aPoints); }
    void SetClosed(bool aClosed) override { m_ptr->SetClosed(aClosed); }
    void AppendPoint(const OutlinePoint& aPoint) override { m_ptr->AppendPoint(aPoint); }
    OutlinePoint* OutlinePointData() override { return m_ptr->OutlinePointData(); }
    CartoTypeCore::Point* PointData() override { return m_ptr->PointData(); }

    private:
    SimpleContourData m_data;
    MWritableContour* m_ptr = nullptr;
    };

/** The standard path class. */
class Outline: public MPath
    {
    public:
    Outline() = default;
    Outline(const MPath& aPath);
    Outline(const Rect& aRect);
    Outline& operator=(const MPath& aPath);
    Outline& operator=(const Rect& aRect);

    // virtual functions from MPath
    size_t Contours() const override { return iContour.size(); }
    bool MayHaveCurves() const override { return true; }

    ContourView ContourByIndex(size_t aIndex) const override
        {
        return iContour[aIndex].ContourByIndex(0);
        }

    /** Appends a contour. */
    void AppendContour(Contour&& aContour)
        {
        iContour.push_back(std::move(aContour));
        }

    /** Clears the outline by removing all contours. */
    void Clear() { iContour.clear(); }

    /** Appends a new empty contour to the outline and return it. */
    [[nodiscard]] Contour& AppendContour()
        {
        iContour.emplace_back();
        return iContour.back();
        }

    /** Appends a contour to the outline. */
    void AppendContour(const MContour& aContour)
        {
        auto& c = AppendContour();
        c.SetClosed(aContour.Closed());
        c.AppendContour(aContour);
        }

    /** Return a non-constant reference to a contour, selected by its index. */
    Contour& ContourByIndex(size_t aIndex) { return iContour[aIndex]; }

    void MapCoordinatesToLatLong(const Projection& aProjection,int32_t aLatLongFractionalBits = 16);
    void LatLongToMapCoordinates(const Projection& aProjection,int32_t aLatLongFractionalBits = 16);
    /** Removes the vector of contours and puts it into aDest. */
    void RemoveData(std::vector<Contour>& aDest)
        {
        aDest = std::move(iContour);
        iContour.clear();
        }
    static Outline Read(DataInputStream& aInput);

    /** Returns an iterator to the start of the vector of contours. */
    std::vector<Contour>::iterator begin() { return iContour.begin(); }
    /** Returns an iterator to the end of the vector of contours. */
    std::vector<Contour>::iterator end() { return iContour.end(); }
    /** Returns a constant iterator to the start of the vector of contours. */
    std::vector<Contour>::const_iterator begin() const { return iContour.begin(); }
    /** Returns a constant iterator to the end of the vector of contours. */
    std::vector<Contour>::const_iterator end() const { return iContour.end(); }

    private:
    std::vector<Contour> iContour;
    };

/** Returns the contour to which this iterator refers. */
inline ContourView MPath::ConstIter::operator*() const
    {
    return m_path.ContourByIndex(m_index);
    }

/** Traverses this path, calling the functions defined by aTraverser to handle moves, lines, and curves. Clips the output to aClip. */
template<class MPathTraverser> inline void MPath::Traverse(MPathTraverser& aTraverser,const Rect& aClip) const
    {
    Traverse(aTraverser,&aClip);
    }

/** Traverses this path, calling the functions defined by aTraverser to handle moves, lines, and curves. Clips the output to aClip if aClip is non-null. */
template<class MPathTraverser> inline void MPath::Traverse(MPathTraverser& aTraverser,const Rect* aClip) const
    {
    if (aClip && aClip->IsMaximal())
        aClip = nullptr;
    for (auto c : *this)
        c.Traverse(aTraverser,aClip);
    }

/** Traverses this contour, calling the functions defined by aTraverser to handle moves, lines, and curves. Clips the output to aClip if aClip is non-null. */
template<class MTraverser> inline void ContourView::Traverse(MTraverser& aTraverser,const Rect* aClip) const
    {
    if (aClip && !aClip->IsMaximal() && ClipType(*aClip) != CartoTypeCore::ClipType::Inside)
        {
        if (Points() < 2)
            return;
        Outline clipped_contour = ClippedContour(*aClip);
        clipped_contour.Traverse(aTraverser);
        return;
        }
    if (m_has_off_curve_points)
        CartoTypeCore::Traverse(aTraverser,(OutlinePoint*)m_point,m_points,m_closed);
    else
        CartoTypeCore::Traverse(aTraverser,m_point,m_points,m_closed);
    }

/**
An iterator to traverse a path.
Limitation: for the moment it works with straight lines only; it
treats all points as on-curve points.
*/
class PathIterator
    {
    public:
    PathIterator(const MPath& aPath);
    bool Forward(double aDistance);
    bool NextContour();
    void MoveToNearestPoint(const PointFP& aPoint);
    /** Returns the current contour index. */
    size_t ContourIndex() const { return iContourIndex; }
    /** Returns the current contour. */
    ContourView ContourByIndex() const { return iContour; }
    /** Returns the current line index. */
    size_t LineIndex() const { return iLineIndex; }
    /** Returns the current position along the current line. */
    double PositionOnLine() const { return iPositionOnLine; }
    /** Returns the current position. */
    const Point& Position() const { return iPosition; }
    /** Returns the current direction in radians, clockwise from straight up, as an angle on the map, not a geodetic azimuth. */
    double Direction() const { return iDirection; }

    private:
    void CalculatePosition();

    const MPath& iPath;
    size_t iContourIndex = 0;
    ContourView iContour;
    size_t iLineIndex = 0;
    Point iPosition;
    double iPositionOnLine = 0;
    double iLineLength = 0;
    double iDirection = 0;
    double iDx = 0;
    double iDy = 0;
    };

/** A contour with a fixed number of points. */
template<size_t aPointCount,bool aClosed> class FixedSizeContour: public std::array<OutlinePoint,aPointCount>, public MContour
    {
    public:
    size_t Points() const override { return aPointCount; }
    OutlinePoint Point(size_t aIndex) const override { return std::array<OutlinePoint,aPointCount>::data()[aIndex]; }
    bool Closed() const override { return aClosed; }
    bool MayHaveCurves() const override { return true; }
    ContourView ContourByIndex(size_t /*aIndex*/) const override { return ContourView(std::array<OutlinePoint,aPointCount>::data(),aPointCount,aClosed); }

    using std::array<OutlinePoint,aPointCount>::begin;
    using std::array<OutlinePoint,aPointCount>::end;
    };

/** A contour consisting of non-owned points using floating-point coordinates, all of them being on-curve points. A ContourViewFP may be open or closed. */
class ContourViewFP
    {
    public:
    ContourViewFP() = default;
    /** Creates a ContourViewFP referring to a certain number of points, specifying whether it is open or closed. */
    ContourViewFP(const PointFP* aPoint,size_t aPoints,bool aClosed) :
        m_point(aPoint),
        m_end(aPoint + aPoints),
        m_closed(aClosed)
        {
        }
    /** Returns a constant pointer to the first point. */
    const PointFP* Point() const noexcept { return m_point; }
    /** Returns the number of points. */
    size_t Points() const noexcept { return m_end - m_point; }
    /** Returns true if this contour is closed. */
    bool Closed() const noexcept { return m_closed; }
    RectFP Bounds() const noexcept;
    bool Intersects(const RectFP& aRect) const noexcept;
    bool Contains(const PointFP& aPoint) const noexcept;

    private:
    const PointFP* m_point = nullptr;
    const PointFP* m_end = nullptr;
    bool m_closed = false;
    };

/** A contour with a fixed number of floating-point points. */
template<size_t aPointCount,bool aClosed> class FixedSizeContourFP: public std::array<PointFP,aPointCount>
    {
    public:
    /** Returns a ContourViewFP object representing this object. */
    operator ContourViewFP() const { return ContourViewFP(std::array<PointFP,aPointCount>::data(),aPointCount,aClosed); }
    };

class PolygonFP;

/** A contour consisting of owned points using floating-point coordinates, all of them being on-curve points. A ContourFP may be open or closed. */
class ContourFP
    {
    public:
    ContourFP() { }
    /** Creates a contour from an axis-aligned rectangle. */
    explicit ContourFP(const RectFP& aRect)
        {
        PointArray.resize(4);
        PointArray[0] = aRect.Min;
        PointArray[1] = aRect.MinMax();
        PointArray[2] = aRect.Max;
        PointArray[3] = aRect.MaxMin();
        }
    /** Appends a point to this contour. */
    void AppendPoint(const PointFP& aPoint) { PointArray.push_back(aPoint); }
    PolygonFP Clip(const RectFP& aClip);
    /** Returns the bounding box as an axis-aligned rectangle. */
    RectFP Bounds() const
        { return ContourViewFP(PointArray.data(),PointArray.size(),false).Bounds(); }

    /** The vector of points representing this contour. */
    std::vector<PointFP> PointArray;
    };

/** A polygon using floating-point coordinates, made up from zero or more contours represented as ContourFP objects. */
class PolygonFP
    {
    public:
    RectFP Bounds() const;

    /** The vector of contours. */
    std::vector<ContourFP> ContourArray;
    };

/**
A clip region.
This class enables optimisations: detemining whether the clip region is
an axis-aligned rectangle, and getting the bounding box.
*/
class ClipRegion
    {
    public:
    ClipRegion() = default;
    ClipRegion(const Rect& aRect);
    ClipRegion(const MPath& aPath);
    /** Returns the bounding box of this clip region. */
    const Rect& Bounds() const noexcept { return m_bounds; }
    /** Returns the clip region as a path. */
    const Outline& Path() const noexcept { return m_path; }
    /** Returns true if this clip region is an axis-aligned rectangle. */
    bool IsRect() const noexcept { return m_is_rect; }
    /** Returns true if this clip region is empty. */
    bool IsEmpty() const noexcept { return m_bounds.IsEmpty(); }

    private:
    Rect m_bounds;             // the bounds of the clip region as an axis-aligned rectangle
    Outline m_path;            // the clip path
    bool m_is_rect = true;      // true if the clip region is an axis-aligned rectangle
    };

/** Returns the area of a triangle made from the points aA, aB, aC. */
inline double TriangleArea(const Point& aA,const Point& aB,const Point& aC)
    {
    return fabs((double(aA.X) - double(aC.X)) * (double(aB.Y) - double(aA.Y)) - (double(aA.X) - double(aB.X)) * (double(aC.Y) - double(aA.Y))) / 2.0;
    }

/**
A templated function to simplify a contour of any type. Returns the new number of points, after simplification.

Simplification consists in removing all points with an area less than aResolutionArea.
A point's area is the area of the triangle made by the point and its two neighbours.

The start and end of an open contour are not removed.
Closed contours may be reduced to nothing.
*/
template<class T> size_t SimplifyContour(T* aPointArray,size_t aPointCount,bool aClosed,double aResolutionArea)
    {
    if (aPointCount < 3)
        return aPointCount;
    
    double cur_area = 0;
    if (aClosed)
        {
        T* source_point = aPointArray;
        T* dest_point = source_point;
        T* end_point = aPointArray + aPointCount;
        T* prev_point = end_point - 1;
        while (source_point < end_point)
            {
            T* next_point = source_point + 1;
            if (next_point == end_point)
                next_point = aPointArray;
            cur_area += TriangleArea(*prev_point,*source_point,*next_point);
            if (cur_area >= aResolutionArea)
                {
                prev_point = dest_point;
                *dest_point++ = *source_point;
                cur_area = 0;
                }
            source_point++;
            }
        return dest_point - aPointArray;
        }

    T* source_point = aPointArray + 1;
    T* dest_point = source_point;
    T* end_point = aPointArray + aPointCount - 1;
    while (source_point < end_point)
        {
        cur_area += TriangleArea(dest_point[-1],*source_point,source_point[1]);
        if (cur_area >= aResolutionArea)
            {
            *dest_point++ = *source_point;
            cur_area = 0;
            }
        source_point++;
        }
    *dest_point++ = *end_point;
    return dest_point - aPointArray;
    }

/** Returns the axis-aligned bounding box of a sequence of points, treating control points as ordinary points. */
template<class T> Rect CBox(T* aPointArray,size_t aPointCount)
    {
    if (!aPointCount)
        return Rect();
    const T* p = aPointArray;
    const T* q = p + aPointCount;
    Rect box(INT32_MAX,INT32_MAX,INT32_MIN,INT32_MIN);
    while (p < q)
        {
        int32_t x = p->X;
        int32_t y = p->Y;
        if (x < box.Min.X)
            box.Min.X = x;
        if (x > box.Max.X)
            box.Max.X = x;
        if (y < box.Min.Y)
            box.Min.Y = y;
        if (y > box.Max.Y)
            box.Max.Y = y;
        p++;
        }
    return box;
    }

}
