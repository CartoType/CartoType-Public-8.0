#if !defined(AFX_FINDDIALOG_H__BC253915_DC2E_4FB7_BC09_C574263FB0A3__INCLUDED_)
#define AFX_FINDDIALOG_H__BC253915_DC2E_4FB7_BC09_C574263FB0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>
#include "Resource.h"
#include <cartotype_framework.h>
#include <mutex>

class CFindTextDialog : public CDialog
    {
    // Construction
    public:
        CFindTextDialog();
        void Set(CartoType::Framework* aFramework);
        CartoType::MapObjectArray FoundObjectArray();

        // Dialog Data
        //{{AFX_DATA(CFindTextDialog)
        enum { IDD = IDD_FIND_TEXT_DIALOG };
        CString	FindText;
        int Symbols = 0;
        int Fuzzy = 0;
        //}}AFX_DATA

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFindTextDialog)

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual BOOL OnInitDialog();
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        afx_msg void OnEditChange();
        afx_msg void OnComboBoxDoubleClick();
        afx_msg void OnComboBoxSelChange();
        DECLARE_MESSAGE_MAP()

    private:
        void PopulateComboBox();
        void SetComboboxStrings();
        void UpdateMatch();

        CartoType::Framework* m_framework = nullptr;
        CartoType::FindParam m_find_param;
        std::unique_ptr<CartoType::MapObjectGroupArray> m_map_object_group_array;
        std::mutex m_map_object_group_array_mutex;
        int m_list_box_index = -1;
    };

class CFindAddressDialog : public CDialog
    {
    // Construction
    public:
        CFindAddressDialog(CWnd* pParent = 0);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CFindAddressDialog)
        enum { IDD = IDD_FIND_ADDRESS_DIALOG };
        CString	FindText;
        CString Building;
        CString Feature;
        CString Street;
        CString SubLocality;
        CString Locality;
        CString SubAdminArea;
        CString AdminArea;
        CString Country;
        CString PostCode;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFindAddressDialog)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CFindAddressDialog)
        // NOTE: the ClassWizard will add member functions here
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDDIALOG_H__BC253915_DC2E_4FB7_BC09_C574263FB0A3__INCLUDED_)
