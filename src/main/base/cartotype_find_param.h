/*
cartotype_find_param.h
Copyright (C) CartoType Ltd 2013-2022.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_geometry.h>
#include <cartotype_feature_info.h>

namespace CartoTypeCore
{

/** Parameters for finding nearby places. */
class FindNearbyParam
    {
    public:
    /** The type of place to search for. The value Invalid causes iText only to be used. */
    FeatureType Type = FeatureType::Invalid;
    /** The name, full or partial, of the place. */
    String Text;
    /** The location of interest. */
    Geometry Location;
    };

/** Parameters for the general find function. */
class FindParam
    {
    public:
    FindParam() = default;
    FindParam(const FindNearbyParam& aFindNearbyParam);

    /** The maximum number of objects to return; default = SIZE_MAX. */
    size_t MaxObjectCount = SIZE_MAX;
    /** The clip path; no clipping is done if Clip is empty. */
    Geometry Clip;
    /** The current location. If it is non-empty, objects in or near this region are preferred. It may be a single point. */
    Geometry Location;
    /** A list of layer names separated by spaces or commas. If it is empty all layers are searched. Layer names may contain the wild cards * and ?. */
    String Layers;
    /**
    Attributes is used in text searching (if Text is non-null). If Attributes
    is empty, search all attributes, otherwise Attributes is a list of
    attributes separated by spaces or commas; use "$" to indicate the label (the
    unnamed attribute).
    */
    String Attributes;
    /**
    Text, if not empty, restricts the search to objects containing a string
    in one of their string attributes.
    */
    String Text;
    /** The string matching method used for text searching; default = Exact. */
    CartoTypeCore::StringMatchMethod StringMatchMethod = StringMatchMethod::Exact;
    /**
    Condition, if not empty, is a style sheet condition (e.g., "sub_type==2")
    which must be fulfilled by all the objects. 
    */
    String Condition;
    /** If Merge is true adjoining objects with the same name and attributes may be merged into a single object. The default value is false. */
    bool Merge = false;
    /**
    The maximum time in seconds allowed for a find operation. Find operations are not guaranteed to
    return in this time or less, but will attempt to do so.
    */
    double TimeOut = 0.5;
    };

}

