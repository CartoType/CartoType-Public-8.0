
// CartoTypeDemo.h : main header file for the CartoTypeDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include <memory>

#include "resource.h"       // main symbols


// CCartoTypeDemoApp:
// See CartoTypeDemo.cpp for the implementation of this class
//

#include <cartotype.h>

class CCartoTypeDemoApp : public CWinAppEx
{
public:
	CCartoTypeDemoApp();
    ~CCartoTypeDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
    void ShowError(const _TCHAR* aMessage);
    void ShowError(const _TCHAR* aMessage,int aError);
    void ShowError(const std::string& aMessage,int aError);
    void ShowError(int aError);
    std::shared_ptr<CartoType::FrameworkEngine> Engine() { return m_engine; }
    const CartoType::MString& DefaultStyleSheetName() const { return m_default_style_sheet_name; }
    double ZoomFactor() const { return m_zoom_factor; }

private:
    void GetFontPath(CartoType::String& aPath,const ::CString& aAppPath,const char* aFontName);
    CartoType::Result LoadFont(const ::CString& aAppPath,const char* aFontName);
    CartoType::Result Configure(const ::CString& aAppPath,const char* aFileName);
    CartoType::Result InitializeCartoType();

    std::shared_ptr<CartoType::FrameworkEngine> m_engine;
    CartoType::String m_default_style_sheet_name;
    double m_zoom_factor;
    };


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
    {
    public:
        CAboutDlg();

        // Dialog Data
            // Dialog Data
            //{{AFX_DATA(CAboutDlg)
        enum { IDD = IDD_ABOUTBOX };
        CString	iTextLine1;
        CString	iTextLine2;
        CString iTextLine3;
        //}}AFX_DATA

    protected:
        //{{AFX_VIRTUAL(CLatLongDialog)
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

    // Implementation
    protected:
        DECLARE_MESSAGE_MAP()
    };

static inline void SetString(CartoType::MString& aDest, LPCTSTR aSource)
    {
    aDest.Set((const uint16_t*)aSource);
    }

static inline void SetString(::CString& aDest,const char* aSource)
    {
    CartoType::String s(aSource);
    aDest.SetString((LPCTSTR)s.Data(),(int)s.Length());
    }

static inline void SetString(::CString& aDest, const CartoType::MString& aSource)
    {
    aDest.SetString((LPCTSTR)aSource.Data(),(int)aSource.Length());
    }

extern CCartoTypeDemoApp theApp;
