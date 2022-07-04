// FindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CartoTypeDemo.h"
#include "FindDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFindTextDialog::CFindTextDialog():
    CDialog(CFindTextDialog::IDD,nullptr),
    m_map_object_group_array(std::make_unique<CartoType::MapObjectGroupArray>())
    {
    m_find_param.MaxObjectCount = 64;
    m_find_param.Attributes = "$,name:*,ref,alt_name,int_name,addr:housename,pco";
    m_find_param.Condition = "@feature_type!='bsp'"; // exclude bus stops
    m_find_param.StringMatchMethod = CartoType::StringMatchMethod(CartoType::StringMatchMethodFlag::Prefix,
                                                                 CartoType::StringMatchMethodFlag::FoldAccents,
                                                                 CartoType::StringMatchMethodFlag::FoldCase,
                                                                 CartoType::StringMatchMethodFlag::IgnoreSymbols,
                                                                 CartoType::StringMatchMethodFlag::IgnoreWhitespace);
    m_find_param.TimeOut = 2;
    m_find_param.Merge = true;
    }

void CFindTextDialog::Set(CartoType::Framework* aFramework)
    {
    std::lock_guard<std::mutex> lock(m_map_object_group_array_mutex);
    m_framework = aFramework;
    m_map_object_group_array->clear();
    CartoType::RectFP view;
    m_framework->GetView(view,CartoType::CoordType::Map);
    m_find_param.Location = CartoType::Geometry(view,CartoType::CoordType::Map);
    }

CartoType::MapObjectArray CFindTextDialog::FoundObjectArray()
    {
    CartoType::MapObjectArray a;

    std::lock_guard<std::mutex> lock(m_map_object_group_array_mutex);
    if (m_list_box_index >= 0 && size_t(m_list_box_index) < m_map_object_group_array->size())
        a = std::move((*m_map_object_group_array)[m_list_box_index].MapObjectArray);

    else
        {
        for (auto& p : *m_map_object_group_array)
            {
            for (auto& o : p.MapObjectArray)
                a.push_back(std::move(o));
            }
        }

    m_map_object_group_array->clear();
    return a;
    }

void CFindTextDialog::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFindTextDialog)
    DDX_Text(pDX,IDC_FIND_TEXT,FindText);
    DDX_Check(pDX,IDC_FIND_SYMBOLS,Symbols);
    DDX_Check(pDX,IDC_FIND_FUZZY,Fuzzy);
    //}}AFX_DATA_MAP
    }

BEGIN_MESSAGE_MAP(CFindTextDialog,CDialog)
    ON_CBN_EDITCHANGE(IDC_FIND_TEXT,OnEditChange)
    ON_CBN_DBLCLK(IDC_FIND_TEXT,OnComboBoxDoubleClick)
    ON_CBN_SELCHANGE(IDC_FIND_TEXT,OnComboBoxSelChange)
END_MESSAGE_MAP()

BOOL CFindTextDialog::OnInitDialog()
    {
    CComboBox* cb = (CComboBox*)GetDlgItem(IDC_FIND_TEXT);
    cb->SetHorizontalExtent(400);
    UpdateData(0);
    PopulateComboBox();
    return true;
    }

void CFindTextDialog::OnEditChange()
    {
    PopulateComboBox();
    }

void CFindTextDialog::OnComboBoxDoubleClick()
    {
    UpdateMatch();
    EndDialog(IDOK);
    }

void CFindTextDialog::OnComboBoxSelChange()
    {
    UpdateMatch();
    }

void CFindTextDialog::PopulateComboBox()
    {
    // Copy the text and button states into the member variables.
    UpdateData(1);

    if (FindText.IsEmpty())
        {
        std::unique_lock<std::mutex> lock(m_map_object_group_array_mutex);
        m_map_object_group_array->clear();
        lock.unlock();
        SetComboboxStrings();
        return;
        }

    // Get the current text.
    CartoType::String text;
    SetString(text,FindText);

    // Find up to 64 items starting with the current text or, if the text is the name of a feature type, matching that feature type.
    auto poi = CartoType::FeatureTypeFromName(text);

    auto find_callback = [this](std::unique_ptr<CartoType::MapObjectGroupArray> aMapObjectGroupArray)
        {
        std::unique_lock<std::mutex> lock(m_map_object_group_array_mutex);
        m_map_object_group_array = std::move(aMapObjectGroupArray);
        lock.unlock();
        SetComboboxStrings();
        };

    if (poi != CartoType::FeatureType::Invalid)
        {
        CartoType::FindNearbyParam param;
        param.Type = poi;
        param.Location = m_find_param.Location;
        m_framework->FindAsync(find_callback,param,true);
        }
    else
        {
        CartoType::FindParam param(m_find_param);
        param.Text = text;
        if (Fuzzy)
            param.StringMatchMethod = CartoType::StringMatchMethod::Fuzzy;
        if (Symbols)
            param.StringMatchMethod -= CartoType::StringMatchMethodFlag::IgnoreSymbols;
        m_framework->FindAsync(find_callback,param,true);
        }

    // Put them in the combo box.
    SetComboboxStrings();
    }

void CFindTextDialog::SetComboboxStrings()
    {
    // The window handle is null if this is called from the find callback after the dialog window has closed.
    if (!m_hWnd)
        return;

    /*
    Note on locks: not trying the lock, but setting it unconditionally, causes a deadlock; I don't know why.
    It may be because the find thread is blocked in a wait().
    */
    std::unique_lock<std::mutex> lock(m_map_object_group_array_mutex,std::defer_lock);
    if (!lock.try_lock())
        return;

    CComboBox* cb = (CComboBox*)GetDlgItem(IDC_FIND_TEXT);
    for (int i = cb->GetCount(); i >= 0; i--)
        cb->DeleteString(i);
    for (const auto& cur_group : *m_map_object_group_array)
        {
        CString s;
        SetString(s,cur_group.Name);
        cb->AddString(s);
        }
    }

void CFindTextDialog::UpdateMatch()
    {
    CComboBox* cb = (CComboBox*)GetDlgItem(IDC_FIND_TEXT);
    m_list_box_index = cb->GetCurSel();
    }

CFindAddressDialog::CFindAddressDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CFindAddressDialog::IDD,pParent)
    {
    //{{AFX_DATA_INIT(CFindAddressDialog)
    Building = _T("");
    Feature = _T("");
    Street = _T("");
    SubLocality = _T("");
    Locality = _T("");
    SubAdminArea = _T("");
    AdminArea = _T("");
    Country = _T("");
    PostCode = _T("");
    //}}AFX_DATA_INIT
    }

void CFindAddressDialog::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFindAddressDialog)
    DDX_Text(pDX,IDC_FIND_BUILDING,Building);
    DDX_Text(pDX,IDC_FIND_FEATURE,Feature);
    DDX_Text(pDX,IDC_FIND_STREET,Street);
    DDX_Text(pDX,IDC_FIND_SUBLOCALITY,SubLocality);
    DDX_Text(pDX,IDC_FIND_LOCALITY,Locality);
    DDX_Text(pDX,IDC_FIND_SUBADMINAREA,SubAdminArea);
    DDX_Text(pDX,IDC_FIND_ADMINAREA,AdminArea);
    DDX_Text(pDX,IDC_FIND_COUNTRY,Country);
    DDX_Text(pDX,IDC_FIND_POSTCODE,PostCode);
    //}}AFX_DATA_MAP
    }

BEGIN_MESSAGE_MAP(CFindAddressDialog,CDialog)
    //{{AFX_MSG_MAP(CFindAddressDialog)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
