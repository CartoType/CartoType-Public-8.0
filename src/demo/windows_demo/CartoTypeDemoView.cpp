
// CartoTypeDemoView.cpp : implementation of the CCartoTypeDemoView class
//

#include "stdafx.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CartoTypeDemo.h"
#endif

#include "CartoTypeDemoDoc.h"
#include "CartoTypeDemoView.h"
#include "FindDialog.h"
#include "LatLongDialog.h"
#include "LayerDialog.h"
#include "SetScaleDialog.h"
#include "RouteDialog.h"
#include "MainFrm.h"
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCartoTypeDemoView

IMPLEMENT_DYNCREATE(CCartoTypeDemoView, CView)

BEGIN_MESSAGE_MAP(CCartoTypeDemoView, CView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_COMMAND(ID_APP_ABOUT,&CCartoTypeDemoView::OnAppAbout)
    ON_COMMAND(ID_FILE_OPEN_IN_CURRENT_MAP,&CCartoTypeDemoView::OnFileOpenInCurrentMap)
    ON_COMMAND(ID_FILE_CLOSE,&CCartoTypeDemoView::OnFileClose)
    ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE,&CCartoTypeDemoView::OnUpdateFileClose)
    ON_COMMAND(ID_FILE_SAVE_AS_PNG,&CCartoTypeDemoView::OnFileSaveAsPng)
    ON_COMMAND(ID_FILE_SAVE_AS_CTSQL,&CCartoTypeDemoView::OnFileSaveAsCtsql)
    ON_COMMAND(ID_FIND,&CCartoTypeDemoView::OnFind)
    ON_UPDATE_COMMAND_UI(ID_FIND,&CCartoTypeDemoView::OnUpdateFind)
    ON_COMMAND(ID_FIND_ADDRESS,&CCartoTypeDemoView::OnFindAddress)
    ON_UPDATE_COMMAND_UI(ID_FIND_ADDRESS,&CCartoTypeDemoView::OnUpdateFindAddress)
    ON_COMMAND(ID_FIND_NEXT,&CCartoTypeDemoView::OnFindNext)
    ON_UPDATE_COMMAND_UI(ID_FIND_NEXT,&CCartoTypeDemoView::OnUpdateFindNext)
    ON_COMMAND(ID_VIEW_RELOAD_STYLE_SHEET,&CCartoTypeDemoView::OnViewReloadStyleSheet)
    ON_UPDATE_COMMAND_UI(ID_VIEW_RELOAD_STYLE_SHEET,&CCartoTypeDemoView::OnUpdateViewReloadStyleSheet)
    ON_COMMAND(ID_VIEW_ZOOM_IN,&CCartoTypeDemoView::OnViewZoomIn)
    ON_COMMAND(ID_VIEW_ZOOM_OUT,&CCartoTypeDemoView::OnViewZoomOut)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN,&CCartoTypeDemoView::OnUpdateViewZoomIn)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT,&CCartoTypeDemoView::OnUpdateViewZoomOut)
    ON_COMMAND(ID_VIEW_ROTATE,&CCartoTypeDemoView::OnViewRotate)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE,&CCartoTypeDemoView::OnUpdateViewRotate)
    ON_COMMAND(ID_NORTH_UP,&CCartoTypeDemoView::OnViewNorthup)
    ON_UPDATE_COMMAND_UI(ID_NORTH_UP,&CCartoTypeDemoView::OnUpdateViewNorthup)
    ON_COMMAND(ID_VIEW_LATLONG,&CCartoTypeDemoView::OnViewLatlong)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LATLONG,&CCartoTypeDemoView::OnUpdateViewLatlong)
    ON_COMMAND(ID_VIEW_STYLESHEET,&CCartoTypeDemoView::OnViewStylesheet)
    ON_UPDATE_COMMAND_UI(ID_VIEW_STYLESHEET,&CCartoTypeDemoView::OnUpdateViewStylesheet)
    ON_COMMAND(ID_VIEW_LAYERS,&CCartoTypeDemoView::OnViewLayers)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LAYERS,&CCartoTypeDemoView::OnUpdateViewLayers)
    ON_COMMAND(ID_VIEW_LEGEND,&CCartoTypeDemoView::OnViewLegend)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LEGEND,&CCartoTypeDemoView::OnUpdateViewLegend)
    ON_COMMAND(ID_VIEW_SET_SCALE,&CCartoTypeDemoView::OnViewSetScale)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SET_SCALE,&CCartoTypeDemoView::OnUpdateViewSetScale)
    ON_COMMAND(ID_VIEW_PERSPECTIVE,&CCartoTypeDemoView::OnViewPerspective)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE,&CCartoTypeDemoView::OnUpdateViewPerspective)
    ON_COMMAND(ID_VIEW_METRIC_UNITS,&CCartoTypeDemoView::OnViewMetricUnits)
    ON_UPDATE_COMMAND_UI(ID_VIEW_METRIC_UNITS,&CCartoTypeDemoView::OnUpdateViewMetricUnits)
    ON_COMMAND(ID_VIEW_DRIVING_INSTRUCTIONS,&CCartoTypeDemoView::OnViewTurnInstructions)
    ON_UPDATE_COMMAND_UI(ID_VIEW_DRIVING_INSTRUCTIONS,&CCartoTypeDemoView::OnUpdateViewTurnInstructions)
    ON_COMMAND(ID_VIEW_FLY_THROUGH,&CCartoTypeDemoView::OnViewFlyThrough)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FLY_THROUGH,&CCartoTypeDemoView::OnUpdateViewFlyThrough)
    ON_COMMAND(ID_GO_ALONG_ROUTE,&CCartoTypeDemoView::OnGoAlongRoute)
    ON_UPDATE_COMMAND_UI(ID_GO_ALONG_ROUTE,&CCartoTypeDemoView::OnUpdateGoAlongRoute)
    ON_COMMAND(ID_ROUTE_REVERSE_ROUTE,&CCartoTypeDemoView::OnReverseRoute)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_REVERSE_ROUTE,&CCartoTypeDemoView::OnUpdateReverseRoute)
    ON_COMMAND(ID_ROUTE_DELETE_ROUTE,&CCartoTypeDemoView::OnDeleteRoute)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_DELETE_ROUTE,&CCartoTypeDemoView::OnUpdateDeleteRoute)
    ON_COMMAND(ID_ROUTE_DRIVE,&CCartoTypeDemoView::OnSetDriveProfile)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_DRIVE,&CCartoTypeDemoView::OnUpdateSetDriveProfile)
    ON_COMMAND(ID_ROUTE_CYCLE,&CCartoTypeDemoView::OnSetCycleProfile)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_CYCLE,&CCartoTypeDemoView::OnUpdateSetCycleProfile)
    ON_COMMAND(ID_ROUTE_WALK,&CCartoTypeDemoView::OnSetWalkProfile)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_WALK,&CCartoTypeDemoView::OnUpdateSetWalkProfile)
    ON_COMMAND(ID_ROUTE_HIKE,&CCartoTypeDemoView::OnSetHikeProfile)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_HIKE,&CCartoTypeDemoView::OnUpdateSetHikeProfile)
    ON_COMMAND(ID_ROUTE_SKI,&CCartoTypeDemoView::OnSetSkiProfile)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_SKI,&CCartoTypeDemoView::OnUpdateSetSkiProfile)
    ON_COMMAND(ID_ROUTE_SHORTEST,&CCartoTypeDemoView::OnShortestRoute)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_SHORTEST,&CCartoTypeDemoView::OnUpdateShortestRoute)
    ON_COMMAND(ID_ROUTE_USEGRADIENTS,&CCartoTypeDemoView::OnUseGradients)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_USEGRADIENTS,&CCartoTypeDemoView::OnUpdateUseGradients)
    ON_COMMAND(ID_ROUTE_SAVE_INSTRUCTIONS,&CCartoTypeDemoView::OnSaveRouteInstructions)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_SAVE_INSTRUCTIONS,&CCartoTypeDemoView::OnUpdateSaveRouteInstructions)
    ON_COMMAND(ID_ROUTE_SAVE_ROUTE_XML,&CCartoTypeDemoView::OnSaveRouteXml)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_SAVE_ROUTE_XML,&CCartoTypeDemoView::OnUpdateSaveRouteXml)
    ON_COMMAND(ID_ROUTE_SAVE_ROUTE_GPX,&CCartoTypeDemoView::OnSaveRouteGpx)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_SAVE_ROUTE_GPX,&CCartoTypeDemoView::OnUpdateSaveRouteGpx)
    ON_COMMAND(ID_ROUTE_LOAD_ROUTE_XML,&CCartoTypeDemoView::OnLoadRouteXml)
    ON_UPDATE_COMMAND_UI(ID_ROUTE_LOAD_ROUTE_XML,&CCartoTypeDemoView::OnUpdateLoadRouteXml)
    ON_COMMAND(ID_EDIT_LOAD_KML,&CCartoTypeDemoView::OnEditLoadKml)
    ON_UPDATE_COMMAND_UI(ID_EDIT_LOAD_KML,&CCartoTypeDemoView::OnUpdateEditLoadKml)
    ON_COMMAND(ID_EDIT_SAVE_KML,&CCartoTypeDemoView::OnEditSaveKml)
    ON_UPDATE_COMMAND_UI(ID_EDIT_SAVE_KML,&CCartoTypeDemoView::OnUpdateEditSaveKml)
    ON_COMMAND(ID_EDIT_LOAD_CTMS,&CCartoTypeDemoView::OnEditLoadCtms)
    ON_UPDATE_COMMAND_UI(ID_EDIT_LOAD_CTMS,&CCartoTypeDemoView::OnUpdateEditLoadCtms)
    ON_COMMAND(ID_EDIT_SAVE_CTMS,&CCartoTypeDemoView::OnEditSaveCtms)
    ON_UPDATE_COMMAND_UI(ID_EDIT_SAVE_CTMS,&CCartoTypeDemoView::OnUpdateEditSaveCtms)
    ON_COMMAND(ID_EDIT_LOAD_CTSQL,&CCartoTypeDemoView::OnEditLoadCtsql)
    ON_UPDATE_COMMAND_UI(ID_EDIT_LOAD_CTSQL,&CCartoTypeDemoView::OnUpdateEditLoadCtsql)
    ON_COMMAND(ID_EDIT_SAVE_CTSQL,&CCartoTypeDemoView::OnEditSaveCtsql)
    ON_UPDATE_COMMAND_UI(ID_EDIT_SAVE_CTSQL,&CCartoTypeDemoView::OnUpdateEditSaveCtsql)
    ON_COMMAND(ID_EDIT_LOAD_GPX,&CCartoTypeDemoView::OnEditLoadGpx)
    ON_UPDATE_COMMAND_UI(ID_EDIT_LOAD_GPX,&CCartoTypeDemoView::OnUpdateEditLoadGpx)
END_MESSAGE_MAP()

// CCartoTypeDemoView construction/destruction

CCartoTypeDemoView::CCartoTypeDemoView()
    {
    m_route_point_array.resize(2);
    }

CCartoTypeDemoView::~CCartoTypeDemoView()
    {
    m_map_renderer = nullptr;
    m_route_iter = nullptr;
    m_framework = nullptr;
    m_extra_gc = nullptr;
    }

BOOL CCartoTypeDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

const CartoType::BitmapView* CCartoTypeDemoView::MapBitmap(CartoType::Result& aError,const CartoType::Rect& aMapClientArea,bool& aRedrawNeeded)
    {
    // Get the map bitmap.
    const CartoType::BitmapView* bitmap = m_framework->MapBitmap(aError,&aRedrawNeeded);

    /*
    If there are driving instructions or a legend, or there's a drag offset, copy the map bitmap into the extra GC,
    offset if necessary, and draw them on top.
    */
    if (!aError && (m_draw_turn_instructions || m_draw_legend || m_map_drag_offset.X || m_map_drag_offset.Y))
        {
        // Create the extra GC if necessary.
        if (m_extra_gc == nullptr || m_extra_gc->Bounds() != CartoType::Rect(0,0,bitmap->Width(),bitmap->Height()))
            m_extra_gc = m_framework->CreateGraphicsContext(bitmap->Width(),bitmap->Height());

        if (!aError)
            {
            ASSERT(bitmap->DataBytes() == m_extra_gc->Bitmap()->DataBytes());
            ASSERT(bitmap->RowBytes() == m_extra_gc->Bitmap()->RowBytes());

            // Copy the bitmap into the extra GC's bitmap. */
            if (m_map_drag_offset.X || m_map_drag_offset.Y)
                {
                // Set the destination bitmap to white so that the edges don't have relics of the previous map.
                memset(const_cast<uint8_t*>(m_extra_gc->Bitmap()->Data()),0xFF,m_extra_gc->Bitmap()->DataBytes());

                // Draw the source bitmap at the required offset.
                m_extra_gc->DrawBitmap(*bitmap,m_map_drag_offset);
                }
            else
                memcpy(const_cast<uint8_t*>(m_extra_gc->Bitmap()->Data()),bitmap->Data(),bitmap->DataBytes());

            // Draw the legend and copyright notice.
            if (m_draw_legend || m_draw_turn_instructions)
                m_framework->DrawNotices(*m_extra_gc);

            bitmap = m_extra_gc->Bitmap();
            }
        }

    return bitmap;
    }

void CCartoTypeDemoView::CreateLegend()
    {
    auto legend = std::make_unique<CartoType::Legend>(*m_framework);

    CartoType::Color grey(90,90,90);
    legend->SetTextColor(grey);
    legend->SetAlignment(CartoType::Align::Center);
    CartoType::String dataset_name(m_framework->DataSetName());
    m_framework->SetCase(dataset_name,CartoType::LetterCase::Lower);
    m_framework->SetCase(dataset_name,CartoType::LetterCase::Title);
    legend->SetFontSize(10,"pt");
    legend->AddTextLine(dataset_name);

    legend->SetAlignment(CartoType::Align::Right);
    legend->SetFontSize(6,"pt");
    CartoType::String s;
    s.SetAttribute("ref","M4");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"road/major",CartoType::FeatureType::Motorway,s,"motorway");
    s.SetAttribute("ref","A40");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"road/major",CartoType::FeatureType::TrunkRoad,s,"trunk road");
    s.SetAttribute("ref","A414");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"road/major",CartoType::FeatureType::PrimaryRoad,s,"primary road");
    s.SetAttribute("ref","B4009");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"road/mid",CartoType::FeatureType::SecondaryRoad,s,"secondary road");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"road/mid",CartoType::FeatureType::TertiaryRoad,"High Street","tertiary road");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"path",CartoType::FeatureType::Cycleway,"","cycleway");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"path",CartoType::FeatureType::Bridleway,"","bridle path");
    legend->AddMapObjectLine(CartoType::MapObjectType::Line,"path",CartoType::FeatureType::Path,"","footpath");
    legend->AddMapObjectLine(CartoType::MapObjectType::Polygon,"land/major",CartoType::FeatureType::Forestry,"Ashridge","forest or wood");
    legend->AddMapObjectLine(CartoType::MapObjectType::Polygon,"land/minor",CartoType::FeatureType::Park,"Green Park","park, golf course or common");
    legend->AddMapObjectLine(CartoType::MapObjectType::Polygon,"land/minor",CartoType::FeatureType::Grass,"","grassland");
    legend->AddMapObjectLine(CartoType::MapObjectType::Polygon,"land/minor",CartoType::FeatureType::Orchard,"","orchard, vineyard, etc.");
    legend->AddMapObjectLine(CartoType::MapObjectType::Polygon,"land/minor",CartoType::FeatureType::Commercial,"","commercial or industrial");
    legend->AddMapObjectLine(CartoType::MapObjectType::Polygon,"land/minor",CartoType::FeatureType::Construction,"","construction, quarry, landfill, etc.");
    legend->AddMapObjectLine(CartoType::MapObjectType::Point,"amenity/minor",CartoType::FeatureType::Station,"Berkhamsted","station");

    // Uncomment these lines to create a scale bar as part of the legend.
    // legend->SetAlignment(CartoType::Align::Center);
    // legend->AddScaleLine(iMetricUnits);

    legend->SetBorder(CartoType::KGray,1,4,"pt");
    CartoType::Color b(CartoType::KWhite);
    b.SetAlpha(224);
    legend->SetBackgroundColor(b);

    m_framework->SetLegend(std::move(legend),1,"in",CartoType::NoticePosition::TopRight);
    }

void CCartoTypeDemoView::OnDraw(CDC* pDC)
    {
    if (m_map_renderer)
        {
        m_map_renderer->Draw();
        return;
        }

    RECT clip;
    pDC->GetClipBox(&clip);
    CartoType::Rect dest_clip(clip.left,clip.top,clip.right,clip.bottom);

    RECT mc;
    GetClientRect(&mc);
    CartoType::Rect map_client_area(mc.left,mc.top,mc.right,mc.bottom);

    CartoType::Rect source_rect = map_client_area;
    CartoType::Rect dest_rect = map_client_area;

    // Return now if there is nothing to be drawn.
    dest_clip.Intersection(dest_rect);
    if (dest_clip.IsEmpty())
        return;
    CartoType::Rect source_clip(dest_clip);
    source_clip.Intersection(source_rect);
    if (source_clip.IsEmpty())
        return;

    // Get the map bitmap. If the image server is in use this will be the in-memory db only.
    CartoType::Result error;
    bool redraw_needed = false;
    const CartoType::BitmapView* map_bitmap = MapBitmap(error,map_client_area,redraw_needed);

    if (error)
        {
        CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();

        char buffer[256];
        auto error_string = CartoType::ErrorString(error);
        sprintf_s(buffer,"failed to draw map: %s",error_string.c_str());
        ::CString str(buffer);
        CMainFrame* main_window = (CMainFrame*)app->m_pMainWnd;
        main_window->SetMessageText(str);
        return;
        }

    // Blit the drawing surface to the window.
    BitmapInfoX bm;
    bm.bmiHeader.bV4Size = sizeof(BITMAPV4HEADER);
    bm.bmiHeader.bV4Width = map_bitmap->Width();
    bm.bmiHeader.bV4Height = -map_bitmap->Height();
    bm.bmiHeader.bV4Planes = 1;
    bm.bmiHeader.bV4BitCount = 32;
    bm.bmiHeader.bV4V4Compression = BI_BITFIELDS;
    bm.bmiHeader.bV4SizeImage = 0;
    bm.bmiHeader.bV4XPelsPerMeter = 1000;
    bm.bmiHeader.bV4YPelsPerMeter = 1000;
    bm.bmiHeader.bV4ClrUsed = 0;
    bm.bmiHeader.bV4ClrImportant = 0;
    bm.bmiHeader.bV4RedMask = 0xFF000000;
    bm.bmiHeader.bV4GreenMask = 0xFF0000;
    bm.bmiHeader.bV4BlueMask = 0xFF00;
    bm.bmiHeader.bV4AlphaMask = 0xFF;
    const unsigned char* map_data = (const unsigned char*)(map_bitmap->Data());

    /*
    Blit the whole GC if the clip rectangle is at the bottom left of the GC.
    This gets round a bug in StretchDIBits (when dealing with 32bpp bitmaps) that causes a y coordinate of 0 to
    be treated as the top, not the bottom, of the source bitmap.
    */
    if (source_clip.MinY() > 0 && source_clip.MinX() == 0 && source_clip.MaxY() == map_bitmap->Height())
        ::StretchDIBits(*pDC,
        0,0,map_bitmap->Width(),map_bitmap->Height(),
        0,0,map_bitmap->Width(),map_bitmap->Height(),
        map_data,(BITMAPINFO*)&bm,DIB_RGB_COLORS,SRCCOPY);
    else
        ::StretchDIBits(*pDC,dest_clip.MinX(),dest_clip.MinY(),dest_clip.Width(),dest_clip.Height(),
        source_clip.MinX(),map_bitmap->Height() - source_clip.MaxY(),source_clip.Width(),source_clip.Height(),
        map_data,(BITMAPINFO*)&bm,DIB_RGB_COLORS,SRCCOPY);
    }

void CCartoTypeDemoView::CalculateAndDisplayRoute()
    {
    CartoType::RouteCoordSet cs(CartoType::CoordType::Map);

    for (const auto& p : m_route_point_array)
        {
        CartoType::RoutePoint rp;
        rp = p.Point;
        cs.RoutePointArray.push_back(rp);
        }

    CartoType::Result error;
    CartoType::RouteProfile profile = *m_framework->Profile(0);
    if (m_best_route && cs.RoutePointArray.size() > 3)
        {
        size_t iterations = cs.RoutePointArray.size() * cs.RoutePointArray.size();
        if (iterations < 16)
            iterations = 16;
        else if (iterations > 256)
            iterations = 256;
        auto r = m_framework->CreateBestRoute(error,profile,cs,true,true,iterations);
        if (!error)
            error = m_framework->UseRoute(*r,true);
        }
    else
        {
        auto r = m_framework->CreateRoute(error,profile,cs);
        if (!error)
            error = m_framework->UseRoute(*r,true);
        
        if (!error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            CMainFrame* main_window = (CMainFrame*)app->m_pMainWnd;
            char buffer[256];
            auto data = m_framework->RouteCreationData();
            sprintf_s(buffer,"route creation = %gs, expansion = %gs, node queries = %d, node misses = %d, arc queries = %d, arc misses = %d",
                      data.RouteCalculationTime,data.RouteExpansionTime,
                      int(data.NodeCacheQueries),int(data.NodeCacheMisses),
                      int(data.ArcCacheQueries),int(data.ArcCacheMisses));
            CartoType::String s = buffer;
            s.Append((uint16_t)0);
            main_window->SetMessageText((LPCTSTR)s.Data());
            }
        }

    if (error)
        {
        CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
        app->ShowError(_TEXT("Routing error"),error);
        }
    else
        {
        if (m_framework->Route())
            m_route_iter.reset(new CartoType::RouteIterator(*m_framework->Route()));

        bool at_end = false;
        SimulateNavigationFix(false,at_end);
        }
    }

void CCartoTypeDemoView::OnRButtonUp(UINT nFlags, CPoint point)
    {
    CView::OnRButtonUp(nFlags,point);
    CartoType::PointFP p(point.x,point.y);

    // Get the address of the point clicked on.
    CartoType::Address address;
    m_framework->GetAddress(address,point.x,point.y,CartoType::CoordType::Display);
    CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
    CMainFrame* main_window = (CMainFrame*)app->m_pMainWnd;

    CartoType::String address_string;
    m_framework->ConvertPoint(p.X,p.Y,CartoType::CoordType::Display,CartoType::CoordType::Degree);
    address_string = address.ToString(false,&p);

    address_string.Append((uint16_t)0);
    main_window->SetMessageText((LPCTSTR)address_string.Data());

    if (address_string.Length())
        address_string.Delete(address_string.Length() - 1,address_string.Length());
    address_string.Append("\r\n\r\n[");
    address_string += address.ToStringWithLabels();
    address_string.Append("]");

    // Add the altitude if available.
    CartoType::CoordSet cs(&p,1);
    CartoType::Result error;
    auto alt = m_framework->Heights(error,cs,CartoType::CoordType::Degree);
    if (!error)
        {
        char buffer[32];
        sprintf_s(buffer," (altitude = %dm)",alt[0]);
        address_string.Append(buffer);
        }

    // If possible, get the Ordnance Survey National Grid reference.
    CartoType::String os_grid_ref = CartoType::UKGridReferenceFromDegrees(p,3);
    if (os_grid_ref.Length())
        {
        address_string.Append("\r\n\r\nOrdnance Survey of Great Britain grid reference: ");
        address_string.Append(os_grid_ref);
        }

    // Get information about the objects clicked on - within 2mm.
    CartoType::MapObjectArray object;
    double pixel_mm = m_framework->ResolutionDpi() / 25.4;
    m_framework->FindInDisplay(object,100,point.x,point.y,int(ceil(2 * pixel_mm)));
    CartoType::MapObject* pushpin = nullptr;

    // See if we have a pushpin, and geocode the objects so that they can be sorted.
    for (auto& o : object)
        {
        if (o->LayerName() == "pushpin" && o->Type() == CartoType::MapObjectType::Point)
            pushpin = o.get();
        o->SetUserData((int)o->GeoCodeType());
        }

    // Sort objects on geocode and add information about them to the 'Address and nearby objects' field.
    if (object.size())
        {
        bool first_object = true;
        std::sort(object.begin(),object.end(),CartoType::MapObjectUserDataLessThan);
        int max_size = int(10000.0 / m_framework->MapUnitSize());
        for (auto& p : object)
            {
            // Skip objects with an bounding box wider or taller than 10 km.; avoids reporting entire nation, etc.
            if (p->CBoxBiggerThan(max_size))
                continue;
            // Skip unnamed map_extent and outline objects.
            if (!p->Label().Length())
                {
                if (p->LayerName() == "outline" || p->LayerName() == "map-extent")
                    continue;
                }
            if (first_object)
                {
                address_string.Append("\r\n\r\nNearby objects:\r\n");
                first_object = false;
                }
            address_string.Append(p->VerboseDescription().c_str());
            address_string.Append("\r\n");
            }
        }

    // Display the route dialog.
    CRouteDialog route_dialog(pushpin != nullptr);
    if (pushpin)
        route_dialog.ButtonIndex = 2;
    else if (m_route_point_array[0].Point.Point == CartoType::PointFP())
        route_dialog.ButtonIndex = 0;
    else
        route_dialog.ButtonIndex = 1;
    route_dialog.BestRoute = m_best_route;

    SetString(route_dialog.Address,address_string);

    // Convert the point clicked on to map coords.
    CartoType::RoutePoint rp;
    rp.Point.X = point.x;
    rp.Point.Y = point.y;
    m_framework->ConvertPoint(rp.Point.X,rp.Point.Y,CartoType::CoordType::Display,CartoType::CoordType::Map);

    // Set the heading to that of the nearest road.
    CartoType::NearestRoadInfo info;
    error = m_framework->FindNearestRoad(info,rp.Point.X,rp.Point.Y,CartoType::CoordType::Map,-1,false);
    static const char* compass_points[16] =
        { "n", "nne", "ne", "ene", "e", "ese", "se", "sse", "s", "ssw", "sw", "wsw", "w", "wnw", "nw", "nnw" };
    double d = 0;
    info.HeadingInDegrees += 11.25;
    if (info.HeadingInDegrees < 0)
        info.HeadingInDegrees += 360;
    else if (info.HeadingInDegrees > 360)
        info.HeadingInDegrees -= 360;
    if (!error)
        for (const auto& p : compass_points)
            {
            if (info.HeadingInDegrees >= d && info.HeadingInDegrees < d + 22.5)
                {
                SetString(route_dialog.Heading,p);
                break;
                }
            d += 22.5;
            }

    uint64_t pushpin_id = 0;
    if (pushpin)
        {
        m_pushpin_name.Set(pushpin->Label());
        m_pushpin_desc.Set(pushpin->StringAttribute("desc"));
        pushpin_id = pushpin->Id();
        }
    else
        {
        m_pushpin_name.Set(address.ToString(false));
        m_pushpin_desc.Set(address.ToString(true));
        }
    SetString(route_dialog.PushPinName,m_pushpin_name);
    SetString(route_dialog.PushPinDesc,m_pushpin_desc);

    if (route_dialog.DoModal() != IDOK)
        return;

    // Stop any route fly-through.
    StopFlyThrough();

    // Get the heading if any.
    CartoType::String h;
    SetString(h,route_dialog.Heading);
    h.SetCase(CartoType::LetterCase::Lower);
    d = 0;
    for (const auto& p : compass_points)
        {
        if (h == p)
            {
            rp.Heading = d;
            rp.HeadingKnown = true;
            break;
            }
        d += 22.5;
        }
    if (!rp.HeadingKnown && h.Length())
        {
        size_t length = 0;
        d = h.ToDouble(&length);
        if (length == h.Length())
            {
            rp.Heading = d;
            rp.HeadingKnown = true;
            }
        }

    bool create_route = false;
    m_best_route = route_dialog.BestRoute != 0;
    if (route_dialog.ButtonIndex == 0) // set start of route
        {
        m_route_point_array[0].Point = rp;
        create_route = true;
        }
    else if (route_dialog.ButtonIndex == 1) // set end of route
        {
        m_route_point_array[m_route_point_array.size() - 1].Point = rp;
        create_route = true;
        }
    else if (route_dialog.ButtonIndex == 2) // add or change a pushpin
        {
        SetString(m_pushpin_name,route_dialog.PushPinName);
        SetString(m_pushpin_desc,route_dialog.PushPinDesc);
        CartoType::String string_attrib;
        string_attrib.Set(m_pushpin_name);
        if (m_pushpin_desc.Length())
            {
            string_attrib.Append((uint16_t)0);
            string_attrib.Append("desc=");
            string_attrib.Append(m_pushpin_desc);
            }
        double x = 0,y = 0;
        if (pushpin)
            {
            CartoType::Rect r { pushpin->CBox() };
            x = r.Min.X;
            y = r.Min.Y;
            }
        else
            {
            x = rp.Point.X;
            y = rp.Point.Y;
            }
        CartoType::TextLiteral(layer,u"pushpin");
        m_framework->InsertPointMapObject(m_writable_map_handle,layer,x,y,CartoType::CoordType::Map,string_attrib,CartoType::FeatureInfo(),pushpin_id,true);

        if (route_dialog.UseAsWayPoint)
            {
            RoutePoint* waypoint = nullptr;

            for (size_t i = 1; waypoint == nullptr && i < m_route_point_array.size() - 1; i++)
                if (m_route_point_array[i].Id == pushpin_id)
                    waypoint = &m_route_point_array[i];
            if (waypoint == nullptr)
                {
                RoutePoint rp;
                m_route_point_array.insert(m_route_point_array.begin() + m_route_point_array.size() - 1,rp);
                waypoint = &m_route_point_array[m_route_point_array.size() - 2];
                }
            if (waypoint)
                {
                waypoint->Point = rp;
                waypoint->Point.Point.X = x;
                waypoint->Point.Point.Y = y;
                waypoint->Id = pushpin_id;
                create_route = true;
                }
            }  

        m_writable_map_changed = true;
        }
    else if (route_dialog.ButtonIndex == 3) // cut a pushpin
        {
        uint64_t count = 0;
        m_framework->DeleteMapObjects(m_writable_map_handle,pushpin_id,pushpin_id,count);
        for (size_t i = 1; i < m_route_point_array.size() - 1; i++)
            if (m_route_point_array[i].Id == pushpin_id)
                {
                m_route_point_array.erase(m_route_point_array.begin() + i);
                create_route = true;
                break;
                }
        m_writable_map_changed = true;
        }

    if (m_route_point_array[0].Point.Point == CartoType::PointFP() ||
        m_route_point_array[m_route_point_array.size() - 1].Point.Point == CartoType::PointFP())
        create_route = false;

    if (create_route)
        CalculateAndDisplayRoute();

    Update();
    }

void CCartoTypeDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCartoTypeDemoView diagnostics

#ifdef _DEBUG
void CCartoTypeDemoView::AssertValid() const
{
    CView::AssertValid();
}

void CCartoTypeDemoView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CCartoTypeDemoDoc* CCartoTypeDemoView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCartoTypeDemoDoc)));
    return (CCartoTypeDemoDoc*)m_pDocument;
}
#endif //_DEBUG

static void AppendDistance(CartoType::String& aDistance,double aMetres,bool aMetricUnits)
    {
    char text[64];

    if (aMetricUnits)
        {
        if (aMetres < 50)
            sprintf_s(text,"%d meters",CartoType::Round(aMetres));
        else if (aMetres < 250)
            sprintf_s(text,"%d meters",CartoType::Round(aMetres / 10) * 10);
        else if (aMetres < 100000)
            sprintf_s(text,"%.1fkm",aMetres / 1000);
        else
            sprintf_s(text,"%.fkm",aMetres / 1000);
        }
    else
        {
        double yards = aMetres * 1.093613298;
        double miles = yards / 1760;
        if (yards < 55)
            sprintf_s(text,"%d yards",CartoType::Round(yards));
        else if (yards < 220)
            sprintf_s(text,"%d yards",CartoType::Round(yards / 10) * 10);
        else if (miles < 100)
            sprintf_s(text,"%.1f miles",miles);
        else
            sprintf_s(text,"%.f miles",miles);
        }

    aDistance.Append(text);
    }

static void AppendTime(CartoType::String& aTime,double aSeconds)
    {
    char text[64];

    int hours = (int)aSeconds;
    int seconds = hours % 60;
    hours /= 60;
    int minutes = hours % 60;
    hours /= 60;

    int n = 0;
    if (hours > 1)
        n = sprintf_s(text,"%d hours",hours);
    else if (hours == 1)
        n = sprintf_s(text,"1 hour");
    if (hours > 0)
        n += sprintf_s(text + n,sizeof(text) - n," ");
    if (hours > 0 || minutes > 0)
        {
        if (minutes == 1)
            n += sprintf_s(text + n,sizeof(text) - n,"1 minute");
        else
            n += sprintf_s(text + n,sizeof(text) - n,"%d minutes",minutes);
        }
    if (hours == 0 && minutes == 0)
        {
        if (seconds == 1)
            sprintf_s(text,"1 second");
        else
            sprintf_s(text,"%d seconds",seconds);
        }
    aTime.Append(text);
    }

void CCartoTypeDemoView::PanToDraggedPosition()
    {
    auto to = m_map_drag_anchor;
    to += m_map_drag_offset;
    CartoType::Result error = m_framework->Pan(m_map_drag_anchor,to);

    if (error)
        ((CCartoTypeDemoApp*)AfxGetApp())->ShowError(_TEXT("Error panning to dragged position"),error);
    Update();
    }

void CCartoTypeDemoView::StopDragging()
    {
    m_map_drag_enabled = false;
    PanToDraggedPosition();
    m_map_drag_offset = CartoType::Point(0,0);
    TRACKMOUSEEVENT t;
    t.cbSize = sizeof(t);
    t.dwFlags = TME_LEAVE | TME_CANCEL;
    t.dwHoverTime = 0;
    t.hwndTrack = *this;
    TrackMouseEvent(&t);
    Update();
    }

void CCartoTypeDemoView::OnDestroy()
    {
    if (m_writable_map_changed &&
        AfxMessageBox(_TEXT("You have unsaved editable data (pushpins, etc). Save it now?"),MB_YESNO) == IDYES)
        OnEditSaveCtms();
    CView::OnDestroy();
    }

CartoType::Result CCartoTypeDemoView::LoadStyleSheet(const ::CString& aPath)
    {
    CartoType::String path;
    SetString(path,aPath);
    CartoType::Result error = m_framework->SetStyleSheet(path);
    if (error)
        {
        CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
        app->ShowError("error loading stylesheet",error);
        }
    else
        Update();
    CreateLegend();
    return error;
    }

/** Simulate a navigation fix after moving along the route by aDriveDistance metres, or to the next turn if aDriveDistance is zero. */
void CCartoTypeDemoView::SimulateNavigationFix(bool aGoToNextTurn,bool& aAtEnd)
    {
    if (!m_route_iter)
        {
        aAtEnd = true;
        return;
        }

    CartoType::Point p = m_route_iter->Position();
    if (aGoToNextTurn)
        {
        // Move along the route to the next turn.
        double distance = m_framework->FirstTurn().Distance;
        if (distance >= 10 || m_framework->NavigationState() == CartoType::NavigationState::Arrival)
            p = m_framework->FirstTurn().Position;
        else if (m_framework->SecondTurn().Distance > 0)
            {
            distance += m_framework->SecondTurn().Distance;
            p = m_framework->SecondTurn().Position;
            }
        else
            {
            const CartoType::RouteSegment* segment = m_framework->NextRouteSegment();
            if (segment)
                {
                auto end = segment->Path.End();
                if (end.second)
                    p = end.first;
                }
            }
        m_on_route_time += distance / 13.888;
        }

    // Simulate passing some GPS data to the navigator.
    CartoType::NavigationData nav;
    nav.Validity = CartoType::NavigationData::KPositionValid | CartoType::NavigationData::KTimeValid;
    nav.Position.X = p.X;
    nav.Position.Y = p.Y;
    nav.Time = m_on_route_time;
    m_framework->ConvertPoint(nav.Position.X,nav.Position.Y,CartoType::CoordType::Map,CartoType::CoordType::Degree);
    m_framework->Navigate(nav);

    if (!aGoToNextTurn)
        {
        // Get distance in metres.
        double distance = m_fly_through_speed_in_kph / 3.6 * m_timer_interval_in_seconds;

        // Convert to map units.
        distance /= m_framework->MapUnitSize();

        // Move along the route.
        aAtEnd = !m_route_iter->Forward(distance);
        m_on_route_time += m_timer_interval_in_seconds;

        // Adjust the speed.
        const auto& turn = m_framework->FirstTurn();
        auto route_type = turn.FromRouteInfo.Type();
        if (!turn.FromRouteInfo.Route())
            route_type = CartoType::FeatureType::ServiceRoad;
        double new_speed = m_framework->Profile(0)->Speed[size_t(route_type)];
        if (new_speed < 1)
            new_speed = 1;
        if (route_type != CartoType::FeatureType::Motorway)
            {
            if (turn.Distance < 30)
                new_speed = std::min(new_speed,20.0);
            else if (turn.Distance < 15)
                new_speed = std::min(new_speed,15.0);
            else if (turn.Distance < 5)
                new_speed = std::min(new_speed,8.0);
            }

        if (new_speed > m_fly_through_speed_in_kph)
            m_fly_through_speed_in_kph = std::min(new_speed,m_fly_through_speed_in_kph + 5 * m_timer_interval_in_seconds);
        else if (new_speed < m_fly_through_speed_in_kph)
            m_fly_through_speed_in_kph = std::max(new_speed,m_fly_through_speed_in_kph - 20 * m_timer_interval_in_seconds);

        char buffer[16];
        sprintf_s(buffer,"%.2f",m_fly_through_speed_in_kph);
        m_framework->SetCopyrightNotice(buffer);
        }
    }

// CCartoTypeDemoView message handlers

void CCartoTypeDemoView::OnInitialUpdate()
    {
    __super::OnInitialUpdate();

    EnableScrollBarCtrl(SB_BOTH,false);
    CartoType::Result error;

    CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
    ASSERT_VALID(app);
    CCartoTypeDemoDoc* doc = GetDocument();
    ASSERT_VALID(doc);


    CartoType::Framework::Param param;
    param.SharedEngine = app->Engine();
    param.SharedMapDataSet = doc->MapDataSet();
    param.StyleSheetFileName = app->DefaultStyleSheetName();
    RECT rect;
    GetClientRect(&rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    param.ViewHeight = h;
    param.ViewWidth = w;
    param.TextIndexLevels = 0;
    
    m_framework = CartoType::Framework::New(error,param);
    if (!error)
        {
        error = m_framework->CreateWritableMap(CartoType::WritableMapType::Memory);
        m_writable_map_handle = m_framework->LastMapHandle();
        }
    if (error)
        {
        app->ShowError(_TEXT("error creating CartoType framework"),error);
        return;
        }
    OutputMapExtent();

    // Start loading navigation data asynchronously.
    m_framework->LoadNavigationData();

    m_framework->SetFollowMode(CartoType::FollowMode::None);
    m_framework->SetLocale(m_metric_units ? "en_xx" : "en");

    CreateLegend();
    m_framework->EnableLegend(m_draw_legend);
    m_framework->EnableTurnInstructions(m_draw_turn_instructions);
    m_framework->SetScaleBar(m_metric_units,2.5,"in",CartoType::NoticePosition::BottomLeft);
    m_framework->SetTurnInstructions(m_metric_units,true,2,"in",CartoType::NoticePosition::TopLeft);
    m_framework->SetVehiclePosOffset(0,0.25);
    m_framework->SetAnimateTransitions(true);

    // COMMENT OUT THE NEXT LINE TO DISABLE GRAPHICS ACCELERATION
    m_map_renderer = std::make_unique<CartoType::MapRenderer>(*m_framework,m_hWnd);
 
    // Add a call to iFramework->SetResolutionDpi to set the pixel size. The default resolution is 144dpi, which works well on Windows.

    TestCode();
    }

static std::unique_ptr<CartoType::Route> CreateSnappedRoute(CartoType::Result& aError,CartoType::Framework& aFramework,
                                                             const CartoType::RouteProfile& aFindRoadProfile,const CartoType::RouteProfile& aProfile,
                                                             CartoType::PointFP aStart,CartoType::PointFP aEnd)
    {
    CartoType::LocationMatchParam param;
    param.MaxRoadDistanceInMeters = 500;
    aFramework.SetLocationMatchParam(param);
    aFramework.SetMainProfile(aFindRoadProfile);
    CartoType::NearestRoadInfo info;
    aError = aFramework.FindNearestRoad(info,aStart.X,aStart.Y,CartoType::CoordType::Degree,-1,false);
    if (aError)
        return nullptr;
    aStart = info.NearestPoint;
    aError = aFramework.FindNearestRoad(info,aEnd.X,aEnd.Y,CartoType::CoordType::Degree,-1,false);
    if (aError)
        return nullptr;
    aEnd = info.NearestPoint;
    CartoType::CoordSetOfTwoPoints cs(aStart.X,aStart.Y,aEnd.X,aEnd.Y);
    param.MaxRoadDistanceInMeters = 15;
    aFramework.SetLocationMatchParam(param);
    return aFramework.CreateRoute(aError,aProfile,cs,CartoType::CoordType::Map);
    }

void CCartoTypeDemoView::TestCode()
    {
    }

void CCartoTypeDemoView::OnSize(UINT nType,int cx,int cy)
    {
    __super::OnSize(nType,cx,cy);
    if (m_framework)
        {
        m_framework->Resize(cx,cy);
        Update();
        }
    }

void CCartoTypeDemoView::OnAppAbout()
    {
    CAboutDlg about_dialog;
    
    SetString(about_dialog.iTextLine1,CartoType::String("CartoType Windows demo"));
    CartoType::String s = "built using CartoType ";
    s += CartoType::Version();
    s += ".";
    s += CartoType::Build();
    SetString(about_dialog.iTextLine2,s);

    auto m = m_framework->MapMetaData(0);
    char buffer[256];
    s = "map ";
    if (m->CartoTypeVersion.Major)
        {
        sprintf_s(buffer,"%d.%d.%d",m->CartoTypeVersion.Major,m->CartoTypeVersion.Minor,m->CartoTypeBuild);
        s += m->DataSetName + " created by CartoType " + buffer;
        }
    else
        s += m->DataSetName;
    if (m->FileVersion.Major)
        {
        sprintf_s(buffer,"%d.%d",m->FileVersion.Major,m->FileVersion.Minor);
        s += "\nCTM1 version ";
        s += buffer;
        }
    s += "\nprojection: " + m->ProjectionName;
    sprintf_s(buffer,"\nextent in degrees: %g, %g, %g, %g.\nroute table: ",m->ExtentInDegrees.MinX(),m->ExtentInDegrees.MinY(),m->ExtentInDegrees.MaxX(),m->ExtentInDegrees.MaxY());
    s += buffer;
    const char* r = "none";
    switch (m->RouteTableType)
        {
        case CartoType::MapTableType::KRouteTableAStar: r = "a-star (/route=a)"; break;
        case CartoType::MapTableType::KRouteTableCH: r = "contraction hierarchy (/route=c)"; break;
        case CartoType::MapTableType::KRouteTableTurnExpanded: r = "turn-expanded (/route=t)"; break;
        case CartoType::MapTableType::KRouteTableCHStandAlone: r = "contraction hierarchy stand-alone (/route=cs)"; break;
        case CartoType::MapTableType::KRouteTableTECH: r = "turn-expanded contraction hierarchy (/route=tech)"; break;
        case CartoType::MapTableType::KRouteTableCHTiled: r = "contraction hierarchy, tiled (/route=ct)"; break;
        case CartoType::MapTableType::KRouteTableTECHTiled: r = "turn-expanded contraction hierarchy, tiled (/route=tt)"; break;
        case CartoType::MapTableType::KRouteTableTurnExpandedCompact: r = "turn-expanded, compact (/route=tc)"; break;
        default: break;
        }
    s += r;
    s += ".";
    SetString(about_dialog.iTextLine3,s);

    about_dialog.DoModal();
    }

BOOL CCartoTypeDemoView::OnEraseBkgnd(CDC* /*pDC*/)
    {
    // Suppress erasure of the background.
    return TRUE;
    }

bool CCartoTypeDemoView::SetCentre(CPoint& aPoint)
    {
    GetCursorPos(&aPoint);
    ScreenToClient(&aPoint);
    CRect r;
    GetClientRect(&r);
    bool set_centre = false;
    if (aPoint.x >= r.left && aPoint.x < r.right && aPoint.y >= r.top && aPoint.y <= r.bottom)
        {
        aPoint.x -= r.left;
        aPoint.y -= r.top;
        set_centre = true;
        }
    return set_centre;
    }

BOOL CCartoTypeDemoView::OnMouseWheel(UINT nFlags,short zDelta,CPoint point)
    {
    if (zDelta == 0)
        return TRUE;

    CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();

    // If the mouse pointer is in the window, centre the zoom at that point.
    bool set_centre = SetCentre(point);

    /*
    If using the image server, always zoom by a factor of 2 because
    the image server system snaps to zoom levels arranged in factors
    of 2, and zooming less will often have no effect because it
    will snap back to the previous level.
    */
    double zoom = sqrt(app->ZoomFactor());

    int zoom_count = zDelta / WHEEL_DELTA;
    if (zoom_count == 0)
        zoom_count = zDelta >= 0 ? 1 : -1;

    zoom = pow(zoom,zoom_count);

    if (set_centre)
        m_framework->ZoomAt(zoom,point.x,point.y,CartoType::CoordType::Display);
    else
        m_framework->Zoom(zoom);

    Update();
    return TRUE;
    }


void CCartoTypeDemoView::OnMouseMove(UINT /*nFlags*/,CPoint point)
    {
    if (m_map_drag_enabled)
        {
        m_map_drag_offset.X = point.x - m_map_drag_anchor.X;
        m_map_drag_offset.Y = point.y - m_map_drag_anchor.Y;

        if (m_map_renderer)
            {
            m_framework->Pan(m_map_drag_anchor_in_map_coords.X,m_map_drag_anchor_in_map_coords.Y,CartoType::CoordType::Map,
                            point.x,point.y,CartoType::CoordType::Display);
            m_map_drag_offset = CartoType::Point(0,0);
            m_map_drag_anchor.X = point.x;
            m_map_drag_anchor.Y = point.y;
            m_map_drag_anchor_in_map_coords = m_map_drag_anchor;
            m_framework->ConvertPoint(m_map_drag_anchor_in_map_coords.X,m_map_drag_anchor_in_map_coords.Y,CartoType::CoordType::Display,CartoType::CoordType::Map);
            }

        if (!m_map_renderer)
            {
            CDC* dc = GetDC();
            if (dc)
                {
                OnDraw(dc);
                ReleaseDC(dc);
                }
            }
        }
    }

void CCartoTypeDemoView::OnMouseLeave()
    {
    StopDragging();
    }

void CCartoTypeDemoView::OnLButtonDown(UINT nFlags,CPoint point)
    {
    m_map_drag_enabled = true;
    m_map_drag_anchor.X = point.x;
    m_map_drag_anchor.Y = point.y;
    m_map_drag_anchor_in_map_coords = m_map_drag_anchor;
    m_framework->ConvertPoint(m_map_drag_anchor_in_map_coords.X,m_map_drag_anchor_in_map_coords.Y,CartoType::CoordType::Display,CartoType::CoordType::Map);
    TRACKMOUSEEVENT t;
    t.cbSize = sizeof(t);
    t.dwFlags = TME_LEAVE;
    t.dwHoverTime = 0;
    t.hwndTrack = *this;
    TrackMouseEvent(&t);
    }

void CCartoTypeDemoView::OnLButtonUp(UINT nFlags,CPoint point)
    {
    m_map_drag_offset.X = point.x - m_map_drag_anchor.X;
    m_map_drag_offset.Y = point.y - m_map_drag_anchor.Y;

    // Supply a simulated navigation fix if the left mouse button is pressed with the shift key down.	
    if (m_shift_left_click_simulates_navigation_fix)
        {
        if (m_map_drag_offset.X == 0 && m_map_drag_offset.Y == 0 && (nFlags & MK_SHIFT))
            {
            CartoType::NavigationData nav;
            nav.Validity = CartoType::NavigationData::KPositionValid | CartoType::NavigationData::KTimeValid;
            nav.Position.X = point.x;
            nav.Position.Y = point.y;
            m_on_route_time += 20;
            nav.Time = m_on_route_time;
            m_framework->ConvertPoint(nav.Position.X,nav.Position.Y,CartoType::CoordType::Display,CartoType::CoordType::Degree);
            m_framework->Navigate(nav);

            CartoType::String state = "navigation state: ";
            switch (m_framework->NavigationState())
                {
                case CartoType::NavigationState::None: state += "None (no route has been created, or navigation is disabled)"; break;
                case CartoType::NavigationState::NoPosition: state += "NoPosition (there is a route, and navigation is enabled, but no position has been supplied)"; break;
                case CartoType::NavigationState::Turn: state += "Turn (the position is on the route and turn information is available)"; break;
                case CartoType::NavigationState::OffRoute: state += "OffRoute (the position is off the route)"; break;
                case CartoType::NavigationState::ReRouteNeeded: state += "ReRouteNeeded (a new route needs to be calculated)"; break;
                case CartoType::NavigationState::ReRouteDone: state += "ReRouteDone (a new route has been calculated after a period off route)"; break;
                case CartoType::NavigationState::TurnRound: state += "TurnRound (the position is on the route but a U-turn is needed)"; break;
                case CartoType::NavigationState::Arrival: state += "Arrival (the position is on the route and very close to the destination)"; break;
                }

            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            CMainFrame* main_window = (CMainFrame*)app->m_pMainWnd;
            state.Append((uint16_t)0);
            main_window->SetMessageText((LPCTSTR)state.Data());
            }
        }

    StopDragging();
    }

void CCartoTypeDemoView::OnFileOpenInCurrentMap()
    {
    ::CString filename;
    CartoType::Result error = CartoType::KErrorGeneral;
    if (AfxGetApp()->DoPromptFileName(filename,AFX_IDS_OPENFILE,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,true,nullptr))
        {
        CartoType::String f;
        SetString(f,filename);
        bool used_built_in_profiles = m_framework->BuiltInProfileCount() > 0;
        if (f.Last(10) == ".ctm1_ctci")
            {
            std::string key("password");
            error = m_framework->LoadMap(f,&key);
            }
        else
            error = m_framework->LoadMap(f);
        if (!error)
            {
            m_map_handle.push_back(m_framework->LastMapHandle());
            if (used_built_in_profiles || m_framework->BuiltInProfileCount() > 0)
                SetProfile(0);
            }
        }
    if (error)
        {
        CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
        app->ShowError(_TEXT("failed to load map"),error);
        }

    OutputMapExtent();

    if (m_map_renderer)
        {
        m_map_renderer = nullptr;
        m_map_renderer = std::make_unique<CartoType::MapRenderer>(*m_framework,m_hWnd);
        }

    if (error == CartoType::KErrorNone)
        Update();
    }

void CCartoTypeDemoView::OnFileClose()
    {
    size_t index = m_map_handle.size();
    if (index)
        {
        index--;
        m_framework->UnloadMapByHandle(m_map_handle[index]);
        m_map_handle.erase(m_map_handle.begin() + index);
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateFileClose(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_map_handle.size() > 0);
    }

void CCartoTypeDemoView::OnFileSaveAsPng()
    {
    CFileDialog file_dialog(false,_TEXT(".png"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("PNG files (*.png)|*.png||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error;
        try
            {
            CartoType::FileOutputStream output_stream(path);
            const CartoType::BitmapView* bitmap = m_framework->MapBitmap(error);
            if (!error)
                error = bitmap->WritePng(output_stream,false);
            }
        catch (CartoType::Result e)
            {
            error = e;
            }
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to save as PNG"),error);
            }
        }
    }

void CCartoTypeDemoView::OnFileSaveAsCtsql()
    {
    CFileDialog file_dialog(false,_TEXT(".ctsql"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("CartoType SQLite database files (*.ctsql)|*.ctsql||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->SaveMap(m_framework->MainMapHandle(),path,CartoType::FileType::CTSQL);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to save data to a CTSQL file"),error);
            }
        }
    }

void CCartoTypeDemoView::ShowNextFoundObject()
    {
    const auto& object = m_found_object[m_found_object_index];

    CartoType::String summary;
    m_framework->GetGeoCodeSummary(summary,*object);
    CartoType::Result error;
    double length = object->LengthOrPerimeter(error);
    double area = 0;
    if (!error)
        area = object->Area(error);
    if (!error)
        {
        char buffer[64];
        if (area != 0)
            {
            if (m_metric_units)
                {
                if (area > 1000000)
                    sprintf_s(buffer,"; area = %.2f sq km",area / 1000000);
                else
                    sprintf_s(buffer,"; area = %.2f ha",area / 10000);
                }
            else
                {
                // Convert to acres.
                double acres = area / 4046.856422;
                if (acres > 640)
                    sprintf_s(buffer,"; area = %.2f sq mi",acres / 640);
                else
                    sprintf_s(buffer,"; area = %.2f acres",acres);
                }
            summary.Append(buffer);
            }
        else if (length != 0)
            {
            if (m_metric_units)
                {
                if (length > 1000)
                    sprintf_s(buffer,"; length = %.2f km",length / 1000);
                else
                    sprintf_s(buffer,"; length = %.2f m",length);
                }
            else
                {
                // Convert to yards.
                double yards = length / 0.9144;
                if (yards > 1760)
                    sprintf_s(buffer,"; length = %.2f mi",yards / 1760);
                else
                    sprintf_s(buffer,"; length = %.2f yards",yards);
                }
            summary.Append(buffer);
            }
        }

    CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
    CMainFrame* main_window = (CMainFrame*)app->m_pMainWnd;
    summary.Append((uint16_t)0);
    main_window->SetMessageText((LPCTSTR)summary.Data());

    CartoType::TextLiteral(found_layer,u"found");
    if (object->Type() == CartoType::MapObjectType::Line)
        {
        auto envelope = object->Envelope(5.0 / m_framework->MapUnitSize());
        error = m_framework->InsertMapObject(0,found_layer,CartoType::Geometry(envelope,CartoType::CoordType::Map,true),"",CartoType::FeatureInfo(),m_found_object_id,true);
        }
    else
        {
        error = m_framework->InsertCopyOfMapObject(0,found_layer,*object,object->Type() == CartoType::MapObjectType::Point ? 50 : 0,
                                                  CartoType::CoordType::MapMeter,m_found_object_id,true);
        }

    if (!error)
        {
        bool animate = m_framework->SetAnimateTransitions(true);
        m_framework->SetView(*object,64,4000);
        m_framework->SetAnimateTransitions(animate);
        }
    if (error)
        app->ShowError(_TEXT("Error displaying found object"),error);

    m_found_object_index = (m_found_object_index + 1) % m_found_object.size();
    Update();
    }

void CCartoTypeDemoView::OnFind()
    {
    m_find_text_dialog.Set(m_framework.get());

    bool stop = false;
    m_found_object_index = 0;
    while (!stop)
        {
        if (m_find_text_dialog.DoModal() == IDOK)
            {
            m_found_object = m_find_text_dialog.FoundObjectArray();

            if (m_found_object.size() == 0)
                {
                ::CString msg("Text ");
                msg += m_find_text_dialog.FindText;
                msg += " not found";
                AfxMessageBox(msg);
                }
            else
                {
                uint64_t count = 0;
                m_framework->DeleteMapObjects(0,m_found_object_id,m_found_object_id,count);
                m_found_object_id = 0;
                ShowNextFoundObject();

                // Insert pushpins for first 10 found objects.
                m_framework->DeleteMapObjects(0,0,UINT64_MAX,count,"@layer=='pushpin' and @sub_type==3");
                int n = 0;
                for (const auto& p : m_found_object)
                    {
                    auto centre = p->CenterOfGravity();
                    uint64_t id = 0;
                    m_framework->InsertPointMapObject(0,"pushpin",centre.X,centre.Y,CartoType::CoordType::Map,"",CartoType::FeatureInfo(3),id,false);
                    if (++n == 10)
                        break;
                    }

                stop = true;
                }
            }
        else
            stop = true;
        }
    }

void CCartoTypeDemoView::OnUpdateFind(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnFindAddress()
    {
    CFindAddressDialog find_dialog;
    SetString(find_dialog.Building,m_address.Building);
    SetString(find_dialog.Feature,m_address.Feature);
    SetString(find_dialog.Street,m_address.Street);
    SetString(find_dialog.SubLocality,m_address.SubLocality);
    SetString(find_dialog.Locality,m_address.Locality);
    SetString(find_dialog.SubAdminArea,m_address.SubAdminArea);
    SetString(find_dialog.AdminArea,m_address.AdminArea);
    SetString(find_dialog.Country,m_address.Country);
    SetString(find_dialog.PostCode,m_address.PostCode);

    bool stop = false;

    m_found_object_index = 0;
    while (!stop)
        {
        if (find_dialog.DoModal() == IDOK)
            {
            CWaitCursor cursor;

            m_found_object.clear();
            CartoType::Result error;
            SetString(m_address.Building,find_dialog.Building);
            SetString(m_address.Feature,find_dialog.Feature);
            SetString(m_address.Street,find_dialog.Street);
            SetString(m_address.SubLocality,find_dialog.SubLocality);
            SetString(m_address.Locality,find_dialog.Locality);
            SetString(m_address.SubAdminArea,find_dialog.SubAdminArea);
            SetString(m_address.AdminArea,find_dialog.AdminArea);
            SetString(m_address.Country,find_dialog.Country);
            SetString(m_address.PostCode,find_dialog.PostCode);
            error = m_framework->FindAddress(m_found_object,100,m_address,true);

            if (m_found_object.size() == 0)
                {
                ::CString msg("address not found");
                AfxMessageBox(msg);
                }
            else
                {
                ShowNextFoundObject();
                stop = true;
                }
            }
        else
            stop = true;
        }
    }

void CCartoTypeDemoView::OnUpdateFindAddress(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnFindNext()
    {
    ShowNextFoundObject();
    }

void CCartoTypeDemoView::OnUpdateFindNext(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_found_object.size() > 1);
    }

void CCartoTypeDemoView::OnViewReloadStyleSheet()
    {
    m_framework->ReloadStyleSheet();
    CreateLegend();
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewReloadStyleSheet(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewZoomIn()
    {
    CPoint point;
    bool set_centre = SetCentre(point);

    CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
    if (set_centre)
        m_framework->ZoomAt(app->ZoomFactor(),point.x,point.y,CartoType::CoordType::Display);
    else
        m_framework->Zoom(app->ZoomFactor());
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewZoomIn(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewZoomOut()
    {
    CPoint point;
    bool set_centre = SetCentre(point);

    CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
    if (set_centre)
        m_framework->ZoomAt(1.0 / app->ZoomFactor(),point.x,point.y,CartoType::CoordType::Display);
    else
        m_framework->Zoom(1.0 / app->ZoomFactor());
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewZoomOut(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewRotate()
    {
    CPoint point;
    if (SetCentre(point))
        m_framework->RotateAndZoom(15,1,point.x,point.y,CartoType::CoordType::Display);
    else
        m_framework->Rotate(15);
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewRotate(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewNorthup()
    {
    m_framework->SetRotation(0);
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewNorthup(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Rotation() != 0);
    }

void CCartoTypeDemoView::OnViewLatlong()
    {
    CLatLongDialog lat_long_dialog;

    double minx,miny,maxx,maxy;
    m_framework->GetView(minx,miny,maxx,maxy,CartoType::CoordType::Degree);
    double cur_long = (minx + maxx) / 2;
    double cur_lat = (miny + maxy) / 2;
    m_framework->GetMapExtent(minx,miny,maxx,maxy,CartoType::CoordType::Degree);

    char buffer[32];
    sprintf_s(buffer,"%.4f",cur_long);
    SetString(lat_long_dialog.Longitude,buffer);
    sprintf_s(buffer,"%.4f",cur_lat);
    SetString(lat_long_dialog.Latitude,buffer);

    if (lat_long_dialog.DoModal() == IDOK)
        {
        CartoType::TextBuffer<32> lo;
        SetString(lo,lat_long_dialog.Longitude);
        CartoType::TextBuffer<32> la;
        SetString(la,lat_long_dialog.Latitude);
        double x = lo.ToDouble();
        double y = la.ToDouble();

        if (x < minx)
            x = minx;
        else if (x > maxx)
            x = maxx;

        if (y < miny)
            y = miny;
        else if (y > maxy)
            y = maxy;

        CartoType::Result error = m_framework->SetViewCenter(x,y,CartoType::CoordType::Degree);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("Set center error"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateViewLatlong(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewStylesheet()
    {
    CFileDialog file_dialog(true,_TEXT(".ctstyle"),nullptr,0,TEXT("style sheets (*.ctstyle)|*.ctstyle||"));
    if (file_dialog.DoModal() == IDOK)
        LoadStyleSheet(file_dialog.GetPathName());
    }

void CCartoTypeDemoView::OnUpdateViewStylesheet(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewLayers()
    {
    CLayerDialog dialog(*m_framework);
    if (dialog.DoModal() == IDOK)
        Update();
    }

void CCartoTypeDemoView::OnUpdateViewLayers(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewLegend()
    {
    m_draw_legend = !m_draw_legend;
    m_framework->EnableLegend(m_draw_legend);
    m_framework->ForceRedraw();
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewLegend(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    pCmdUI->SetCheck(m_draw_legend);
    }

void CCartoTypeDemoView::OnViewSetScale()
    {
    CSetScaleDialog dialog;
    dialog.Scale.Format(_TEXT("%d"),(int)m_framework->ScaleDenominator());
    if (dialog.DoModal() == IDOK)
        {
        CartoType::TextBuffer<32> scale_text;
        SetString(scale_text,dialog.Scale);
        m_framework->SetScaleDenominatorInView(scale_text.ToInt32());
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateViewSetScale(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnViewPerspective()
    {
    m_framework->SetPerspective(!m_framework->Perspective());
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewPerspective(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    pCmdUI->SetCheck(m_framework->Perspective());
    }

void CCartoTypeDemoView::OnViewMetricUnits()
    {
    m_metric_units = !m_metric_units;
    m_framework->SetLocale(m_metric_units ? "en_xx" : "en");
    CreateLegend();
    m_framework->SetScaleBar(m_metric_units,2.5,"in",CartoType::NoticePosition::BottomLeft);
    m_framework->SetTurnInstructions(m_metric_units,true,2,"in",CartoType::NoticePosition::TopLeft);
    if (m_draw_legend || m_draw_turn_instructions)
        {
        m_framework->ForceRedraw();
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateViewMetricUnits(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    pCmdUI->SetCheck(m_metric_units);
    }

void CCartoTypeDemoView::OnViewTurnInstructions()
    {
    m_draw_turn_instructions = !m_draw_turn_instructions;
    m_framework->EnableTurnInstructions(m_draw_turn_instructions);
    m_framework->ForceRedraw();
    Update();
    }

void CCartoTypeDemoView::OnUpdateViewTurnInstructions(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Route() != nullptr);
    pCmdUI->SetCheck(m_draw_turn_instructions);
    }

void CCartoTypeDemoView::StopFlyThrough()
    {
    if (m_fly_through_timer)
        {
        KillTimer(m_fly_through_timer);
        m_fly_through_timer = 0;
        m_route_iter = nullptr;
        m_framework->SetFollowMode(CartoType::FollowMode::None);
        m_framework->SetPerspective(false);
        m_framework->SetRotation(0);
        Update();
        }
    }

void CCartoTypeDemoView::OnViewFlyThrough()
    {
    // If doing a fly-through, cancel it.
    if (m_fly_through_timer)
        {
        StopFlyThrough();
        return;
        }

    m_route_iter.reset(new CartoType::RouteIterator(*m_framework->Route()));
    if (m_route_iter)
        {
        m_framework->SetFollowMode(CartoType::FollowMode::LocationHeading);
        m_fly_through_timer = (UINT)SetTimer(1,UINT(m_timer_interval_in_seconds * 1000),0);
        }
    }

void CCartoTypeDemoView::OnUpdateViewFlyThrough(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Navigating());
    pCmdUI->SetCheck(m_fly_through_timer != 0);
    }

void CCartoTypeDemoView::OnGoAlongRoute()
    {
    bool at_end = false;
    SimulateNavigationFix(true,at_end);

    // Temporarily turn the follow mode on so that the view jumps to the position.
    m_framework->SetFollowMode(CartoType::FollowMode::Location);
    m_framework->SetFollowMode(CartoType::FollowMode::None);

    // Uncomment to test position extrapolation.
    // CartoType::NavigationData n;
    // n.iValidity = CartoType::NavigationData::ESpeedValid;
    // n.iSpeed = 100;
    // iFramework->Navigate(n);

    Update();
    }

void CCartoTypeDemoView::OnUpdateGoAlongRoute(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Navigating());
    }

void CCartoTypeDemoView::OnReverseRoute()
    {
    std::reverse(m_route_point_array.begin(),m_route_point_array.end());
    if (m_route_point_array[0].Point.Point != CartoType::PointFP() &&
        m_route_point_array[m_route_point_array.size() - 1].Point.Point != CartoType::PointFP())
        CalculateAndDisplayRoute();
    }

void CCartoTypeDemoView::OnUpdateReverseRoute(CCmdUI* pCmdUI)
    {
    pCmdUI->Enable(m_framework->Route() != nullptr);
    }

void CCartoTypeDemoView::OnDeleteRoute()
    {
    StopFlyThrough();
    m_route_point_array.resize(2);
    m_route_point_array[0] = m_route_point_array[1] = RoutePoint();
    m_framework->DeleteRoutes();
    Update();
    }

void CCartoTypeDemoView::OnUpdateDeleteRoute(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Route() != nullptr);
    }

void CCartoTypeDemoView::SetProfile(size_t aProfileIndex)
    {
    StopFlyThrough();

    size_t b = m_framework->BuiltInProfileCount();
    if (b == 0)
        {
        m_route_profile_index = aProfileIndex;
        auto type = CartoType::RouteProfileType::Car;
        switch (aProfileIndex)
            {
            case 1: type = CartoType::RouteProfileType::Cycle; break;
            case 2: type = CartoType::RouteProfileType::Walk; break;
            case 3: type = CartoType::RouteProfileType::Hike; break;
            case 4: type = CartoType::RouteProfileType::Ski; break;
            default: break;
            }
        CartoType::RouteProfile p = type;
        p.Shortest = m_shortest_route;
        if (!m_use_gradients)
            p.GradientFlags = 0;
        m_framework->SetMainProfile(p);
        }
    else
        {
        m_framework->SetBuiltInProfile(aProfileIndex);
        if (aProfileIndex < b)
            {
            m_route_profile_index = aProfileIndex;
            m_shortest_route = m_framework->BuiltInProfile(aProfileIndex)->Shortest;
            m_use_gradients = m_framework->BuiltInProfile(aProfileIndex)->GradientFlags != 0;
            }
        }
    if (m_route_point_array[0].Point.Point != CartoType::PointFP() &&
        m_route_point_array[m_route_point_array.size() - 1].Point.Point != CartoType::PointFP())
        CalculateAndDisplayRoute();
    }

void CCartoTypeDemoView::OutputMapExtent()
    {
    double min_long,min_lat,max_long,max_lat;
    auto error = m_framework->GetMapExtent(min_long,min_lat,max_long,max_lat,CartoType::CoordType::Degree);
    if (!error)
        {
        char buffer[256];
        sprintf_s(buffer,"map extent: %g%s %g%s ... %g%s %g%s\n",
                  min_long < 0 ? -min_long : min_long,min_long < 0 ? "W" : "E",
                  min_lat < 0 ? -min_lat : min_lat,min_lat < 0 ? "S" : "N",
                  max_long < 0 ? -max_long : max_long,max_long < 0 ? "W" : "E",
                  max_lat < 0 ? -max_lat : max_lat,max_lat < 0 ? "S" : "N");
        OutputDebugStringA(buffer);
        }
    }

// Set the drive profile, or profile 0 if using built-in profiles.
void CCartoTypeDemoView::OnSetDriveProfile()
    {
    SetProfile(0);
    }

void CCartoTypeDemoView::SetMenuItemToBuiltInProfileName(CCmdUI* aMenuItem,size_t aProfileIndex)
    {
    if (m_framework->BuiltInProfileCount() > aProfileIndex)
        {
        aMenuItem->Enable(true);
        CartoType::String s = m_framework->BuiltInProfile(aProfileIndex)->Name;
        s.SetCase(CartoType::LetterCase::Title);
        s += " (built-in)";
        s.Append(uint16_t(0));
        aMenuItem->SetText((LPCTSTR)s.Data());
        }
    else
        {
        aMenuItem->Enable(false);
        aMenuItem->SetText(_TEXT("(none)"));
        }
    }

void CCartoTypeDemoView::OnUpdateSetDriveProfile(CCmdUI *pCmdUI)
    {
    if (m_framework->BuiltInProfileCount())
        SetMenuItemToBuiltInProfileName(pCmdUI,0);
    else
        pCmdUI->SetText(_TEXT("Drive"));
    pCmdUI->SetCheck(m_route_profile_index == 0);
    }

// Set the cycle profile, or profile 1 if using built-in profiles.
void CCartoTypeDemoView::OnSetCycleProfile()
    {
    SetProfile(1);
    }

void CCartoTypeDemoView::OnUpdateSetCycleProfile(CCmdUI *pCmdUI)
    {
    if (m_framework->BuiltInProfileCount())
        SetMenuItemToBuiltInProfileName(pCmdUI,1);
    else
        pCmdUI->SetText(_TEXT("Cycle"));
    pCmdUI->SetCheck(m_route_profile_index == 1);
    }

// Set the walk profile, or profile 2 if using built-in profiles.
void CCartoTypeDemoView::OnSetWalkProfile()
    {
    SetProfile(2);
    }

void CCartoTypeDemoView::OnUpdateSetWalkProfile(CCmdUI *pCmdUI)
    {
    if (m_framework->BuiltInProfileCount())
        SetMenuItemToBuiltInProfileName(pCmdUI,2);
    else
        pCmdUI->SetText(_TEXT("Walk"));
    pCmdUI->SetCheck(m_route_profile_index == 2);
    }

// Set the hike profile, or profile 3 if using built-in profiles.
void CCartoTypeDemoView::OnSetHikeProfile()
    {
    SetProfile(3);
    }

void CCartoTypeDemoView::OnUpdateSetHikeProfile(CCmdUI* pCmdUI)
    {
    if (m_framework->BuiltInProfileCount())
        SetMenuItemToBuiltInProfileName(pCmdUI,3);
    else
        pCmdUI->SetText(_TEXT("Hike"));
    pCmdUI->SetCheck(m_route_profile_index == 3);
    }

// Set the ski profile, or profile 4 if using built-in profiles.
void CCartoTypeDemoView::OnSetSkiProfile()
    {
    SetProfile(4);
    }

void CCartoTypeDemoView::OnUpdateSetSkiProfile(CCmdUI* pCmdUI)
    {
    if (m_framework->BuiltInProfileCount())
        SetMenuItemToBuiltInProfileName(pCmdUI,3);
    else
        pCmdUI->SetText(_TEXT("Ski"));
    pCmdUI->SetCheck(m_route_profile_index == 4);
    }

void CCartoTypeDemoView::OnShortestRoute()
    {
    m_shortest_route = !m_shortest_route;
    SetProfile(m_route_profile_index);
    }

void CCartoTypeDemoView::OnUpdateShortestRoute(CCmdUI* pCmdUI)
    {
    pCmdUI->Enable(m_framework->BuiltInProfileCount() == 0);
    pCmdUI->SetCheck(m_shortest_route);
    }

void CCartoTypeDemoView::OnUseGradients()
    {
    m_use_gradients = !m_use_gradients;
    SetProfile(m_route_profile_index);
    }

void CCartoTypeDemoView::OnUpdateUseGradients(CCmdUI* pCmdUI)
    {
    pCmdUI->Enable(m_framework->BuiltInProfileCount() == 0 && m_framework->NavigationDataHasGradients());
    pCmdUI->SetCheck(m_use_gradients);
    }

void CCartoTypeDemoView::OnSaveRouteInstructions()
    {
    CFileDialog file_dialog(false,_TEXT(".txt"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("Text files (*.txt)|*.txt||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        std::string path_utf8(path);
        CartoType::Result error;
        FILE* file = nullptr;
        fopen_s(&file,path_utf8.c_str(),"w");
        if (!file)
            error = CartoType::KErrorIo;
        if (!error)
            {
            std::string text(m_framework->RouteInstructions(*m_framework->Route()));
            size_t n = fwrite(text.c_str(),text.length(),1,file);
            if (!n)
                error = CartoType::KErrorIo;
            }
        fclose(file);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to save the route instructions to a text file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateSaveRouteInstructions(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Route() != nullptr);
    }

void CCartoTypeDemoView::OnSaveRouteXml()
    {
    CFileDialog file_dialog(false,_TEXT(".ctroute"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("CartoType Route files (*.ctroute)|*.ctroute||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->WriteRouteAsXml(*m_framework->Route(),path);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to save the route to a .ctroute file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateSaveRouteXml(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Route() != nullptr);
    }

void CCartoTypeDemoView::OnSaveRouteGpx()
    {
    CFileDialog file_dialog(false,_TEXT(".gpx"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("GPX files (*.gpx)|*.gpx||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->WriteRouteAsXml(*m_framework->Route(),path,CartoType::FileType::GPX);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to save the route to a GPX file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateSaveRouteGpx(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(m_framework->Route() != nullptr);
    }

void CCartoTypeDemoView::OnLoadRouteXml()
    {
    CFileDialog file_dialog(true,_TEXT(".ctroute"),nullptr,0,TEXT("CartoType Route files (*.ctroute)|*.ctroute|GPX files (*.gpx)|*.gpx||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->ReadRouteFromXml(path,true);
        if (m_framework->Route())
            {
            m_route_iter = std::make_unique<CartoType::RouteIterator>(*m_framework->Route());
            bool at_end = false;
            SimulateNavigationFix(false,at_end);
            }
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to read a route from a file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateLoadRouteXml(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnTimer(UINT_PTR aEventId)
    {
    if (m_manual_draw_timer)
        m_map_renderer->Draw();

    if (m_fly_through_timer && m_route_iter)
        {
        bool at_end = false;
        SimulateNavigationFix(false,at_end);

        // Stop the fly-through if we have reached the end.
        if (at_end)
            StopFlyThrough();

        Update();
        }
    }

void CCartoTypeDemoView::OnEditLoadKml()
    {
    CFileDialog file_dialog(true,_TEXT(".kml"),nullptr,0,TEXT("KML files (*.kml)|*.kml||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->ReadMap(m_writable_map_handle,path,CartoType::FileType::KML);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to read data from a KML file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateEditLoadKml(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnEditSaveKml()
    {
    CFileDialog file_dialog(false,_TEXT(".kml"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("KML files (*.kml)|*.kml||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->SaveMap(m_writable_map_handle,path,CartoType::FileType::KML);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to write data to a KML file"),error);
            }
        else
            m_writable_map_changed = false;
        }
    }

void CCartoTypeDemoView::OnUpdateEditSaveKml(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(!m_framework->MapIsEmpty(m_writable_map_handle));
    }

void CCartoTypeDemoView::OnEditLoadCtms()
    {
    CFileDialog file_dialog(true,_TEXT(".ctms"),nullptr,0,TEXT("CTMS files (*.ctms)|*.ctms||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->ReadMap(m_writable_map_handle,path,CartoType::FileType::CTMS);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to read data from a CTMS file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateEditLoadCtms(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnEditSaveCtms()
    {
    CFileDialog file_dialog(false,_TEXT(".ctms"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("CTMS files (*.ctms)|*.ctms||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->SaveMap(m_writable_map_handle,path,CartoType::FileType::CTMS);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to save data to a CTMS file"),error);
            }
        else
            m_writable_map_changed = false;
        }
    }

void CCartoTypeDemoView::OnUpdateEditSaveCtms(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(!m_framework->MapIsEmpty(m_writable_map_handle));
    }

void CCartoTypeDemoView::OnEditLoadCtsql()
    {
    CFileDialog file_dialog(true,_TEXT(".ctsql"),nullptr,0,TEXT("CTSQL files (*.ctsql)|*.ctsql||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->ReadMap(m_writable_map_handle,path,CartoType::FileType::CTSQL);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to read data from a CTSQL file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateEditLoadCtsql(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }

void CCartoTypeDemoView::OnEditSaveCtsql()
    {
    CFileDialog file_dialog(false,_TEXT(".ctsql"),nullptr,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,TEXT("CartoType SQLite database files (*.ctsql)|*.ctsql||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->SaveMap(m_writable_map_handle,path,CartoType::FileType::CTSQL);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to save data to a CTSQL file"),error);
            }
        else
            m_writable_map_changed = false;
        }
    }

void CCartoTypeDemoView::OnUpdateEditSaveCtsql(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable(!m_framework->MapIsEmpty(m_writable_map_handle));
    }

void CCartoTypeDemoView::OnEditLoadGpx()
    {
    CFileDialog file_dialog(true,_TEXT(".gpx"),nullptr,0,TEXT("GPX files (*.gpx)|*.gpx||"));
    if (file_dialog.DoModal() == IDOK)
        {
        CString p = file_dialog.GetPathName();
        CartoType::String path;
        SetString(path,p);
        CartoType::Result error = m_framework->ReadGpx(m_writable_map_handle,path);
        if (error)
            {
            CCartoTypeDemoApp* app = (CCartoTypeDemoApp*)AfxGetApp();
            app->ShowError(_TEXT("failed to read data from a GPX file"),error);
            }
        Update();
        }
    }

void CCartoTypeDemoView::OnUpdateEditLoadGpx(CCmdUI *pCmdUI)
    {
    pCmdUI->Enable();
    }
