/*
cartotype_legend.h
Copyright (C) 2015-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_graphics_context.h>
#include <cartotype_framework_observer.h>
#include <vector>
#include <memory>

namespace CartoTypeCore
{

class LegendObjectParam;
class Framework;
class ThreadSafeNavigationState;

/**
The Legend class creates bitmaps showing sample map objects, with optional text lines and a scale,
to be used as a map legend or for visualisation in the style sheet editor.

Dimensions are specified with units, which may be "pt" (point), "pc" (pica), "cm" (centimetre),
"mm" (millimetre), "in" (inch), or "px" (pixel). An empty or unrecognised unit is taken as "px".

It's relatively expensive to construct a Legend object, so it is best to keep such an object in
existence rather than creating one as a temporary object in a draw loop.
*/
class Legend: public MFrameworkObserver
    {
    public:
    Legend(Framework& aFramework);
    ~Legend();
    /** Creates a Legend object by copying another one. */
    Legend(const Legend& aOther);
    /** The assignment operator: copies aOther to this Legend. */
    Legend& operator=(const Legend& aOther);
    
    std::unique_ptr<Bitmap> CreateLegend(Result& aError,double aWidth,const char* aUnit,double aScaleDenominator,double aScaleDenominatorInView,const BlendStyleSet* aBlendStyleSet = nullptr);
    void Clear();
    int32_t Pixels(double aDimension,const char* aUnit);

    void AddMapObjectLine(MapObjectType aType,const String& aLayer,FeatureInfo aFeatureInfo,const String& aStringAttrib,const String& aLabel);
    void AddTextLine(const String& aText);
    void AddScaleLine(bool aMetricUnits);
    void AddTurnLine(bool aMetricUnits,bool aAbbreviate);

    void SetMainStyleSheet(const uint8_t* aData,size_t aLength);
    void SetExtraStyleSheet(const uint8_t* aData,size_t aLength);
    void SetBackgroundColor(Color aColor);
    void SetBorder(Color aColor,double aStrokeWidth,double aRadius,const char* aUnit);
    void SetMarginWidth(double aMarginWidth,const char* aUnit);
    void SetMinLineHeight(double aLineHeight,const char* aUnit);
    void SetLabelWrapWidth(double aWrapWidth,const char* aUnit);
    void SetFontFamily(const String& aFontFamily);
    void SetFontSize(double aFontSize,const char* aUnit);
    void SetTextColor(Color aTextColor);
    void SetDiagramColor(Color aDiagramColor);
    void SetAlignment(Align aAlignment);
    void SetPolygonRotation(double aDegrees);
    bool HasTurnInstruction() const;
    void SetTurnInstruction(const MString& aText);
    String TurnInstruction();
    uint32_t Serial() const;
    std::shared_ptr<ThreadSafeNavigationState> NavigationState() const;
    void SetNavigationState(std::shared_ptr<ThreadSafeNavigationState> aState);

    private:
    std::unique_ptr<Bitmap> CreateLegendInternal(double aWidth,const char* aUnit,double aScaleDenominator,double aScaleDenominatorInView,const BlendStyleSet* aBlendStyleSet);
    void DrawScale(GraphicsContext& aGc,const LegendObjectParam& aParam,int32_t aX,int32_t aY,int32_t aWidth,Color aBlendColor);
    void Copy(const Legend& aOther);

    // virtual functions from MNavigatorObserver
    void OnRoute(const Route* aRoute) override;
    void OnTurn(const NavigatorTurn& aFirstTurn,const NavigatorTurn* aSecondTurn,const NavigatorTurn* aContinuationTurn,double aDistanceLeft,double aTimeLeft) override;
    void OnState(CartoTypeCore::NavigationState aState) override;

    std::unique_ptr<Framework> m_framework;
    std::vector<LegendObjectParam> m_object_array;
    std::shared_ptr<ThreadSafeNavigationState> m_navigation_state;
    Color m_background_color { KWhite };
    Color m_border_color { KGray };
    int32_t m_border_width_in_pixels { };
    int32_t m_border_radius_in_pixels { };
    int32_t m_margin_width_in_pixels { };
    int32_t m_min_line_height_in_pixels { };
    int32_t m_label_wrap_width_in_pixels { };
    FontSpec m_font_spec;
    Color m_color { KGray };
    Color m_diagram_color { KGray };
    Align m_alignment = Align::Left;
    double m_polygon_rotation { 0 };
    double m_metre { };
    double m_pt { };
    double m_inch { };
    };

}
