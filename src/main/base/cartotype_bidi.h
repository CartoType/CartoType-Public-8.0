/*
cartotype_bidi.h
Copyright (C) 2008-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_char.h>
#include <cartotype_errors.h>

namespace CartoTypeCore
{

/** The override status during bidirectional reordering. */
enum class BidiOverride
    {
    Neutral,
    LeftToRight,
    RightToLeft
    };

/** A run of text in a certain birectional type. */
class BidiRun
    {
    public:
    /** The length of the run in bytes. */
    int32_t Length { 0 };
    /** The bidirectional type of the run. */
    BidiType Type { BidiType::ON };
    /** The level of the run as usded in the Unicode Bidirectional Algorithm. */
    int32_t Level { 0 };
    };

/** The paragraph direction used during bidirectional reordering. */
enum class BidiParDir
    {
    /**
    Set the paragraph direction from the first strongly directional character.
    If there are none, use left-to-right.
    */
    LeftToRightPreferred,
    /**
    Set the paragraph direction from the first strongly directional character.
    If there are none, use right-to-left.
    */
    RightToLeftPreferred,
    /** Set the paragraph direction to left-to-right whatever its content. */
    LeftToRightForced,
    /** Set the paragraph direction to right-to-left whatever its content. */
    RightToLeftForced
    };

/**
An engine for doing bidirectional reordering and contextual shaping, and storing state
when reordering and shaping a series of lines.
*/
class BidiEngine
    {
    public:
    BidiEngine();

    void Order(uint16_t* aText,size_t aLength,size_t& aNewLength,BidiParDir aParDir,bool aParStart,int32_t* aUserData = nullptr);
    static int32_t ShapeArabic(uint16_t* aText,size_t aLength);
    
    /** Return the resolved direction; meaningful only after Order has been called at least once. */
    bool RightToLeft() { return (iStack[iStackLevel].iLevel & 1) != 0; }

    private:
    void operator=(const BidiEngine&) = delete;
    void operator=(BidiEngine&&) = delete;
    BidiEngine(const BidiEngine&) = delete;
    BidiEngine(BidiEngine&&) = delete;

    void Push(bool aRightToLeft,BidiOverride aOverride);
    static void ShapeAndMirror(uint16_t* aText,size_t& aNewLength,bool aHaveArabic,int32_t* aUserData,BidiRun* aRunStart,BidiRun* aRunEnd);
    
    static constexpr int KRunArraySize = 8;
    static constexpr int KLevels = 63; // legal levels are 0...62 

    class StackItem
        {
        public:
        uint8_t iLevel;
        uint8_t iOverride;
        };

    StackItem iStack[KLevels];
    int32_t iStackLevel { 0 };
    };

}
