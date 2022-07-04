
// CartoTypeDemoDoc.h : interface of the CCartoTypeDemoDoc class
//


#pragma once

#include <memory>

class CCartoTypeDemoDoc : public CDocument
    {
    protected: // create from serialization only
        CCartoTypeDemoDoc();
        DECLARE_DYNCREATE(CCartoTypeDemoDoc)

        // Overrides
    public:
        virtual void Serialize(CArchive& ar);
        virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
#ifdef SHARED_HANDLERS
        virtual void InitializeSearchContent();
        virtual void OnDrawThumbnail(CDC& dc,LPRECT lprcBounds);
#endif // SHARED_HANDLERS

        // Implementation
    public:
        virtual ~CCartoTypeDemoDoc();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif
        auto MapDataSet() const { return m_map_data_set; }

        // Generated message map functions
    protected:
        DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
        // Helper function that sets search content for a Search Handler
        void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
    
    private:
        std::shared_ptr<CartoType::FrameworkMapDataSet> m_map_data_set;
    public:
        virtual BOOL OnNewDocument();
    };
