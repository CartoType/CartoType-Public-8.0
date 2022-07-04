/*
cartotype_transform.h
Copyright (C) 2004-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_base.h>
#include <cartotype_arithmetic.h>
#include <cartotype_stream.h>
#include <array>

namespace CartoTypeCore
{

/** Transform type flag bits; used to reduce calculation. */
namespace TransformType
    {
    /** The transform has no effect. */
    constexpr uint32_t Identity = 0;
    /** The transform reflects around the line (y = 0). */
    constexpr uint32_t YReflect = 1;
    /** The transform does translation. */
    constexpr uint32_t Translate = 2;
    /** The transform has an effect, and does something other than reflection and translation. */ 
    constexpr uint32_t General = 4;
    };

/** The components of a transform. */
class TransformComponents
    {
    public:
    /** The translation. */
    PointFP Translation;
    /** The center around which the transform performas any rotation. */
    PointFP CenterOfRotation;
    /** The rotation in radians. */
    double Rotation = 0;
    /** The scale. */
    PointFP Scale = { 1, 1 };
    /** The shear. */
    PointFP Shear;
    };

/** An affine 2D transformation. */
class AffineTransform
    {
    public:
    AffineTransform();
    AffineTransform(double aA,double aB,double aC,double aD,double aTx,double aTy);
    explicit AffineTransform(DataInputStream& aInput);

    /** Transforms the point (aX,aY). */
    void Transform(double& aX,double& aY) const
        {
        double new_x(iA * aX + iC * aY + iTx);
        double new_y(iB * aX + iD * aY + iTy);
        aX = new_x;
        aY = new_y;
        }
    void Transform(Point& aPoint) const;
    void Transform(PointFP& aPoint) const;
    void Transform(Rect& aRect) const;
    void Transform(RectFP& aRect) const;
    void Concat(const AffineTransform& aTransform);
    void Prefix(const AffineTransform& aTransform);
    void Scale(double aXScale,double aYScale);
    void ExtractScale(double& aXScale,double& aYScale);
    void Invert();
    /** Concatenates the translation (aX,aY) to this transform. */
    void Translate(double aX,double aY) { iTx += aX; iTy += aY; }
    void Rotate(double aAngle);
    /** Concatenates the rotation defined by Atan(aX,aY) to this transform. */
    void Rotate(double aX,double aY) { Rotate(Atan2(aY,aX)); }
    /** Concatenates a reflection about the line y=aY to this transform. */
    void ReflectY(double aY)
        {
        iTy -= aY;
        Scale(1,-1);
        iTy += aY;
        }
    /** Return the A (x scale) parameter. */
    double A() const { return iA; }
    /** Returns the B (y shear) parameter. */
    double B() const { return iB; }
    /** Returns the C (x shear) parameter. */
    double C() const { return iC; }
    /** Returns the D (y scale) parameter. */
    double D() const { return iD; }
    /** Returns the TX (x translation) parameter. */
    double Tx() const { return iTx; }
    /** Returns the TY (y translation) parameter. */
    double Ty() const { return iTy; }
    /** Returns the center of rotation of the projection. */
    PointFP CenterOfRotation() const;
    TransformComponents Components() const;
    AffineTransform Interpolate(const AffineTransform& aOther,double aTime);
    /** The equality operator. */
    bool operator==(const AffineTransform& aTransform) const;
    /** The inequality operator. */
    bool operator!=(const AffineTransform& aTransform) const { return !(*this == aTransform); }
    uint32_t Type() const;
    /** Returns true if this is the identity transform. */
    bool IsIdentity() const { return iA==1 && iB==0 && iC==0 && iD==1 && iTx==0 && iTy==0; }
    void Write(DataOutputStream& aOutput) const;

    private:
    double iA, iB, iC, iD, iTx, iTy;
    };

/** A 3D transformation. */
class Transform3D
    {
    public:
    Transform3D();
    Transform3D(const AffineTransform& aTransform);
    /** The equality operator. */
    bool operator==(const Transform3D& aOther) const { return iM == aOther.iM; }
    void Transform(Point3FP& aPoint) const;
    void Transform(double& aX,double& aY,double& aZ,double& aW) const;
    void Concat(const Transform3D& aTransform);
    void Translate(double aX,double aY,double aZ);
    void Scale(double aXScale,double aYScale,double aZScale);
    void RotateX(double aAngle);
    void RotateY(double aAngle);
    void RotateZ(double aAngle);
    void ReflectY(double aY);
    void Invert();
    double Determinant() const;
    Result Perspective(double aFieldOfViewYDegrees,double aAspect,double aNear,double aFar);
    Result Frustum(double aLeft,double aRight,double aBottom,double aTop,double aNear,double aFar);
    AffineTransform Affine2DTransform() const;
    /** Returns a pointer to the contiguous array of 16 values representing the 4x4 transformation matrix. */
    const double* Data() const { return iM.data(); }
   
    private:
    std::array<double,16> iM; // the transform matrix
    };

/** 
Parameters defining a camera position relative to a flat plane
representing the earth's surface projected on to a map.
*/
class CameraParam
    {
    public:
    /** The position of the camera. The z coordinate represents height above the surface. */
    Point3FP Position;
    /** The azimuth of the camera in degrees going clockwise, where 0 is N, 90 is E, etc. */
    double AzimuthDegrees = 0;
    /** The declination of the camera downward from the horizontal plane. */
    double DeclinationDegrees = 30;
    /** The amount by which the camera is rotated about its axis, after applying the declination, in degrees going clockwise. */
    double RotationDegrees = 0;
    /** The camera's field of view in degrees. */
    double FieldOfViewDegrees = 22.5;
    /**
    The display rectangle on to which the camera view is projected.
    The view width is scaled to that of the display, and the aspect
    ratio of the camera view is preserved.
    */
    RectFP Display = { 0,0,1,1 };
    /** True if y values increase upwards in the display coordinate system. */
    bool YAxisUp = false;
    };

} // namespace CartoTypeCore
