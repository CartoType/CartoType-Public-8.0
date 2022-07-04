// LayerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CartoTypeDemo.h"
#include "LayerDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerDialog dialog


CLayerDialog::CLayerDialog(CartoType::Framework& aFramework,CWnd* pParent /*=NULL*/) :
CDialog(CLayerDialog::IDD,pParent),
m_framework(aFramework)
    {
    EnableAutomation();

    //{{AFX_DATA_INIT(CLayerDialog)
    //}}AFX_DATA_INIT
    }


void CLayerDialog::OnFinalRelease()
    {
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.

    CDialog::OnFinalRelease();
    }

void CLayerDialog::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLayerDialog)
    //}}AFX_DATA_MAP
    DDX_Control(pDX,IDC_CHECK1,Check[0]);
    DDX_Control(pDX,IDC_CHECK2,Check[1]);
    DDX_Control(pDX,IDC_CHECK3,Check[2]);
    DDX_Control(pDX,IDC_CHECK4,Check[3]);
    DDX_Control(pDX,IDC_CHECK5,Check[4]);
    DDX_Control(pDX,IDC_CHECK6,Check[5]);
    DDX_Control(pDX,IDC_CHECK7,Check[6]);
    DDX_Control(pDX,IDC_CHECK8,Check[7]);
    DDX_Control(pDX,IDC_CHECK9,Check[8]);
    DDX_Control(pDX,IDC_CHECK10,Check[9]);
    DDX_Control(pDX,IDC_CHECK11,Check[10]);
    DDX_Control(pDX,IDC_CHECK12,Check[11]);
    DDX_Control(pDX,IDC_CHECK13,Check[12]);
    DDX_Control(pDX,IDC_CHECK14,Check[13]);
    DDX_Control(pDX,IDC_CHECK15,Check[14]);
    DDX_Control(pDX,IDC_CHECK16,Check[15]);
    DDX_Control(pDX,IDC_CHECK17,Check[16]);
    DDX_Control(pDX,IDC_CHECK18,Check[17]);
    DDX_Control(pDX,IDC_CHECK19,Check[18]);
    DDX_Control(pDX,IDC_CHECK20,Check[19]);
    DDX_Control(pDX,IDC_CHECK21,Check[20]);
    DDX_Control(pDX,IDC_CHECK22,Check[21]);
    DDX_Control(pDX,IDC_CHECK23,Check[22]);
    DDX_Control(pDX,IDC_CHECK24,Check[23]);
    DDX_Control(pDX,IDC_CHECK25,Check[24]);
    DDX_Control(pDX,IDC_CHECK26,Check[25]);
    DDX_Control(pDX,IDC_CHECK27,Check[26]);
    DDX_Control(pDX,IDC_CHECK28,Check[27]);
    DDX_Control(pDX,IDC_CHECK29,Check[28]);
    DDX_Control(pDX,IDC_CHECK30,Check[29]);
    DDX_Control(pDX,IDC_CHECK31,Check[30]);
    DDX_Control(pDX,IDC_CHECK32,Check[31]);
    }


BEGIN_MESSAGE_MAP(CLayerDialog,CDialog)
    //{{AFX_MSG_MAP(CLayerDialog)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLayerDialog,CDialog)
    //{{AFX_DISPATCH_MAP(CLayerDialog)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ILayerDialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {35690769-F374-4198-9935-52A3E770ADFC}
static const IID IID_ILayerDialog =
    { 0x35690769,0xf374,0x4198,{ 0x99,0x35,0x52,0xa3,0xe7,0x70,0xad,0xfc } };

BEGIN_INTERFACE_MAP(CLayerDialog,CDialog)
    INTERFACE_PART(CLayerDialog,IID_ILayerDialog,Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerDialog message handlers

BOOL CLayerDialog::OnInitDialog()
    {
    CDialog::OnInitDialog();

    // Set the buttons to the layer names.
    m_layer_names = m_framework.LayerNames();
    size_t layers = m_layer_names.size();
    size_t i = 0;
    while (i < layers && i < 32)
        {
        const CartoType::MString& layer_name = m_layer_names[i];
#ifdef _UNICODE
        CartoType::String text;
        text.Set(layer_name);
        text.Append((uint16_t)0);
        Check[i].SetWindowText((LPCTSTR)text.Data());
#else
        CartoType::CUtf8String text(layer_name);
        iCheck[i].SetWindowText(text);
#endif
        Check[i].SetCheck(m_framework.LayerIsEnabled(layer_name));
        i++;
        }
    while (i < 32)
        {
        Check[i].ShowWindow(SW_HIDE);
        i++;
        }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    }

void CLayerDialog::OnOK()
    {
    // Set the layer parameters according to the check boxes.
    size_t layers = m_layer_names.size();
    for (size_t i = 0; i < layers && i < 32; i++)
        m_framework.EnableLayer(m_layer_names[i],Check[i].GetCheck() == 1);

    CDialog::OnOK();
    }
