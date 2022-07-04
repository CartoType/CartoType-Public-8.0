/*
cartotype_map_object.h
Copyright (C) 2013-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_string.h>
#include <cartotype_path.h>
#include <cartotype_address.h>
#include <cartotype_transform.h>
#include <cartotype_geometry.h>
#include <cartotype_feature_info.h>

#include <map>

namespace CartoTypeCore
{
class CBasicMapObject;
class CMapAttributeTest;
class MapObject;
class MapTransform;
class Projection;
class BitmapView;
class TAttribSet;

/** A group of map objects with the same name returned by a search function. */
class MapObjectGroup
    {
    public:
    MapObjectGroup() = default;

    /** Creates a MapObjectGroup by moving data from another one. */
    MapObjectGroup(MapObjectGroup&& aOther) noexcept
        {
        if (this == &aOther)
            return;
        Name = std::move(aOther.Name);
        MapObjectArray = std::move(aOther.MapObjectArray);
        }

    /** Assigns data to a MapObjectGroup by moving data from another one. */
    MapObjectGroup& operator=(MapObjectGroup&& aOther) noexcept
        {
        if (this == &aOther)
            return *this;
        Name = std::move(aOther.Name);
        MapObjectArray = std::move(aOther.MapObjectArray);
        return *this;
        }

    /** The name, summary address or other common attribute of all the objects. */
    String Name;
    /** The map objects. */
    CartoTypeCore::MapObjectArray MapObjectArray;
    };

/** A type for arrays of map object groups returned by search functions. */
using MapObjectGroupArray = std::vector<MapObjectGroup>;

/** A type for arbitrary user-defined data associated with a map object. */
union UserData
    {
    UserData() { memset(this,0,sizeof(UserData)); }
    /** The integer value. */
    int64_t Int;
    /** The pointer value. */
    void* Ptr;
    };

/** Information returned by MapObject::Match. */
class MapObjectMatch
    {
    public:
    /** True if a match was found. */
    bool Found = false;
    /** The name of the attribute in which the matched text was found. */
    String Key;
    /** The value of the attribute in which the matched text was found. */
    String Value;
    /** The start position of the matched text within the value. */
    size_t Start = 0;
    /** The end position of the matched text within the value. */
    size_t End = 0;
    };

/** A type for different qualities of text match; returned by MapObject::MatchType. */
enum class MapObjectMatchType
    {
    /** No match. */
    None,
    /** A substring of the text matches, but the substring is not aligned to token boundaries. */
    Substring,
    /** There is a fuzzy match: a small number of extra characters, missing characters, or transpositions. */
    Fuzzy,
    /** There is a phrase match: a substring match aligned to token boundaries. */
    Phrase,
    /** A full match, ignoring letter case and accents. */
    Full
    };

/**
The abstract base map object class. A map object is a path that
also possesses cartographic attributes.
*/
class MapObject: public MPath
    {
    public:
    virtual ~MapObject()
        {
        }

    /**
    Creates a copy of a map object.
    The base implementation works for all types of map object except array map objects.
    */
    virtual std::unique_ptr<MapObject> Copy(Result& aError) const;
    
    /**
    Returns all the string attributes, null-separated, as key-value pairs separated
    by an equals sign. The first key must be empty, and means the label or name.
    */
    virtual Text StringAttributes() const = 0;
    
    /** Returns the default label or name of the object: the first, unnamed, string attribute. */
    virtual Text Label() const = 0;

    /** Returns a writable interface to the contour indexed by aIndex. */
    virtual WritableContourView WritableContour(size_t aIndex) = 0;

    /** Returns a pointer to writable string attributes if possible; used by makemap. */
    virtual MString* WritableStringAttributes() { return nullptr; }

    /**
    If the object contains array data that can be represented as
    a bitmap, returns a pointer to the bitmap.
    The base implementation returns null.
    */
    virtual const BitmapView* Bitmap() const;
    /**
    If the object contains array data that can be represented as
    a bitmap, returns a transform to convert from the bitmap bounds
    to the extent in map coordinates.
    The base implementation returns null.
    */
    virtual const AffineTransform* BitmapTransform() const;
    /**
    Normalizes a map object if possible by removing point contours with no points,
    polygon contours with fewer than 3 points, and line contours with fewer than 2 points.
    The base implementation does nothing.
    */
    virtual void Normalize();
    /**
    Simplifies a map object, if possible, by re-sampling it at a specified resolution.
    The base implementation does nothing.
    */
    virtual void Simplify(int32_t aResolution);
    /**
    Splits a line object into objects with no more than aMaxPoints points each.
    Puts any extra objects created into aExtraObjectArray.
    */
    void Split(size_t aMaxPoints,MapObjectArray& aExtraObjectArray);
 
    /** Returns a pointer to a TAttribSet if possible; used by makemap. */
    virtual TAttribSet* AttribSet() { return nullptr; }
    /** Returns a constant pointer to a TAttribSet if possible; used by makemap. */
    const TAttribSet* AttribSet() const { return const_cast<MapObject*>(this)->AttribSet(); }

    /** Returns the projection used to convert from degrees of latitude and longitude to the map units used by this object, or null if the projection is not known. */
    virtual std::shared_ptr<CartoTypeCore::Projection> Projection() const { return nullptr; }

    /** Returns the value of the string attribute with name aName. */
    Text StringAttribute(const MString& aName) const
        {
        return StringAttributes().Attribute(aName);
        };
    
    /** Returns the value of the string attribute with name aName. */
    Text StringAttribute(const String& aName) const
        {
        return StringAttributes().Attribute(aName);
        }
    
    /**
    Returns a string attribute for a given locale by appending a colon then the locale to aName.
    If no such attribute is available, uses the locale truncated to two letters if it was longer,
    then tries the plain name. The empty attribute name is changed to 'name' if a locale is added.
    */
    Text StringAttributeForLocale(const MString& aName,const char* aLocale) const;
    
    /**
    Returns a string attribute for a given locale by appending a colon then the locale to aName.
    If no such attribute is available, uses the locale truncated to two letters if it was longer,
    then tries the plain name. The empty attribute name is changed to 'name' if a locale is added.
    */
    Text StringAttributeForLocale(const String& aName,const char* aLocale) const
        {
        return StringAttributeForLocale((const MString&)aName,aLocale);
        }

    /**
    Gets a string attribute from a list of semi-colon-separated names in aPath.
    Replaces empty names with aDefault (as for example if aPath is empty and aDefault is 'name:es').
    Returns the first non-empty result.

    Returns true if the returned attribute is a name attribute, defined as the empty attribute,
    or 'name', or any attribute starting with 'name:'.
    */
    bool GetStringAttributeUsingPath(const MString& aPath,const MString& aDefault,Text& aValue) const;

    /** Returns all the string attributes as a std::map from names to values. */
    std::map<String,String> StringAttributeMap() const;

    /**	
    Returns the identifier of this object.
    Identifiers are opaque 64-bit numbers that are specific to a database.
    The value zero is reserved and means 'no identifier'.
    */
    uint64_t Id() const { return iId; }

    /** Returns the type of the object. */
    MapObjectType Type() const { return iType; }
    
    /** Returns the name of the layer this object belongs to. */
    const MString& LayerName() const { return *iLayer; }

    /** Returns the reference-counted name of the layer. */
    RefCountedString LayerRef() const { return iLayer; }

    /** Returns the feature info. */
    CartoTypeCore::FeatureInfo FeatureInfo() const { return iFeatureInfo; };

    /**
    Creates a generic name for a map object, using the supplied locale if possible.
    For example "restaurant", "fuel station" or "minor road".

    Note: locales are not yet implemented. All category names are in English.
    */
    String GenericName(const char* aLocale = nullptr) const;

    /**
    Creates a UTF-8 string describing a map object; used for debugging.
    The string consists of the object's string attributes, separated by | characters,
    then the layer, feature type and object type in square brackets. The object type
    is encoded as a single character: . - @ # for point, line, polygon and array.
    */
    std::string Description() const;
    
    /** Creates a verbose English-language description of a map object as a UTF-8 string. */
    std::string VerboseDescription() const;

    /**
    Returns the center of a map object in map coordinates.
    The center is defined as the center of gravity for polygons and arrays,
    a point half way along the path for lines, and the point itself for points.
    */
    PointFP Center() const;

    /**
    Returns the center of a map object in degrees.
    The center is defined as the center of gravity for polygons and arrays,
    a point half way along the path for lines, and the point itself for points.
    Because the center is calculated using map coordinates then converted to degrees,
    it depends on the map projection used by the map object.
    */
    PointFP CenterInDegrees(Result& aError) const;

    /** Returns the bounding box of a map object in degrees. */
    RectFP BoundsInDegrees(Result& aError) const;

    /** Returns the geometry of a map object in degrees. */
    Geometry GeometryInDegrees(Result& aError) const;

    /**
    Gets the height of a point in metres if this map object contains terrain heights.
    The argument aPoint is in map coordinates.
    The return value is true if a height is successfully obtained.

    There is no check for the validity or sanity of the data. The argument aHaveMetres
    indicates whether the object contains 16-bit metre values, or bytes encoding feet.

    Feet are encoded as follows: 0 for unknown heights (sea or unspecified);
    1...15 for -1400ft to sea level in intervals of 100ft;
    16...195 for 100ft to 18,000ft in intervals of 100ft;
    and 196...255 for 18,200ft to 30,000ft in intervals of 200ft.

    If aInverseTransform is supplied, it is the inverse of the transform
    used to convert from bitmap coordinates to map coordinates. If it is not
    supplied, it is calculated from the bitmap.
    */
    bool GetHeight(Point aPoint,bool aHaveMetres,const AffineTransform* aInverseTransform,int32_t& aHeight) const;
    /** Returns the geocode type of a map object. */
    CartoTypeCore::GeoCodeType GeoCodeType() const;
    /** Returns information used when geocoding a map object. */
    CartoTypeCore::GeoCodeItem GeoCodeItem(const char* aLocale) const;
    /**
    Returns the speed limit in kph for this object if it is a route object.
    Returns 0 if it is not a route object or if the speed limit is unknown or if
    there is no speed limit.
    */
    double SpeedLimit() const;
    /** Gets the speed limit in kph from the string attribute _s, if it exists in aStringAttribute. */
    static double SpeedLimit(const MString& aStringAttribute);
    /**
    Gets the top of an object in metres by reading it
    from the _t attribute. Returns 0 if no top is specified.
    The _t attribute is a copy of the OpenStreetMap 'height' attribute.
    */
    double Top() const;
    /**
    Gets the bottom of an object in metres by reading it
    from the _b attribute. Returns 0 if no bottom is specified.
    The _b attribute is a copy of the OpenStreetMap 'min_height' attribute.
    */
    double Bottom() const;
    /** Returns the value of a OpenStreetMap dimension attribute in meters. */
    double DimensionAttribute(const MString& aKey) const;
    /**
    Gets the next string attribute, starting at character position aPos in the string attributes,
    and updates aPos. Returns true if an attribute is found.

    This function can be used to enumerate string attributes by passing 0 as aPos and repeating
    until a value of false is returned.
    */
    bool NextStringAttribute(size_t& aPos,Text& aName,Text& aValue) const;
    /** Returns true if this map object can be merged with aOther. */
    bool Mergeable(const MapObject& aOther) const;
    /** Projects all the points in a map object to 64ths of pixels using the specified projection. */
    void Project(const MapTransform& aMapTransform);
    /** Projects all the points in a map object using the specified projection. */
    void Project(const CartoTypeCore::Projection& aProjection);
    /** Smooths a map object by converting sequences of straight lines to curves. */
    void Smooth();
    /** Offsets all points in the map object by the specified amounts. */
    void Offset(int32_t aDx,int32_t aDy);
    /**
    Clips a map object to the rectangle aClip.

    If the object was clipped the function returns true and puts the clipped object, which may be null, in aClippedObject.

    If aExtraClippedObjectArray is non-null, line objects with a single contour may be clipped into multiple objects to preserve their single-line nature,
    in which case the extra objects are appended to aExtraClippedObjectArray. All other objects are clipped to a single object.

    If aLinesMustBeSingleContours is true, line objects are never converted to multiple contours, even if aExtraClippedObjectArray is non-null. They are not clipped
    if that is necessary.

    If the object was not clipped (i.e., it was entirely contained in aClip) the function returns false.
    */
    bool Clip(const Rect& aClip,std::unique_ptr<MapObject>& aClippedObject,MapObjectArray* aExtraClippedObjectArray = nullptr,bool aLinesMustBeSingleContours = false) const;
    /**
    Clips a map object to the clip region aClip.

    If the object was clipped the function returns true and puts the clipped object, which may be null, in aClippedObject.

    If aExtraClippedObjectArray is non-null, line objects with a single contour may be clipped into multiple objects to preserve their single-line nature,
    in which case the extra objects are appended to aExtraClippedObjectArray. All other objects are clipped to a single object.

    If aLinesMustBeSingleContours is true, line objects are never converted to multiple contours, even if aExtraClippedObjectArray is non-null. They are not clipped
    if that is necessary.

    If the object was not clipped (i.e., it was entirely contained in aClip) the function returns false.
    */
    bool Clip(const ClipRegion& aClip,std::unique_ptr<MapObject>& aClippedObject,MapObjectArray* aExtraClippedObjectArray = nullptr,bool aLinesMustBeSingleContours = false) const;
    using MPath::Clip;
    /**
    Returns a version of a map object with all curved line segments
    flattened by being replaced by short straight line segments.
    Returns null if no flattening is needed.
    */
    std::unique_ptr<MapObject> Flatten(double aDistance) const;

    /**
    Creates a new map object, interpolating extra points so that the maximum difference between
    coordinates doesn't exceed aGranularity. Chooses the points to
    be interpolated using the coordinate modulo aGranularity, so
    that adjacent polygons have the same interpolated points and thus
    meet without gaps.
    */
    std::unique_ptr<MapObject> Interpolate(int32_t aGranularity) const;

    /** Sets the map object type. */
    void SetType(MapObjectType aType) { iType = aType; }
    /**
    Writes a map object to an output stream in serialized form.
    Writes the geometry as degrees lat/long.
    (For internal use only : if the map object doesn't store its projection, which is true of stack-allocated CTM1 objects, the projection must be supplied.)
    */
    Result Write(DataOutputStream& aOutput,const CartoTypeCore::Projection* aProjectionFromLatLong = nullptr) const;
    /**
    Calculates the area of a map object in square meters, using the projection stored in the map object.
    Returns 0 if the object is a point or line object.
    */
    double Area(Result& aError) const;
    /**
    Calculates the length or perimeter of a map object in meters, using the projection stored in the map object.
    Returns 0 if the object is a point object.
    */
    double LengthOrPerimeter(Result& aError) const;
    /** Sets the ID. */
    void SetId(uint64_t aId) { iId = aId; }
    /** Sets the feature information, which contains the road type if the object is a road. */
    void SetFeatureInfo(CartoTypeCore::FeatureInfo aFeatureInfo) { iFeatureInfo = aFeatureInfo; }
    /** Returns the feature type. */
    CartoTypeCore::FeatureType FeatureType() const { return iFeatureInfo.Type(); }
    /**
    Sets the integer sub-type of a non-route object.
    Returns an error if the sub-type is outside the legal range 0...2047,
    or if this object is a route object.
    */
    Result SetSubType(uint32_t aSubType);
    /** Returns the integer sub-type if this is a non-route object. Returns 0 for route objects. */
    uint32_t SubType() const { return iFeatureInfo.SubType(); }
    /** Sets the layer. */
    void SetLayer(RefCountedString aLayer) { iLayer = aLayer; }
    
    /**
    Finds the first string attribute matching aText, using aMatchMethod, and returns information about it.
    If aAttributes is non-null, examines only attributes in the supplied comma-separated list,
    otherwise examines all attributes except those starting with an underscore, which by convention are non-textual.
    Attribute names may contain the wild cards ? and *.
    If aPhrase is true (the default), allows matches of phrases within the attribute.
    */
    MapObjectMatch Match(const MString& aText,StringMatchMethod aMatchMethod,const MString* aAttributes = nullptr,bool aPhrase = true) const;

    /**
    Finds out whether a map object is a partial or full match for some text.

    If the text matches the whole of a name attribute, or
    the whole of a sub-part of a name attribute (using spaces, hyphens, single quotes, colons, semicolons and slashes as separators)
    ignoring case and accents, it's a phrase match or a fuzzy match.

    If it matches part of the attribute it's a phrase match.
    */
    MapObjectMatchType MatchType(const MString& aText) const;

    /** Sets the user data to a 64-bit integer. */
    void SetUserData(int64_t aData) { iUserData.Int = aData; }
    /** Returns the user data as a 64-bit integer. */
    int64_t UserData() const { return iUserData.Int; }
    /** Sets the user data to a pointer. */
    void SetUserDataToPointer(void* aData) { iUserData.Ptr = aData; }
    /** Returns the user data as a pointer. */
    void* UserDataAsPointer() const { return iUserData.Ptr; }
    /** Sets the user data to a union */
    void SetUserDataToUnion(CartoTypeCore::UserData aUserData) { iUserData = aUserData; }
    /** Returns the user data as a union. */
    CartoTypeCore::UserData UserDataAsUnion() const { return iUserData; }

    private:
    bool ClipHelper(Outline& aClippedOutline,std::unique_ptr<MapObject>& aClippedObject,MapObjectArray* aExtraClippedObjectArray,bool aLinesMustBeSingleContours) const;

    protected:
    /** Creates a map object with a specified layer and type. */
    MapObject(RefCountedString aLayer,MapObjectType aType):
        iLayer(aLayer),
        iType(aType)
        {
        assert(aLayer);
        }
    /** Creates a map object with a specified layer, type and user data. */
    MapObject(RefCountedString aLayer,MapObjectType aType,CartoTypeCore::UserData aUserData):
        iLayer(aLayer),
        iType(aType),
        iUserData(aUserData)
        {
        assert(aLayer);
        }
    /** Creates a map object by copying another one. */
    MapObject(const MapObject& aOther) = default;
    /** The assignment operator. */
    MapObject& operator=(const MapObject& aOther) = default;

    /** The layer this object belongs to. */
    RefCountedString iLayer;
    /** The object's identifier. */
    uint64_t iId = 0;
    /** The type of the object: point, line, polygon, etc. */
    MapObjectType iType;
    /** The feature type, level, road direction, etc., packed into a 32-bit value. */
    CartoTypeCore::FeatureInfo iFeatureInfo;
    /** User data; can hold an integer or a pointer; can be used to associate any value with a map object. */
    CartoTypeCore::UserData iUserData;
    };

/** A comparison function which compares unique pointers to map objects on their user data. Neither pointer can be null. This function can be used in std::sort, etc. */
inline bool MapObjectUserDataLessThan(const std::unique_ptr<MapObject>& aP,const std::unique_ptr<MapObject>& aQ)
    {
    return aP->UserData() < aQ->UserData();
    }

}
