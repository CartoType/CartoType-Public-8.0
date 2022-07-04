/*
cartotype_framework.h
Copyright (C) 2012-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_address.h>
#include <cartotype_bitmap.h>
#include <cartotype_find_param.h>
#include <cartotype_navigation.h>
#include <cartotype_stream.h>
#include <cartotype_string.h>
#include <cartotype_map_object.h>
#include <cartotype_graphics_context.h>
#include <cartotype_legend.h>
#include <cartotype_style_sheet_data.h>
#include <cartotype_expression.h>
#include <cartotype_map_metadata.h>
#include <cartotype_framework_observer.h>
#include <cartotype_feature_info.h>

#include <memory>
#include <set>

namespace CartoTypeCore
{

/**
\mainpage CartoType

\section introduction Introduction

CartoType is a portable C++ library for drawing maps, calculating routes
and providing navigation instructions. It uses map files created using the
makemap tool supplied with the package.

Your application can get access to the CartoType API by including
the header cartotype.h. All classes and other declarations are in the
CartoType namespace.

\section highlevelapi The Framework API

The Framework class is the main API for CartoType and allows
you to build CartoType into your application.

You create a single Framework
object and keep it in existence while your program is running.
It provides access to everything you need, including
map drawing, adding your own data to the map, handling user
interaction, searching, route calculation and turn-by-turn navigation.

The classes FrameworkEngine and FrameworkMapDataSet, in conjunction
with Framework, allow you to use the model-view-controller pattern.
Usually, however, Framework is all you need.
*/

class CEngine;
class CMapDataAccessor;
class CPerspectiveGraphicsContext;
class MInternetAccessor;
class CMap;
class CMapDrawParam;
class CMapDataBase;
class CMapDataBaseArray;
class CMapStore;
class CMapStyle;
class CMemoryMapDataBase;
class CNavigator;
class GraphicsContext;
class C32BitColorBitmapGraphicsContext;
class CStackAllocator;
class CTileServer;
class MapTransform;
class CMapRendererImplementation;
class CAsyncFinder;
class CAsyncRouter;
class CNavigatorFuture;
class CMapObjectEditor;
class MFrameworkObserver;

namespace Router
    {
    class TRoutePointInternal;
    }

/** A type for functions called by the asynchronous Find function. */
using FindAsyncCallBack = std::function<void(std::unique_ptr<MapObjectArray> aMapObjectArray)>;
/** A type for functions called by the asynchronous Find function for map object group arrays. */
using FindAsyncGroupCallBack = std::function<void(std::unique_ptr<MapObjectGroupArray> aMapObjectGroupArray)>;
/** A type for functions called by the asynchronous routing function. */
using RouterAsyncCallBack = std::function<void(Result aError,std::unique_ptr<Route> aRoute)>;

/** A flag to make the center of the map follow the user's location. */
constexpr uint32_t KFollowFlagLocation = 1;
/** A flag to rotate the map to the user's heading. */
constexpr uint32_t KFollowFlagHeading = 2;
/** A flag to set the map to a suitable zoom level for the user's speed. */
constexpr uint32_t KFollowFlagZoom = 4;

/** Flags controlling the way the map follows the user location and heading and automatically zooms. */
enum class FollowMode
    {
    /** The map does not follow the user's location or heading. */
    None = 0,
    /** The map is centred on the user's location. */
    Location = KFollowFlagLocation,
    /** The map is centred on the user's location and rotated to the user's heading. */
    LocationHeading = KFollowFlagLocation | KFollowFlagHeading,
    /** The map is centred on the user's location and zoomed to a suitable level for the user's speed. */
    LocationZoom = KFollowFlagLocation | KFollowFlagZoom,
    /** The map is centred on the user's location, rotated to the user's heading, and zoomed to a suitable level for the user's speed. */
    LocationHeadingZoom = KFollowFlagLocation | KFollowFlagHeading | KFollowFlagZoom
    };

/**
FrameworkEngine holds a CTM1 data accessor and a standard font.
Together with a FrameworkMapDataSet object it makes up the 'model' part of the model-view-controller pattern.
*/
class FrameworkEngine
    {
    public:
    FrameworkEngine(const std::vector<TypefaceData>& aTypefaceDataArray,int32_t aFileBufferSizeInBytes,int32_t aMaxFileBufferCount,int32_t aTextIndexLevels);
    static std::unique_ptr<FrameworkEngine> New(Result& aError,const String& aFontFileName,int32_t aFileBufferSizeInBytes = 0,int32_t aMaxFileBufferCount = 0,int32_t aTextIndexLevels = 0);
    static std::unique_ptr<FrameworkEngine> New(Result& aError,const std::vector<TypefaceData>& aTypefaceDataArray,int32_t aFileBufferSizeInBytes = 0,int32_t aMaxFileBufferCount = 0,int32_t aTextIndexLevels = 0);
    Result Configure(const String& aFilename);
    Result LoadFont(const String& aFontFileName);
    Result LoadFont(const uint8_t* aData,size_t aLength,bool aCopyData);
    std::unique_ptr<FrameworkEngine> Copy(Result& aError);

    // internal use only

    /** Returns the CEngine object used by this FrameworkEngine. For internal use only. */
    std::shared_ptr<CEngine> Engine() const { return iEngine; }

    private:
    FrameworkEngine(const FrameworkEngine&) = delete;
    FrameworkEngine(FrameworkEngine&&) = delete;
    FrameworkEngine& operator=(const FrameworkEngine&) = delete;
    FrameworkEngine& operator=(FrameworkEngine&&) = delete;

    std::shared_ptr<CEngine> iEngine;
    String iConfigFileName;
    std::vector<TypefaceData> iTypefaceDataArray;
    int32_t iFileBufferSizeInBytes = 0;
    int32_t iMaxFileBufferCount = 0;
    int32_t iTextIndexLevels = 0;
    };

/**
FrameworkMapDataSet owns a set of map data used to draw a map.
Together with a FrameworkEngine object it makes up the 'model' part of the model-view-controller pattern.
*/
class FrameworkMapDataSet
    {
    public:
    FrameworkMapDataSet(std::shared_ptr<FrameworkEngine> aEngine,std::unique_ptr<CMapDataBase> aDb);
    static std::unique_ptr<FrameworkMapDataSet> New(Result& aError,std::shared_ptr<FrameworkEngine> aEngine,const String& aMapFileName,const std::string* aEncryptionKey = nullptr,bool aMapOverlaps = true);
    static std::unique_ptr<FrameworkMapDataSet> New(Result& aError,std::shared_ptr<FrameworkEngine> aEngine,std::unique_ptr<CMapDataBase> aDb);

    std::unique_ptr<FrameworkMapDataSet> Copy(Result& aError,std::shared_ptr<FrameworkEngine> aEngine,bool aFull = true);
    Result LoadMapData(const String& aMapFileName,const std::string* aEncryptionKey,bool aMapOverlaps);
    Result LoadMapData(std::unique_ptr<CMapDataBase> aDb);
    Result UnloadMapByHandle(uint32_t aHandle);
    uint32_t LastMapHandle() const;
    Result CreateWritableMap(WritableMapType aType,String aFileName = nullptr);
    Result SaveMap(uint32_t aHandle,const String& aFileName,FileType aFileType);
    Result ReadMap(uint32_t aHandle,const String& aFileName,FileType aFileType);
    Result ReadMap(uint32_t aHandle,const std::vector<uint8_t>& aData);
    bool MapIsEmpty(uint32_t aHandle);
    std::unique_ptr<CMap> CreateMap(int32_t aViewWidth,int32_t aViewHeight);
    uint32_t MainMapHandle() const;
    uint32_t MemoryMapHandle() const;
    size_t MapCount() const;
    uint32_t MapHandle(size_t aIndex) const;
    bool MapIsWritable(size_t aIndex) const;
    std::unique_ptr<CartoTypeCore::MapMetaData> MapMetaData(size_t aIndex) const;
    std::vector<String> LayerNames();
    
    Result InsertMapObject(uint32_t aMapHandle,const String& aLayerName,const MPath& aGeometry,
                            const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result InsertPointMapObject(uint32_t aMapHandle,const String& aLayerName,Point aPoint,
                                 const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result InsertCircleMapObject(uint32_t aMapHandle,const String& aLayerName,Point aCenter,int32_t aRadius,
                                  const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result InsertEnvelopeMapObject(uint32_t aMapHandle,const String& aLayerName,const MPath& aGeometry,int32_t aRadius,
                                    const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result DeleteMapObjectRange(uint32_t aMapHandle,uint64_t aStartId,uint64_t aEndId,uint64_t& aDeletedCount,String aCondition = nullptr);
    Result DeleteMapObjectArray(uint32_t aMapHandle,const uint64_t* aIdArray,size_t aIdCount,uint64_t& aDeletedCount,String aCondition = nullptr);
    Result DeleteAllMapObjects(uint32_t aMapHandle,uint64_t& aDeletedCount);
    std::unique_ptr<MapObject> LoadMapObject(Result& aError,uint32_t aMapHandle,uint64_t aId);
    Result ReadGpx(uint32_t aMapHandle,const String& aFileName);
    std::string Proj4Param() const;

    // for internal use only

    /** Returns the map database array. For internal use only. */
    std::shared_ptr<CMapDataBaseArray> MapDataBaseArray() const { return iMapDataBaseArray; }
    /** Returns the main map database. For internal use only. */
    CMapDataBase& MainDb() const;
    /** Returns a map database by its handle.  For internal use only. */
    CMapDataBase* MapDb(uint32_t aHandle,bool aTolerateNonExistentDb = false);

    private:
    FrameworkMapDataSet(const FrameworkMapDataSet&) = delete;
    FrameworkMapDataSet(FrameworkMapDataSet&&) = delete;
    FrameworkMapDataSet& operator=(const FrameworkMapDataSet&) = delete;
    FrameworkMapDataSet& operator=(const FrameworkMapDataSet&&) = delete;
    
    void RecalculateOverlapPaths();

    std::shared_ptr<FrameworkEngine> iEngine;
    std::shared_ptr<CMapDataBaseArray> iMapDataBaseArray;
    uint32_t iLastMapHandle = 0xFFFF; // start map handles at a value unlikely to conflict with map indexes
    uint32_t iMemoryMapHandle = 0;
    };

/** Parameters giving detailed control of the perspective view. */
class PerspectiveParam
    {
    public:
    PerspectiveParam() = default;
    explicit PerspectiveParam(const char* aText);
    Result ReadFromXml(const char* aText);
    std::string ToXml() const;

    /** The equality operator. */
    bool operator==(const PerspectiveParam& aP) const
        {
        return std::forward_as_tuple(PositionDegrees,AutoPosition,HeightMeters,AzimuthDegrees,AutoAzimuth,DeclinationDegrees,RotationDegrees,FieldOfViewDegrees) ==
               std::forward_as_tuple(aP.PositionDegrees,aP.AutoPosition,aP.HeightMeters,aP.AzimuthDegrees,aP.AutoAzimuth,aP.DeclinationDegrees,aP.RotationDegrees,aP.FieldOfViewDegrees);
        }
    /** The inequality operator. */
    bool operator!=(const PerspectiveParam& aP) const
        {
        return !(*this == aP);
        }
    /** The less-than operator. */
    bool operator<(const PerspectiveParam& aP) const
        {
        return std::forward_as_tuple(PositionDegrees,AutoPosition,HeightMeters,AzimuthDegrees,AutoAzimuth,DeclinationDegrees,RotationDegrees,FieldOfViewDegrees) <
               std::forward_as_tuple(aP.PositionDegrees,aP.AutoPosition,aP.HeightMeters,aP.AzimuthDegrees,aP.AutoAzimuth,aP.DeclinationDegrees,aP.RotationDegrees,aP.FieldOfViewDegrees);
        }

    /** The position of the point on the terrain below the camera, in degrees longitude (x) and latitude (y). */
    PointFP PositionDegrees;
    /** If true, ignore iPosition, and set the camera position so that the location in the center of the display is shown. */
    bool AutoPosition = true;
    /** The height of the camera above the terrain. The value 0 causes a default value to be used, which is the width of the display. */
    double HeightMeters = 0;
    /** The azimuth of the camera in degrees going clockwise, where 0 is N, 90 is E, etc. */
    double AzimuthDegrees = 0;
    /** If true, ignore iAzimuthDegrees and use the current map orientation. */
    bool AutoAzimuth = true;
    /** The declination of the camera downward from the horizontal plane. Values are clamped to the range -90...90. */
    double DeclinationDegrees = 30;
    /** The amount by which the camera is rotated about its axis, after applying the declination, in degrees going clockwise. */
    double RotationDegrees = 0;
    /** The camera's field of view in degrees. */
    double FieldOfViewDegrees = 22.5;
    };

/** The view state, which can be retrieved or set so that maps can be recreated. */
class ViewState
    {
    public:
    ViewState() = default;
    explicit ViewState(const char* aXmlText);
    Result ReadFromXml(const char* aXmlText);
    bool operator==(const ViewState& aOther) const;
    bool operator<(const ViewState& aOther) const;
    /** The inequality operator. */
    bool operator!=(const ViewState& aOther) const { return !(*this == aOther); };
    std::string ToXml() const;

    /** The display width in pixels. */
    int32_t WidthInPixels = 256;
    /** The display height in pixels. */
    int32_t HeightInPixels = 256;
    /** The view center in degrees longitude (x) and latitude (y). */
    PointFP ViewCenterDegrees;
    /** The denominator of the scale fraction; e.g., 50000 for 1:50000. */
    double ScaleDenominator = 0;
    /** The clockwise rotation of the view in degrees. */
    double RotationDegrees = 0;
    /** True if perspective mode is on. */
    bool Perspective = false;
    /** The parameters to be used for perspective mode. */
    CartoTypeCore::PerspectiveParam PerspectiveParam;

    private:
    auto Tuple() const { return std::forward_as_tuple(WidthInPixels,HeightInPixels,ViewCenterDegrees,ScaleDenominator,RotationDegrees,Perspective,PerspectiveParam); }
    };

/** A type for a sequence of track points. */
using TrackGeometry = GeneralGeometry<TrackPoint>;

/**
The Framework class provides a high-level API for CartoTypeCore,
through which map data can be loaded, maps can be created and viewed,
and routing and navigation can be performed.
*/
class Framework: public MNavigatorObserver
    {
    public:
    static std::unique_ptr<Framework> New(Result& aError,
                                           const String& aMapFileName,
                                           const String& aStyleSheetFileName,
                                           const String& aFontFileName,
                                           int32_t aViewWidth,int32_t aViewHeight,
                                           const std::string* aEncryptionKey = nullptr);
    static std::unique_ptr<Framework> New(Result& aError,
                                           std::shared_ptr<FrameworkEngine> aSharedEngine,
                                           std::shared_ptr<FrameworkMapDataSet> aSharedMapDataSet,
                                           const String& aStyleSheetFileName,
                                           int32_t aViewWidth,int32_t aViewHeight,
                                           const std::string* aEncryptionKey = nullptr);

    ~Framework();
    
    /**
    Parameters for creating a framework when more detailed control is needed.
    For example, file buffer size and the maximum number of buffers can be set.
    */
    class Param
        {
        public:
        /** The first map. Must not be empty. */
        String MapFileName;
        /** The first style sheet. If this string is empty, the style sheet must be supplied in iStyleSheetText. */
        String StyleSheetFileName;
        /** The style sheet text; used if iStyleSheetFileName is empty. */
        std::string StyleSheetText;
        /** The first font file. If this is empty, a small built-in font is loaded containing the Roman script only. */
        String FontFileName;
        /** The width of the initial map in pixels. Must be greater than zero. */
        int32_t ViewWidth = 256;
        /** The height of the initial map in pixels. Must be greater than zero. */
        int32_t ViewHeight = 256;
        /** If not empty, an encryption key to be used when loading the first map. */
        std::string EncryptionKey;
        /** The file buffer size in bytes. If it is less than four the default value is used. */
        int32_t FileBufferSizeInBytes = 0;
        /** The maximum number of file buffers. If it is zero or less the default value is used. */
        int32_t MaxFileBufferCount = 0;
        /**
        The number of levels of the text index to load into RAM.
        Use values from 2 to 5 to make text searches faster, at the cost of using much more RAM.
        The value 0 causes the default number of levels to be loaded, which is 1.
        The value -1 disables text index loading.
        */
        int32_t TextIndexLevels = 0;
        /** If non-null, use this shared engine and do not use iMapFileName or iFontFileName. */
        std::shared_ptr<FrameworkEngine> SharedEngine;
        /** If non-null, use this shared dataset and do not use iMapFileName or iFontFileName. */
        std::shared_ptr<FrameworkMapDataSet> SharedMapDataSet;
        /**
        If true, maps are allowed to overlap.
        If false, maps are clipped so that they do not overlap maps previously loaded.
        */
        bool MapsOverlap = true;
        };
    static std::unique_ptr<Framework> New(Result& aError,const Param& aParam);

    std::unique_ptr<Framework> Copy(Result& aError,bool aFull = true) const;

    // general
    Result License(const String& aKey);
    Result License(const uint8_t* aKey,size_t aKeyLength);
    std::string Licensee() const;
    std::string ExpiryDate() const;
    std::string AppBuildDate() const;
    uint8_t License() const;
    String Copyright() const;
    void SetCopyrightNotice();
    void SetCopyrightNotice(const String& aCopyright);
    void SetLegend(std::unique_ptr<Legend> aLegend,double aWidth,const char* aUnit,const ExtendedNoticePosition& aPosition);
    void SetLegend(const Legend& aLegend,double aWidth,const char* aUnit,const ExtendedNoticePosition& aPosition);
    void EnableLegend(bool aEnable);
    bool LegendEnabled() const;
    void SetScaleBar(bool aMetricUnits,double aWidth,const char* aUnit,const ExtendedNoticePosition& aPosition);
    void EnableScaleBar(bool aEnable);
    bool ScaleBarEnabled() const;
    Result SetTurnInstructions(bool aMetricUnits,bool aAbbreviate,double aWidth,const char* aUnit,const ExtendedNoticePosition& aPosition,double aTextSize = 7,const char* aTextSizeUnit = "pt");
    void EnableTurnInstructions(bool aEnable);
    bool TurnInstructionsEnabled() const;
    void SetTurnInstructionText(const String& aText);
    String TurnInstructionText();
    void DrawNoticesAutomatically(bool aEnable);
    bool HasNotices() const;
    PositionedBitmap NoticeBitmap() const;
    Result Configure(const String& aFilename);
    Result LoadMap(const String& aMapFileName,const std::string* aEncryptionKey = nullptr);
    bool SetMapsOverlap(bool aEnable);
    Result CreateWritableMap(WritableMapType aType,String aFileName = nullptr);
    Result SaveMap(uint32_t aHandle,const String& aFileName,FileType aFileType);
    Result ReadMap(uint32_t aHandle,const String& aFileName,FileType aFileType);
    Result SaveMap(uint32_t aHandle,std::vector<uint8_t>& aData,const FindParam& aFindParam);
    Result ReadMap(uint32_t aHandle,const std::vector<uint8_t>& aData);
    Result WriteMapImage(const String& aFileName,FileType aFileType,bool aCompress = false);
    bool MapIsEmpty(uint32_t aHandle);
    size_t MapCount() const;
    uint32_t MapHandle(size_t aIndex) const;
    bool MapIsWritable(size_t aIndex) const;
    std::unique_ptr<CartoTypeCore::MapMetaData> MapMetaData(size_t aIndex) const;
    Result UnloadMapByHandle(uint32_t aHandle);
    Result EnableMapByHandle(uint32_t aHandle,bool aEnable);
    Result EnableAllMaps();
    uint32_t LastMapHandle() const;
    uint32_t MainMapHandle() const;
    uint32_t MemoryMapHandle() const;
    Result LoadFont(const String& aFontFileName);
    Result LoadFont(const uint8_t* aData,size_t aLength,bool aCopyData);
    Result SetStyleSheet(const String& aStyleSheetFileName,size_t aIndex = 0);
    Result SetStyleSheet(const uint8_t* aData,size_t aLength,size_t aIndex = 0);
    Result SetStyleSheet(const StyleSheetData& aStyleSheetData,size_t aIndex = 0);
    Result SetStyleSheet(const StyleSheetDataArray& aStyleSheetDataArray,const VariableDictionary* aStyleSheetVariables = nullptr,const BlendStyleSet* aBlendStyleSet = nullptr);
    Result ReloadStyleSheet(size_t aIndex = 0);
    Result AppendStyleSheet(const String& aStyleSheetFileName);
    Result AppendStyleSheet(const uint8_t* aData,size_t aLength);
    Result DeleteStyleSheet(size_t aIndex);
    std::string StyleSheetText(size_t aIndex) const;
    CartoTypeCore::StyleSheetData StyleSheetData(size_t aIndex) const;
    const CartoTypeCore::StyleSheetDataArray& StyleSheetDataArray() const;
    const VariableDictionary& StyleSheetVariables() const;
    bool SetNightMode(bool aSet);
    Color SetNightModeColor(Color aColor);
    bool NightMode() const;
    Color NightModeColor() const;
    CartoTypeCore::BlendStyleSet SetBlendStyle(const BlendStyleSet* aBlendStyleSet);
    CartoTypeCore::BlendStyleSet BlendStyleSet() const;
    FileLocation StyleSheetErrorLocation() const;

    Result Resize(int32_t aViewWidth,int32_t aViewHeight);
    void SetResolutionDpi(double aDpi);
    double ResolutionDpi() const;
    Result SetScaleDenominator(double aScale);
    double ScaleDenominator() const;
    double MapUnitSize() const;
    Result SetScaleDenominatorInView(double aScale);
    double ScaleDenominatorInView() const;
    double DistanceInMeters(double aX1,double aY1,double aX2,double aY2,CoordType aCoordType) const;
    double ScaleDenominatorFromZoomLevel(double aZoomLevel,int32_t aImageSizeInPixels = 256) const;
    double ZoomLevelFromScaleDenominator(double aScaleDenominator,int32_t aImageSizeInPixels = 256) const;

    void SetPerspective(bool aSet);
    void SetPerspective(const PerspectiveParam& aParam);
    bool Perspective() const;
    CartoTypeCore::PerspectiveParam PerspectiveParam() const;
    Result Zoom(double aZoomFactor);
    Result ZoomAt(double aZoomFactor,double aX,double aY,CoordType aCoordType);
    Result Rotate(double aAngle);
    Result RotateAt(double aAngle,double aX,double aY,CoordType aCoordType);
    Result SetRotation(double aAngle);
    Result SetRotationAt(double aAngle,double aX,double aY,CoordType aCoordType);
    double Rotation() const;
    Result RotateAndZoom(double aAngle,double aZoomFactor,double aX,double aY,CoordType aCoordType);
    Result Pan(int32_t aDx,int32_t aDy);
    Result Pan(const Point& aFrom,const Point& aTo);
    Result Pan(double aFromX,double aFromY,CoordType aFromCoordType,double aToX,double aToY,CoordType aToCoordType);
    Result SetViewCenter(double aX,double aY,CoordType aCoordType);
    Result SetView(double aMinX,double aMinY,double aMaxX,double aMaxY,CoordType aCoordType,int32_t aMarginInPixels = 0,int32_t aMinScaleDenominator = 0);
    Result SetView(const MapObject* const* aMapObjectArray,size_t aMapObjectCount,int32_t aMarginInPixels,int32_t aMinScaleDenominator);
    Result SetView(const MapObjectArray& aMapObjectArray,int32_t aMarginInPixels,int32_t aMinScaleDenominator);
    Result SetView(const MapObject& aMapObject,int32_t aMarginInPixels,int32_t aMinScaleDenominator);
    Result SetView(const Geometry& aGeometry,int32_t aMarginInPixels,int32_t aMinScaleDenominator);
    Result SetView(const ViewState& aViewState);
    Result SetViewToRoute(size_t aRouteIndex,int32_t aMarginInPixels,int32_t aMinScaleDenominator);
    Result SetViewToWholeMap();
    Result GetView(double& aMinX,double& aMinY,double& aMaxX,double& aMaxY,CoordType aCoordType) const;
    Result GetView(RectFP& aView,CoordType aCoordType) const;
    Result GetView(FixedSizeContourFP<4,true>& aView,CoordType aCoordType) const;
    Result GetMapExtent(double& aMinX,double& aMinY,double& aMaxX,double& aMaxY,CoordType aCoordType) const;
    Result GetMapExtent(RectFP& aMapExtent,CoordType aCoordType) const;
    String ProjectionAsProj4Param() const;
    CartoTypeCore::ViewState ViewState() const;
    Result SetViewLimits(double aMinScaleDenominator,double aMaxScaleDenominator,const Geometry& aGeometry);
    Result SetViewLimits(double aMinScaleDenominator = 0,double aMaxScaleDenominator = 0);

    Result InsertMapObject(uint32_t aMapHandle,const String& aLayerName,const Geometry& aGeometry,
                            const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result InsertPointMapObject(uint32_t aMapHandle,const String& aLayerName,double aX,double aY,
                                 CoordType aCoordType,const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result InsertCircleMapObject(uint32_t aMapHandle,const String& aLayerName,
                                  double aCenterX,double aCenterY,CoordType aCenterCoordType,double aRadius,CoordType aRadiusCoordType,
                                  const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result InsertEnvelopeMapObject(uint32_t aMapHandle,const String& aLayerName,const Geometry& aGeometry,
                                    double aRadius,CoordType aRadiusCoordType,
                                    const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    Result InsertPushPin(double aX,double aY,CoordType aCoordType,const String& aStringAttrib,const String& aColor,int32_t aIconCharacter,uint64_t& aId);
    Result InsertCopyOfMapObject(uint32_t aMapHandle,const String& aLayerName,const MapObject& aObject,double aEnvelopeRadius,CoordType aRadiusCoordType,uint64_t& aId,bool aReplace,
                                  String aExtraStringAttributes = nullptr,const FeatureInfo* aFeatureInfo = nullptr);
    Result DeleteMapObjects(uint32_t aMapHandle,uint64_t aStartId,uint64_t aEndId,uint64_t& aDeletedCount,String aCondition = nullptr);
    std::unique_ptr<MapObject> LoadMapObject(Result& aError,uint32_t aMapHandle,uint64_t aId);
    Result ReadGpx(uint32_t aMapHandle,const String& aFileName);
    Geometry Range(Result& aError,const RouteProfile* aProfile,double aX,double aY,CoordType aCoordType,double aTimeOrDistance,bool aIsTime);
    CartoTypeCore::TimeAndDistanceMatrix TimeAndDistanceMatrix(Result& aError,const std::vector<PointFP>& aFrom,const std::vector<PointFP>& aTo,CoordType aCoordType);
    CartoTypeCore::RouteAccess RouteAccess(Result& aError,const PointFP& aPoint,CoordType aCoordType);

    void EnableLayer(const String& aLayerName,bool aEnable);
    bool LayerIsEnabled(const String& aLayerName) const;
    void SetDisabledLayers(const std::set<String>& aLayerNames);
    std::vector<String> LayerNames();

    Result ConvertCoords(double* aCoordArray,size_t aCoordArraySize,CoordType aFromCoordType,CoordType aToCoordType) const;
    Result ConvertCoords(const WritableCoordSet& aCoordSet,CoordType aFromCoordType,CoordType aToCoordType) const;
    /** Converts the coordinates of a geometry object between any combination of lat/long, map coordinates and display pixels. */
    template<class T> Result ConvertCoords(GeneralGeometry<T>& aGeometry,CoordType aToCoordType)
        {
        if (aGeometry.CoordType() == aToCoordType)
            return KErrorNone;
        size_t contour_count = aGeometry.ContourCount();
        for (size_t i = 0; i < contour_count; i++)
            {
            WritableCoordSet cs{ aGeometry.CoordSet(i) };
            Result error = ConvertCoords(cs,aGeometry.CoordType(),aToCoordType);
            if (error)
                return error;
            }
        return KErrorNone;
        }
    Result ConvertPoint(double& aX,double& aY,CoordType aFromCoordType,CoordType aToCoordType) const;
    double PixelsToMeters(double aPixels) const;
    double MetersToPixels(double aMeters) const;
    String DataSetName() const;

    // interactive editing of map objects
    Result EditSetWritableMap(uint32_t aMapHandle);
    Result EditNewLineObject(const PointFP& aDisplayPoint);
    Result EditNewPolygonObject(const PointFP& aDisplayPoint);
    Result EditMoveCurrentPoint(const PointFP& aDisplayPoint);
    Result EditAddCurrentPoint();
    Result EditDeleteCurrentPoint();
    Result EditDeleteCurrentObject();
    Result EditSelectNearestPoint(const PointFP& aDisplayPoint,double aRadiusInMillimeters);
    Result EditInsertCurrentObject(const String& aLayer,uint64_t& aId,bool aReplace);
    Result EditSetCurrentObjectStringAttribute(const String& aKey,const String& aValue);
    Result EditSetCurrentObjectFeatureInfo(FeatureInfo aFeatureInfo);
    Result EditGetCurrentObjectAreaAndLength(double& aArea,double& aLength) const;

    // drawing the map
    const BitmapView* MapBitmap(Result& aError,bool* aRedrawWasNeeded = nullptr);
    const BitmapView* LabelBitmap(Result& aError,bool* aRedrawWasNeeded = nullptr);
    const BitmapView* MemoryDataBaseMapBitmap(Result& aError,bool* aRedrawWasNeeded = nullptr);
    void DrawNotices(GraphicsContext& aGc) const;
    void EnableDrawingMemoryDataBase(bool aEnable);
    void ForceRedraw();
    bool ClipBackgroundToMapBounds(bool aEnable);
    bool DrawBackground(bool aEnable);
    int32_t SetTileOverSizeZoomLevels(int32_t aLevels);
    Result DrawLabelsToLabelHandler(MLabelHandler& aLabelHandler,double aStyleSheetExclusionScale);
    bool ObjectWouldBeDrawn(Result& aError,uint64_t aId,MapObjectType aType,const String& aLayer,FeatureInfo aFeatureInfo,const String& aStringAttrib);
    bool SetDraw3DBuildings(bool aEnable);
    bool Draw3DBuildings() const;
    bool SetAnimateTransitions(bool aEnable);
    bool AnimateTransitions() const;

    // adding and removing style sheet icons loaded from files
    Result LoadIcon(const String& aFileName,const String& aId,const Point& aHotSpot,const Point& aLabelPos);
    void UnloadIcon(const String& aId);
        
    // drawing tiles
    Bitmap TileBitmap(Result& aError,int32_t aTileSizeInPixels,int32_t aZoom,int32_t aX,int32_t aY,const TileBitmapParam* aParam = nullptr);
    Bitmap TileBitmap(Result& aError,int32_t aTileSizeInPixels,const String& aQuadKey,const TileBitmapParam* aParam = nullptr);
    Bitmap TileBitmap(Result& aError,int32_t aTileWidth,int32_t aTileHeight,const RectFP& aBounds,CoordType aCoordType,const TileBitmapParam* aParam = nullptr);

    // finding map objects
    Result Find(MapObjectArray& aObjectArray,const FindParam& aFindParam) const;
    Result Find(MapObjectGroupArray& aObjectGroupArray,const FindParam& aFindParam) const;
    Result FindInDisplay(MapObjectArray& aObjectArray,size_t aMaxObjectCount,double aX,double aY,double aRadius) const;
    Result FindInLayer(MapObjectArray& aObjectArray,size_t aMaxObjectCount,
                        const String& aLayer,double aMinX,double aMinY,double aMaxX,double aMaxY,CoordType aCoordType) const;
    Result FindText(MapObjectArray& aObjectArray,size_t aMaxObjectCount,const String& aText,
                     StringMatchMethod aMatchMethod,const String& aLayers,const String& aAttribs) const;
    Result FindAddress(MapObjectArray& aObjectArray,size_t aMaxObjectCount,const Address& aAddress,bool aFuzzy = false) const;
    Result FindStreetAddresses(MapObjectArray& aObjectArray,size_t aMaxObjectCount,const Address& aAddress,const Geometry* aClip = nullptr) const;
    Result FindAddressPart(MapObjectArray& aObjectArray,size_t aMaxObjectCount,const String& aText,AddressPart aAddressPart,bool aFuzzy,bool aIncremental) const;
    Result FindBuildingsNearStreet(MapObjectArray& aObjectArray,const MapObject& aStreet) const;
    Result FindPolygonsContainingPath(MapObjectArray& aObjectArray,const Geometry& aPath,const FindParam* aParam = nullptr) const;
    Result FindPointsInPath(MapObjectArray& aObjectArray,const Geometry& aPath,const FindParam* aParam = nullptr) const;
    Result FindAsync(FindAsyncCallBack aCallBack,const FindParam& aFindParam,bool aOverride = false);
    Result FindAsync(FindAsyncGroupCallBack aCallBack,const FindParam& aFindParam,bool aOverride = false);
    Result FindAddressAsync(FindAsyncCallBack aCallBack,size_t aMaxObjectCount,const Address& aAddress,bool aFuzzy = false,bool aOverride = false);

    // geocoding
    Result GetGeoCodeSummary(String& aSummary,const MapObject& aMapObject) const;
    Result GetGeoCodeSummary(String& aSummary,double aX,double aY,CoordType aCoordType) const;
    Result GetAddress(Address& aAddress,const MapObject& aMapObject) const;
    Result GetAddressFast(Address& aAddress,const MapObject& aMapObject) const;
    Result GetAddress(Address& aAddress,double aX,double aY,CoordType aCoordType,bool aFullAddress = true) const;

    // terrain heights
    std::vector<int32_t> Heights(Result& aError,const CoordSet& aCoordSet,CoordType aCoordType) const;

    // style sheet variables
    void SetStyleSheetVariable(const String& aVariableName,const String& aValue);
    void SetStyleSheetVariable(const String& aVariableName,int32_t aValue);
    
    // access to graphics
    std::unique_ptr<GraphicsContext> CreateGraphicsContext(int32_t aWidth,int32_t aHeight);
    CartoTypeCore::Font Font(const FontSpec& aFontSpec);
    std::shared_ptr<GraphicsContext> MapGraphicsContext();

    /** The default size of the cache used by the image server. */
    static constexpr size_t KDefaultImageCacheSize = 10 * 1024 * 1024;

    // navigation

    /** The maximum number of alternative routes that can be displayed simultaneously. */
    static constexpr size_t KMaxRoutesDisplayed = 16;    // allow a number of alternative routes well in excess of the expected maximum, which is probably 3
    
    void SetPreferredRouterType(RouterType aRouterType);
    RouterType PreferredRouterType() const;
    RouterType ActualRouterType() const;
    Result StartNavigation(double aStartX,double aStartY,CoordType aStartCoordType,
                            double aEndX,double aEndY,CoordType aEndCoordType);
    Result StartNavigation(const RouteCoordSet& aCoordSet);
    Result StartNavigation(const CoordSet& aCoordSet,CoordType aCoordType);
    void EndNavigation();
    bool EnableNavigation(bool aEnable);
    bool NavigationEnabled() const;
    Result LoadNavigationData();
    bool NavigationDataHasGradients() const;
    Result SetMainProfile(const RouteProfile& aProfile);
    size_t BuiltInProfileCount();
    const RouteProfile* BuiltInProfile(size_t aIndex);
    Result SetBuiltInProfile(size_t aIndex);
    Result AddProfile(const RouteProfile& aProfile);
    Result ChooseRoute(size_t aRouteIndex);
    const RouteProfile* Profile(size_t aIndex) const;
    bool Navigating() const;
    void AddObserver(std::weak_ptr<MFrameworkObserver> aObserver);
    void RemoveObserver(std::weak_ptr<MFrameworkObserver> aObserver);
    Point RouteStart();
    Point RouteEnd();
    Result DisplayRoute(bool aEnable = true);
    const CartoTypeCore::Route* Route() const;
    const CartoTypeCore::Route* Route(size_t aIndex) const;
    std::unique_ptr<CartoTypeCore::Route> CreateRoute(Result& aError,const RouteProfile& aProfile,const RouteCoordSet& aCoordSet);
    std::unique_ptr<CartoTypeCore::Route> CreateRoute(Result& aError,const RouteProfile& aProfile,const CoordSet& aCoordSet,CoordType aCoordType);
    std::unique_ptr<CartoTypeCore::Route> CreateBestRoute(Result& aError,const RouteProfile& aProfile,const RouteCoordSet& aCoordSet,bool aStartFixed,bool aEndFixed,size_t aIterations);
    std::unique_ptr<CartoTypeCore::Route> CreateBestRoute(Result& aError,const RouteProfile& aProfile,const CoordSet& aCoordSet,CoordType aCoordType,bool aStartFixed,bool aEndFixed,size_t aIterations);
    std::unique_ptr<CartoTypeCore::Route> CreateRouteFromXml(Result& aError,const RouteProfile& aProfile,const String& aFileNameOrData);
    std::unique_ptr<CartoTypeCore::Route> CreateRouteHelper(Result& aError,bool aBest,const RouteProfile& aProfile,const RouteCoordSet& aCoordSet,bool aStartFixed,bool aEndFixed,size_t aIterations);
    std::unique_ptr<CartoTypeCore::Route> CreateRouteHelper(Result& aError,bool aBest,const RouteProfile& aProfile,const std::vector<Router::TRoutePointInternal>& aRoutePointArray,bool aStartFixed,bool aEndFixed,size_t aIterations);
    Result CreateRouteAsync(RouterAsyncCallBack aCallback,const RouteProfile& aProfile,const RouteCoordSet& aCoordSet,bool aOverride = false);
    Result CreateBestRouteAsync(RouterAsyncCallBack aCallback,const RouteProfile& aProfile,const RouteCoordSet& aCoordSet,bool aStartFixed,bool aEndFixed,size_t aIterations,bool aOverride = false);
    Result CreateRouteAsyncHelper(RouterAsyncCallBack aCallback,bool aBest,const RouteProfile& aProfile,const RouteCoordSet& aCoordSet,bool aStartFixed,bool aEndFixed,size_t aIterations,bool aOverride = false);
    String RouteInstructions(const CartoTypeCore::Route& aRoute) const;
    Result UseRoute(const CartoTypeCore::Route& aRoute,bool aReplace);
    Result ReadRouteFromXml(const String& aFileNameOrData,bool aReplace);
    Result WriteRouteAsXml(const CartoTypeCore::Route& aRoute,const String& aFileName,FileType aFileType = FileType::CTROUTE) const;
    Result WriteRouteAsXmlString(const CartoTypeCore::Route& aRoute,std::string& aXmlString,FileType aFileType = FileType::CTROUTE) const;
    const RouteSegment* CurrentRouteSegment() const;
    const RouteSegment* NextRouteSegment() const;
    size_t RouteCount() const;
    Result ReverseRoutes();
    Result DeleteRoutes();
    CartoTypeCore::RouteCreationData RouteCreationData() const;
    Result Navigate(const NavigationData& aNavData);
    const NavigatorTurn& FirstTurn() const;
    const NavigatorTurn& SecondTurn() const;
    const NavigatorTurn& ContinuationTurn() const;
    CartoTypeCore::NavigationState NavigationState() const;
    void SetNavigatorParam(const CartoTypeCore::NavigatorParam& aParam);
    CartoTypeCore::NavigatorParam NavigatorParam() const;
    void SetLocationMatchParam(const CartoTypeCore::LocationMatchParam& aParam);
    CartoTypeCore::LocationMatchParam LocationMatchParam() const;
    void SetNavigationMinimumFixDistance(int32_t aMeters);
    void SetNavigationTimeOffRouteTolerance(int32_t aSeconds);
    void SetNavigationDistanceOffRouteTolerance(int32_t aMeters);
    void SetNavigationAutoReRoute(bool aAutoReRoute);
    uint32_t SetNearbyObjectWarning(Result& aError,uint32_t aId,const String& aLayer,const String& aCondition,double aMaxDistance,size_t aMaxObjectCount);
    uint32_t SetVehicleTypeWarning(Result& aError,double aMaxDistance,size_t aMaxObjectCount);
    bool DeleteNearbyObjectWarning(uint32_t aId);
    bool ClearNearbyObjectWarnings();
    MapObjectArray CopyNearbyObjects();
    double DistanceToDestination();
    double EstimatedTimeToDestination();
    void UseSerializedNavigationData(bool aEnable);
    Result FindNearestRoad(NearestRoadInfo& aInfo,double aX,double aY,CoordType aCoordType,double aHeadingInDegrees,bool aDisplayPosition);
    Result DisplayPositionOnNearestRoad(const NavigationData& aNavData,NearestRoadInfo* aInfo = nullptr);
    void SetVehiclePosOffset(double aXOffset,double aYOffset);
    Result SetFollowMode(CartoTypeCore::FollowMode aFollowMode);
    CartoTypeCore::FollowMode FollowMode() const;
    Result GetNavigationPosition(PointFP& aPos,CoordType aCoordType) const;
    Result GetNavigationData(NavigationData& aData,double& aHeading) const;

    // locales
    void SetLocale(const char* aLocale);
    std::string Locale() const;

    // locale-dependent and configuration-dependent string handling
    void AppendDistance(MString& aString,double aDistanceInMeters,bool aMetricUnits,bool aAbbreviate = false) const;
    String DistanceToString(double aDistanceInMeters,bool aMetricUnits,bool aAbbreviate = false) const;
    void AppendTime(MString& aString,double aTimeInSeconds) const;
    String TimeToString(double aTimeInSeconds) const;
    void SetCase(MString& aString,LetterCase aCase);
    void AbbreviatePlacename(MString& aString);

    // traffic information and general location referencing
    Result AddTrafficInfo(uint64_t& aId,const TrafficInfo& aTrafficInfo,LocationRef& aLocationRef);
    double TrafficInfoSpeed(double aX,double aY,CoordType aCoordType,uint32_t aVehicleTypes);
    Result AddPolygonSpeedLimit(uint64_t& aId,const Geometry& aPolygon,double aSpeed,uint32_t aVehicleTypes);
    Result AddLineSpeedLimit(uint64_t& aId,const Geometry& aLine,double aSpeed,uint32_t aVehicleTypes);
    Result AddClosedLineSpeedLimit(uint64_t& aId,const Geometry& aLine,double aSpeed,uint32_t aVehicleTypes);
    Result AddForbiddenArea(uint64_t& aId,const Geometry& aPolygon);
    Result DeleteTrafficInfo(uint64_t aId);
    void ClearTrafficInfo();
    Result WriteTrafficMessageAsXml(MOutputStream& aOutput,const TrafficInfo& aTrafficInfo,LocationRef& aLocationRef);
    Result WriteLineTrafficMessageAsXml(MOutputStream& aOutput,const TrafficInfo& aTrafficInfo,const String& aId,const CartoTypeCore::Route& aRoute);
    Result WriteClosedLineTrafficMessageAsXml(MOutputStream& aOutput,const TrafficInfo& aTrafficInfo,const String& aId,const CartoTypeCore::Route& aRoute);
    bool EnableTrafficInfo(bool aEnable);
    bool TrafficInfoEnabled() const;

    // tracking
    void StartTracking();
    void EndTracking();
    void DeleteTrack();
    bool Tracking() const;
    Result DisplayTrack(bool aEnable);
    bool TrackIsDisplayed() const;
    TrackGeometry Track() const;
    double TrackLengthInMeters() const;
    Result WriteTrackAsXml(const String& aFileName) const;
    Result WriteTrackAsXmlString(std::string& aXmlString) const;

    // functions for internal use only
    std::shared_ptr<CMapStyle> CreateStyleSheet(double aScale);
    std::unique_ptr<CMapStore> NewMapStore(std::shared_ptr<CMapStyle> aStyleSheet,const Rect& aBounds,bool aUseFastAllocator);
    /** Returns the main map database. For internal use only. */
    CMapDataBase& MainDb() const { return iMapDataSet->MainDb(); }
    Transform3D MapTransform() const;
    AffineTransform MapTransform2D() const;
    Transform3D PerspectiveTransform() const;
    /** Returns the CEngine object used by this framework. For internal use only. */
    std::shared_ptr<CEngine> Engine() const { return iEngine->Engine(); }
    /** Returns the CMap object owned by this framework. For internal use only.  */
    CMap& Map() const { return *iMap; }
    Color OutlineColor() const;
    std::unique_ptr<Framework> CreateLegendFramework(Result& aError);
    std::unique_ptr<Bitmap> CreateBitmapFromSvg(MInputStream& aInputStream,FileLocation& aErrorLocation,int32_t aForcedWidth = 0);
    /** Associates arbitrary data with this framework. Used by the Android SDK. */
    void SetUserData(std::shared_ptr<MUserData> aUserData) { iUserData = aUserData; }
    void SetLabelUpAngleInRadians(double aAngle);
    void SetLabelUpVector(PointFP aVector);
    PointFP LabelUpVector() const;
    size_t RouteCalculationCost() const;
    /** Returns the current map drawing parameters. For internal use only. */
    CMapDrawParam& MapDrawParam() const { return *iMapDrawParam; }
    double PolygonArea(const CoordSet& aCoordSet,CoordType aCoordType);
    double PolylineLength(const CoordSet& aCoordSet,CoordType aCoordType);
    Result GetAreaAndLength(const Geometry& aGeometry,double& aArea,double& aLength);
    Result GetContourAreaAndLength(const Geometry& aGeometry,size_t aContourIndex,double& aArea,double& aLength);
    double Pixels(double aSize,const char* aUnit) const;

    private:
    Framework();
    
    Framework(const Framework&) = delete;
    Framework(Framework&&) = delete;
    void operator=(const Framework&) = delete;
    void operator=(Framework&&) = delete;

    Result Construct(const Param& aParam);
    void HandleChangedMapData();
    void InvalidateMapBitmaps() { iMapBitmapType = TMapBitmapType::None; }
    void HandleChangedView();
    void HandleChangedLayer() { InvalidateMapBitmaps(); LayerChanged(); }
    void CreateTileServer(int32_t aTileWidthInPixels,int32_t aTileHeightInPixels);
    void SetRoutePositionAndVector(const Point& aPos,const Point& aVector);
    void CreateNavigator();
    void InstallNavigator(std::unique_ptr<CNavigator> aNavigator);
    void SetCameraParam(CameraParam& aCameraParam,double aViewWidth,double aViewHeight);
    void InsertMapObject(uint32_t aMapHandle,const String& aLayerName,const MPath& aGeometry,
                         const String& aStringAttributes,FeatureInfo aFeatureInfo,uint64_t& aId,bool aReplace);
    void InsertTrackObject();
    void CreateMapObjectGroupArray(MapObjectGroupArray& aObjectGroupArray,MapObjectArray& aObjectArray,const FindParam& aFindParam) const;
    void EnforcePanAndZoomLimits();
    void AddNearbyObjectsToMap();
    void ConvertCoordsInternal(double* aCoordArray,size_t aCoordArraySize,CoordType aFromCoordType,CoordType aToCoordType) const;
    void ConvertPointInternal(double& aX,double& aY,CoordType aFromCoordType,CoordType aToCoordType) const;
    std::vector<Router::TRoutePointInternal> CreateRoutePointArray(const RouteCoordSet& aRouteCoordSet);
    
    // Notifying observers.
    void NotifyObservers(std::function<void(MFrameworkObserver&)>);
    void DeleteNullObservers();
    void ViewChanged() { NotifyObservers([](MFrameworkObserver& aP) { aP.OnViewChange(); }); }
    void MainDataChanged() { NotifyObservers([](MFrameworkObserver& aP) { aP.OnMainDataChange(); }); }
    void DynamicDataChanged() { NotifyObservers([](MFrameworkObserver& aP) { aP.OnDynamicDataChange(); }); }
    void StyleChanged() { NotifyObservers([](MFrameworkObserver& aP) { aP.OnStyleChange(); }); }
    void LayerChanged() { NotifyObservers([](MFrameworkObserver& aP) { aP.OnLayerChange(); }); }
    void NoticeChanged() { NotifyObservers([](MFrameworkObserver& aP) { aP.OnNoticeChange(); }); }

    // virtual functions from MNavigatorObserver
    void OnRoute(const CartoTypeCore::Route* aRoute) override;
    void OnTurn(const NavigatorTurn& aFirstTurn,
                const NavigatorTurn* aSecondTurn,
                const NavigatorTurn* aContinuationTurn) override;
    void OnState(CartoTypeCore::NavigationState aState) override;
    
    void ChangeStyleSheet(const CartoTypeCore::StyleSheetDataArray& aStyleSheetData,const VariableDictionary* aStyleSheetVariables = nullptr,const CartoTypeCore::BlendStyleSet* aBlendStyle = nullptr);
    void ClearTurns();

    std::shared_ptr<FrameworkEngine> iEngine;
    std::shared_ptr<FrameworkMapDataSet> iMapDataSet;
    std::shared_ptr<CMap> iMap;
    std::shared_ptr<CMapDrawParam> iMapDrawParam;
    std::shared_ptr<C32BitColorBitmapGraphicsContext> iGc;
    std::unique_ptr<CPerspectiveGraphicsContext> iPerspectiveGc;
    CartoTypeCore::PerspectiveParam iPerspectiveParam;

    enum class TMapBitmapType
        {
        None,   // the map bitmap is invalid
        Full,   // the map bitmap is valid
        Memory, // the map bitmap has memory map data only
        Label   // the map bitmap has labels only
        };

    TMapBitmapType iMapBitmapType = TMapBitmapType::None;
    bool iPerspective = false;
    bool iUseSerializedNavigationData = true;
    RouterType iPreferredRouterType = RouterType::Default;
    std::unique_ptr<CNavigatorFuture> iNavigatorFuture;
    std::unique_ptr<CNavigator> iNavigator;
    std::vector<std::weak_ptr<MFrameworkObserver>> iObservers;
    std::vector<RouteProfile> iBuiltInRouteProfileArray;
    std::vector<uint64_t> iRouteObjects;
    std::vector<uint64_t> iNearbyObjects;
    uint64_t iRoutePositionObjectId = 0;
    uint64_t iRouteVectorObjectId = 0;
    bool iTracking = false;
    bool iDisplayTrack = false;
    uint64_t iTrackObjectId = 0;
    TrackGeometry iTrack = TrackGeometry(CoordType::Degree);
    std::weak_ptr<Legend> iTurnInstructionNotice;
    NavigatorTurn iFirstTurn;
    NavigatorTurn iSecondTurn;
    NavigatorTurn iContinuationTurn;
    CartoTypeCore::NavigationState iNavigationState = NavigationState::None;
    CartoTypeCore::NavigatorParam iNavigatorParam;
    CartoTypeCore::LocationMatchParam iLocationMatchParam;
    std::vector<RouteProfile> iRouteProfile;
    CartoTypeCore::RouteCreationData iRouteCreationData;
    PointFP iVehiclePosOffset;
    std::shared_ptr<CTileServer> iTileServer;
    int32_t iTileServerOverSizeZoomLevels = 2;
    std::string iLocale;
    CartoTypeCore::FollowMode iFollowMode = FollowMode::LocationHeadingZoom;
    bool iMapsOverlap = true;
    std::unique_ptr<CAsyncFinder> iAsyncFinder;
    std::unique_ptr<CAsyncRouter> iAsyncRouter;
    Geometry iPanArea;
    FileLocation iStyleSheetErrorLocation;
    std::unique_ptr<CMapObjectEditor> iMapObjectEditor;
    std::shared_ptr<MUserData> iUserData;
    };

/** A map renderer using OpenGL ES graphics acceleration. */
class MapRenderer
    {
    public:
    /**
    Constructs a renderer object that can be used to draw a map into a display which supports OpenGL ES drawing.
    The MapRenderer::Draw function should be called to draw the map. You can use the framework object freely; any calls
    to functions which affect the view will automatically be reflected by the Draw() function.

    If aNativeWindow is non-null it is used to create an EGL context for that window, into which all drawing is done
    using a separate thread which calls Draw() 30 times per second. This feature is supported on Windows only.
    */
    MapRenderer(Framework& aFramework,const void* aNativeWindow = nullptr);
    ~MapRenderer();
    /** Draws the map using OpenGL ES. */
    void Draw();
    /** Returns true if this map renderer is valid. If it returns false, graphics acceleration is not enabled. */
    bool Valid() const;
    /**
    Enables or disables drawing by a separate thread. Returns the previous state.
    This function is intended for users who need the full capacity of the GPU
    for a period when drawing is unnecessary.
    When drawing is disabled, draw events can be handled by calls to Draw, but it is also necessary
    to create a timer to redraw occasionally (e.g., once per second) to allow missing tiles to be
    created and drawn after pans, zooms and other changes affecting the display.
    */
    bool Enable(bool aEnable);

    private:
    std::unique_ptr<CMapRendererImplementation> m_implementation;
    };

String UKGridReferenceFromMapPoint(const PointFP& aPointInMapMeters,int32_t aDigits);
String UKGridReferenceFromDegrees(const PointFP& aPointInDegrees,int32_t aDigits);
RectFP MapRectFromUKGridReference(const String& aGridReference);
PointFP MapPointFromUKGridReference(const String& aGridReference);
PointFP PointInDegreesFromUKGridReference(const String& aGridReference);
/**
Expands a street name by replacing abbreviations with their full forms. For example, St is replaced by Street.
This function cannot of course know whether St should actually be replaced by Saint. Its purpose
is to aid address searching.
*/
String ExpandStreetName(const MString& aText);

}
