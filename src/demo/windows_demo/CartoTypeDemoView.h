
// CartoTypeDemoView.h : interface of the CCartoTypeDemoView class
//

#pragma once

#include "FindDialog.h"

// An extended bitmap info class with entries for bitfield specifiers.
class BitmapInfoX
    {
    public:
        BitmapInfoX()
            {
            memset(this,0,sizeof(*this));
            }

        BITMAPV4HEADER bmiHeader;
    };

class CCartoTypeDemoView : public CView
{
protected: // create from serialization only
    CCartoTypeDemoView();
    DECLARE_DYNCREATE(CCartoTypeDemoView)

// Attributes
public:
    CCartoTypeDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();

protected:

// Implementation
public:
    virtual ~CCartoTypeDemoView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags,CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta,CPoint pt);
    afx_msg void OnRButtonUp(UINT nFlags,CPoint point);
    afx_msg void OnSize(UINT nType,int cx,int cy);
    afx_msg void OnAppAbout();
    afx_msg void OnFileOpenInCurrentMap();
    afx_msg void OnFileClose();
    afx_msg void OnUpdateFileClose(CCmdUI *pCmdUI);
    afx_msg void OnFileSaveAsPng();
    afx_msg void OnFileSaveAsCtsql();
    afx_msg void OnFind();
    afx_msg void OnUpdateFind(CCmdUI *pCmdUI);
    afx_msg void OnFindAddress();
    afx_msg void OnUpdateFindAddress(CCmdUI *pCmdUI);
    afx_msg void OnFindNext();
    afx_msg void OnUpdateFindNext(CCmdUI *pCmdUI);
    afx_msg void OnViewReloadStyleSheet();
    afx_msg void OnUpdateViewReloadStyleSheet(CCmdUI *pCmdUI);
    afx_msg void OnViewZoomIn();
    afx_msg void OnViewZoomOut();
    afx_msg void OnUpdateViewZoomIn(CCmdUI *pCmdUI);
    afx_msg void OnUpdateViewZoomOut(CCmdUI *pCmdUI);
    afx_msg void OnViewRotate();
    afx_msg void OnUpdateViewRotate(CCmdUI *pCmdUI);
    afx_msg void OnViewNorthup();
    afx_msg void OnUpdateViewNorthup(CCmdUI *pCmdUI);
    afx_msg void OnViewLatlong();
    afx_msg void OnUpdateViewLatlong(CCmdUI *pCmdUI);
    afx_msg void OnViewStylesheet();
    afx_msg void OnUpdateViewStylesheet(CCmdUI *pCmdUI);
    afx_msg void OnViewLayers();
    afx_msg void OnUpdateViewLayers(CCmdUI *pCmdUI);
    afx_msg void OnViewLegend();
    afx_msg void OnUpdateViewLegend(CCmdUI *pCmdUI);
    afx_msg void OnViewSetScale();
    afx_msg void OnUpdateViewSetScale(CCmdUI *pCmdUI);
    afx_msg void OnViewPerspective();
    afx_msg void OnUpdateViewPerspective(CCmdUI *pCmdUI);
    afx_msg void OnViewMetricUnits();
    afx_msg void OnUpdateViewMetricUnits(CCmdUI *pCmdUI);
    afx_msg void OnViewTurnInstructions();
    afx_msg void OnUpdateViewTurnInstructions(CCmdUI *pCmdUI);
    afx_msg void OnViewFlyThrough();
    afx_msg void OnUpdateViewFlyThrough(CCmdUI *pCmdUI);
    afx_msg void OnGoAlongRoute();
    afx_msg void OnUpdateGoAlongRoute(CCmdUI *pCmdUI);
    afx_msg void OnReverseRoute();
    afx_msg void OnUpdateReverseRoute(CCmdUI* pCmdUI);
    afx_msg void OnDeleteRoute();
    afx_msg void OnUpdateDeleteRoute(CCmdUI* pCmdUI);
    afx_msg void OnSetDriveProfile();
    afx_msg void OnUpdateSetDriveProfile(CCmdUI *pCmdUI);
    afx_msg void OnSetCycleProfile();
    afx_msg void OnUpdateSetCycleProfile(CCmdUI *pCmdUI);
    afx_msg void OnSetWalkProfile();
    afx_msg void OnUpdateSetWalkProfile(CCmdUI *pCmdUI);
    afx_msg void OnSetHikeProfile();
    afx_msg void OnUpdateSetHikeProfile(CCmdUI *pCmdUI);
    afx_msg void OnSetSkiProfile();
    afx_msg void OnUpdateSetSkiProfile(CCmdUI* pCmdUI);
    afx_msg void OnShortestRoute();
    afx_msg void OnUpdateShortestRoute(CCmdUI *pCmdUI);
    afx_msg void OnUseGradients();
    afx_msg void OnUpdateUseGradients(CCmdUI *pCmdUI);
    afx_msg void OnSaveRouteInstructions();
    afx_msg void OnUpdateSaveRouteInstructions(CCmdUI *pCmdUI);
    afx_msg void OnSaveRouteXml();
    afx_msg void OnUpdateSaveRouteXml(CCmdUI *pCmdUI);
    afx_msg void OnSaveRouteGpx();
    afx_msg void OnUpdateSaveRouteGpx(CCmdUI *pCmdUI);
    afx_msg void OnLoadRouteXml();
    afx_msg void OnUpdateLoadRouteXml(CCmdUI *pCmdUI);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnEditLoadKml();
    afx_msg void OnUpdateEditLoadKml(CCmdUI *pCmdUI);
    afx_msg void OnEditSaveKml();
    afx_msg void OnUpdateEditSaveKml(CCmdUI *pCmdUI);
    afx_msg void OnEditLoadCtms();
    afx_msg void OnUpdateEditLoadCtms(CCmdUI *pCmdUI);
    afx_msg void OnEditSaveCtms();
    afx_msg void OnUpdateEditSaveCtms(CCmdUI *pCmdUI);
    afx_msg void OnEditLoadCtsql();
    afx_msg void OnUpdateEditLoadCtsql(CCmdUI *pCmdUI);
    afx_msg void OnEditSaveCtsql();
    afx_msg void OnUpdateEditSaveCtsql(CCmdUI *pCmdUI);
    afx_msg void OnEditLoadGpx();
    afx_msg void OnUpdateEditLoadGpx(CCmdUI *pCmdUI);
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()

private:
    void TestCode();
    bool SetCentre(CPoint& aPoint);
    void Update() { Invalidate(false); }
    void PanToDraggedPosition();
    void StopDragging();
    const CartoType::BitmapView* MapBitmap(CartoType::Result& aError,const CartoType::Rect& aMapClientArea,bool& aRedrawNeeded);
    void CreateLegend();
    void ShowNextFoundObject();
    CartoType::Result LoadStyleSheet(const ::CString& aPath);
    void SimulateNavigationFix(bool aGoToNextTurn,bool& aAtEnd);
    void StopFlyThrough();
    void CalculateAndDisplayRoute();
    void SetProfile(size_t aProfileIndex);
    void SetMenuItemToBuiltInProfileName(CCmdUI* aMenuItem,size_t aProfileIndex);
    void OutputMapExtent();

    std::unique_ptr<CartoType::Framework> m_framework;
    CFindTextDialog m_find_text_dialog;
    CartoType::MapObjectArray m_found_object;
    CartoType::Address m_address;
    int m_found_object_index = 0;
    uint64_t m_found_object_id = 0;                            // map object ID of found object 
    bool m_draw_legend = false;							    // if true and if there is room, draw the legend
    bool m_draw_turn_instructions = true;				        // if true, draw turn instructions if any
    bool m_map_drag_enabled = false;						    // true if dragging the map
    CartoType::Point m_map_drag_anchor;
    CartoType::PointFP m_map_drag_anchor_in_map_coords;
    CartoType::Point m_map_drag_offset;
    UINT m_fly_through_timer = 0;								// if non-zero, ID of timer used to fly through a route
    double m_timer_interval_in_seconds = 0.5;
    double m_fly_through_speed_in_kph = 5;
    std::unique_ptr<CartoType::RouteIterator> m_route_iter;	// if non-null, an iterator used to traverse a route
    double m_on_route_time = 0;                                // simulated time in seconds on the route

    UINT m_manual_draw_timer = 0;

    class RoutePoint 
        {
        public:
        CartoType::RoutePoint Point;
        uint64_t Id = 0;
        };

    std::vector<RoutePoint> m_route_point_array;
    size_t m_route_profile_index = 0;
    bool m_shortest_route = false;
    bool m_use_gradients = false;
    bool m_metric_units = true;
    std::unique_ptr<CartoType::GraphicsContext> m_extra_gc;
    uint32_t m_writable_map_handle = 0;
    bool m_writable_map_changed = false;
    CartoType::String m_pushpin_name;
    CartoType::String m_pushpin_desc;
    bool m_best_route = false;                                // calculate the best route through the waypoints don't just take them in order
    std::vector<uint32_t> m_map_handle;
    bool m_shift_left_click_simulates_navigation_fix = true;

    std::unique_ptr<CartoType::MapRenderer> m_map_renderer;
};

#ifndef _DEBUG  // debug version in CartoTypeDemoView.cpp
inline CCartoTypeDemoDoc* CCartoTypeDemoView::GetDocument() const
   { return reinterpret_cast<CCartoTypeDemoDoc*>(m_pDocument); }
#endif

