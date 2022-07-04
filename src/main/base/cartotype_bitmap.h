/*
cartotype_bitmap.h
Copyright (C) 2013-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_color.h>
#include <cartotype_errors.h>
#include <cartotype_stream.h>

namespace CartoTypeCore
{

class Bitmap;
class MInputStream;
class MOutputStream;

/** A palette of colors used in a bitmap. */
class Palette
    {
    public:
    /** Creates a palette from a vector of colors. */
    Palette(const std::vector<Color>& aColor):
        iColor(aColor)
        {
        }
    /** Returns a pointer to the color array. */
    const CartoTypeCore::Color* Color() const { return iColor.data(); }
    /** Returns the number of colors in the palette. */
    size_t ColorCount() const { return iColor.size(); }

    private:
    std::vector<CartoTypeCore::Color> iColor;
    };

/**
An enumerated type for supported bitmap types.
The number of bits per pixel is held in the low 6 bits.
*/
enum class BitmapType
    {
    /** A mask for the bits in BitmapType that represent the number of bits per pixel. */
    KBitsPerPixelMask = 63,
    /**
    The bit in BitmapType that indicates whether the type is inherently colored,
    which means that its color data is held in the pixel value.
    */
    KColored = 64,
    /**
    The bit in BitmapType indicating whether the bitmap has a palette.
    If this bit is set, EColored should not also be set.
    */
    KPalette = 128,

    /** One bit per pixel: 1 = foreground color, 0 = background color. */
    A1 = 1,
    /** Eight bits per pixel: 255 = foreground color, 0 = background color. */
    A8 = 8,
    /** 16 bits per pixel, monochrome. */
    A16 = 16,
    /**
    16 bits per pixel, accessed as 16-bit words, not as bytes;
    top 5 bits = red, middle 6 bits = green, low 5 bits = blue.
    */
    RGB16 = KColored | 16,
    /** 24 bits per pixel: first byte blue, second byte green, third byte red. */
    RGB24 = KColored | 24,
    /**
    32 bits per pixel: first byte alpha, second byte blue, second byte green, third byte red.
    The red, green and blue values are premultiplied by the alpha value.
    */
    RGBA32 = KColored | 32,
    /**
    Eight bits per pixel with a 256-entry palette.
    */
    P8 = KPalette | 8
    };

/** A bitmap that does not take ownership of pixel data. */
class BitmapView
    {
    public:
    /** Create a bitmap with a specified type, data, and dimensions. */
    BitmapView(BitmapType aType,uint8_t* aData,uint32_t aWidth,uint32_t aHeight,uint32_t aRowBytes,std::shared_ptr<Palette> aPalette = nullptr):
        iData(aData),
        iPalette(aPalette),
        iWidth(aWidth),
        iHeight(aHeight),
        iRowBytes(aRowBytes),
        iType(aType)
        {
        }
    BitmapView(const Bitmap& aBitmap) = delete;
    BitmapView& operator=(const Bitmap& aBitmap) = delete;

    /** A type for functions to supply the color of a pixel at a given point. */
    using TColorFunction = Color(*)(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    
    TColorFunction ColorFunction() const;
    Bitmap Copy(int32_t aExpansion = 0) const;
    Bitmap Blur(bool aGaussian,double aWidth) const;
    Bitmap Palettize() const;
    Bitmap UnPalettize() const;
    Bitmap Trim(Rect& aBounds,bool aTrimLeft = true,bool aTrimRight = true,bool aTrimTop = true,bool aTrimBottom = true) const;
    Bitmap Clip(Rect aClip) const;
    Bitmap Clip(const MPath& aPath,Rect& aNewBounds) const;
    Result WritePng(MOutputStream& aOutputStream,bool aPalettize) const;
    Result Write(DataOutputStream& aOutput) const;

    /** Return the bitmap type, which indicates its depth and whether it is colored. */
    BitmapType Type() const { return iType; }
    /** Return the bitmap depth: the number of bits used to store each pixel. */
    int32_t BitsPerPixel() const { return int32_t(iType) & int32_t(BitmapType::KBitsPerPixelMask); }
    /** Return a constant pointer to the start of the pixel data. */
    const uint8_t* Data() const { return iData; }
    /** Return a writable pointer to the start of the pixel data. */
    uint8_t* Data() { return iData; }
    /** Return the palette if any. */
    std::shared_ptr<CartoTypeCore::Palette> Palette() const { return iPalette; }
    /** Set the palette. */
    void SetPalette(std::shared_ptr<CartoTypeCore::Palette> aPalette) { iPalette = aPalette; }
    /**
    Return the number of bytes actually used to store the data. This may include padding
    at the ends of rows.
    */
    int32_t DataBytes() const { return iHeight * iRowBytes; }
    /** Return the width in pixels. */
    int32_t Width() const { return iWidth; }
    /** Return the height in pixels. */
    int32_t Height() const { return iHeight; }
    /** Return the number of bytes used to store each horizontal row of pixels. */
    int32_t RowBytes() const { return iRowBytes; }
    /** Clear the pixel data to zeroes. */
    void Clear() { memset(iData,0,size_t(iHeight * iRowBytes)); }
    /** Clear the pixel data to ones (normally white). */
    void ClearToWhite() { memset(iData,0xFF,size_t(iHeight * iRowBytes)); }

    /** The less-than operator. Assumes that the bitmaps are of the same type. */
    bool operator<(const BitmapView& aOther) const
        {
        if (iWidth < aOther.iWidth)
            return true;
        if (iWidth == aOther.iWidth)
            {
            if (iHeight < aOther.iHeight)
                return true;
            if (iHeight == aOther.iHeight)
                {
                if (memcmp(iData,aOther.iData,DataBytes()) < 0)
                    return true;
                }
            }
        return false;
        }

    /** The equality operator. Assumes that the bitmaps are of the same type. */
    bool operator==(const BitmapView& aOther) const
        {
        return iWidth == aOther.iWidth && iHeight == aOther.iHeight && memcmp(iData,aOther.iData,DataBytes()) == 0;
        }

    protected:
    BitmapView() = default;
    static Color Color1BitMono(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    static Color Color8BitMono(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    static Color Color8BitPalette(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    static Color Color16BitMono(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    static Color Color16BitColor(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    static Color Color24BitColor(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    static Color Color32BitColor(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);
    static Color ColorUnsupported(const BitmapView& aBitmap,uint32_t aX,uint32_t aY);

    /** The bitmap data. */
    uint8_t* iData = nullptr;
    /** The palette if any. */
    std::shared_ptr<CartoTypeCore::Palette> iPalette;
    /** The width in pixels. */
    uint32_t iWidth = 0;
    /** The height in pixels. */
    uint32_t iHeight = 0;
    /** The number of bytes in each row of pixels. */
    uint32_t iRowBytes = 0;
    /** The bitmap type. */
    BitmapType iType = BitmapType::A8;
    };

/** A bitmap that owns its data. */
class Bitmap: public BitmapView
    {
    public:
    Bitmap();
    Bitmap(BitmapType aType,int32_t aWidth,int32_t aHeight,int32_t aRowBytes = 0,std::shared_ptr<CartoTypeCore::Palette> aPalette = nullptr);
    explicit Bitmap(MInputStream& aInputStream);
    Bitmap(const Bitmap& aOther);
    Bitmap(Bitmap&& aOther) noexcept;
    Bitmap(const BitmapView& aOther);
    Bitmap& operator=(const BitmapView& aOther);
    Bitmap& operator=(Bitmap&& aOther);
    static Bitmap Read(Result& aError,DataInputStream& aInput);
    
    /** Detaches the data, transferring ownership to the caller. */
    std::vector<uint8_t> DetachData() { iData = nullptr; iWidth = iHeight = iRowBytes = 0; return std::move(iOwnData); }

    private:
    std::vector<uint8_t> iOwnData;
    };

/** A bitmap and a position to draw it. Used when drawing notices on the map. */
class PositionedBitmap
    {
    public:
    std::unique_ptr<CartoTypeCore::Bitmap> Bitmap; ///< The bitmap.
    Point TopLeft;                  ///< The position at which to draw the top-left coner of the bitmap.
    };

}
