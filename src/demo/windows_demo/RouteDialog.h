#pragma once

#include <afxwin.h>
#include "Resource.h"
#include <cartotype_framework.h>

class CRouteDialog : public CDialog
    {
    DECLARE_DYNAMIC(CRouteDialog)

    // Dialog Data
    //{{AFX_DATA(CRouteDialog)
    CString	Address;
    CString PushPinName;
    CString PushPinDesc;
    CString Heading;
    int		ButtonIndex;
    int     UseAsWayPoint;
    int     BestRoute;
    //}}AFX_DATA

    public:
        CRouteDialog(bool aPushPinSelected,CWnd* pParent = NULL);   // standard constructor
        virtual ~CRouteDialog();

        // Dialog Data
        enum { IDD = IDD_ROUTE_DIALOG };

        afx_msg void OnRadioButtonClicked();

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual BOOL OnInitDialog();

        DECLARE_MESSAGE_MAP()

    private:
        void ShowOrHideFields();

        bool m_push_pin_selected;
    };
