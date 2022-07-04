/*
cartotype_style_sheet_data.h
Copyright (C) 2016-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_stream.h>
#include <string>

namespace CartoTypeCore
{

/** Style sheet data stored in XML format as text or in a file. */
class StyleSheetData
    {
    public:
    StyleSheetData() = default;
    /** Creates a StyleSheetData object by copying another one. */
    StyleSheetData(const StyleSheetData& aOther) = default;
    /** Creates a StyleSheetData object by moving data from another one. */
    StyleSheetData(StyleSheetData&& aOther) = default;
    /** The assignment operator. */
    StyleSheetData& operator=(const StyleSheetData& aOther) = default;
    /** The move assignment operator. */
    StyleSheetData& operator=(StyleSheetData&& aOther) = default;
    /** Creates a StyleSheetData object representing data from a named file. */
    explicit StyleSheetData(const char* aFileName);
    /** Creates a StyleSheetData object representing data stored in memory. Copies the data. Throws KErrorInvalidArgument if aData is null or aLength is zero. */
    StyleSheetData(const uint8_t* aData,size_t aLength);
    /** Reloads the data in a StyleSheetData object from the file if any. */
    void Reload();
    /** Creates a memory input stream representing the style sheet data. */
    MemoryInputStream Stream() const { return MemoryInputStream((const uint8_t*)iText.data(),iText.length()); }
    /** Returns the filename associated with this data, or the empty string if there is none. */
    const std::string& FileName() const { return iFileName; }
    /** Returns the text of this style sheet data. */
    const std::string& Text() const { return iText; }
    /** Returns true if the style sheet data is empty, which means there is no text and no filename. */
    bool IsEmpty() const { return iText.empty() && iFileName.empty(); }

    private:
    std::string iText;
    std::string iFileName;
    };

/** A set of style sheet data that may consist of more than one style sheet. */
using StyleSheetDataArray = std::vector<StyleSheetData>;

}
