﻿//############################################################################
//      シリアライザーの組み込み型定義ファイル
//          適切なマクロを定義してprimitive.incをインクルードすることで、
//          必要なコードを生成する。
//
//          下記マクロをプリミティブ処理の定義用に定義し、使用している。
//              全て(THEOLIZER_INTERNAL_DEF_PRIMITIVE)
//                  整数型(THEOLIZER_INTERNAL_DEF_INTEGRAL
//                         THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL
//                         THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL)
//                      各整数型(int, unsigned等)処理用個別定義マクロ
//                  浮動小数点型(THEOLIZER_INTERNAL_DEF_FLOATING_POINT)
//                      浮動小数点型(float, double)処理用個別定義マクロ
//
//          パラメータ
//              dType   型名(intやstd::string)
//              dSymbol 実体化に記述しているようなシンボルに展開される
/*
    © 2016 Theoride Technology (http://theolizer.com/) All Rights Reserved.
    "Theolizer" is a registered trademark of Theoride Technology.

    "Theolizer" License
        In the case where you are in possession of a valid “Theolizer” License,
        you may use this file in accordance with the terms and conditions of 
        the use license determined by Theoride Technology.

    General Public License Version 3 ("GPLv3")
        You may use this file in accordance with the terms and conditions of 
        GPLv3 published by Free Software Foundation.
        Please confirm the contents of GPLv3 at https://www.gnu.org/licenses/gpl.txt .
        A copy of GPLv3 is also saved in a LICENSE.TXT file.

    商用ライセンス
        あなたが有効なTheolizer商用ライセンスを保持している場合、
        セオライド テクノロジーの定める使用許諾書の条件に従って、
        このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        またGPLv3のコピーをLICENSE.TXTファイルにおいてます。
*/
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4100)
#endif

// ***************************************************************************
//      save/loadPrimitive宣言用マクロ
//          THEOLIZER_INTERNAL_DEF_SAVE : savePrimitive()宣言
//          THEOLIZER_INTERNAL_DEF_LOAD : loadPrimitive()宣言
// ***************************************************************************

#if defined(THEOLIZER_INTERNAL_DEF_SAVE) && defined(THEOLIZER_INTERNAL_DEF_LOAD)
    #define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSimbol)                \
        void savePrimitive(dType const& iPrimitive);                        \
        void loadPrimitive(dType      & iPrimitive);
#elif defined(THEOLIZER_INTERNAL_DEF_SAVE)
    #define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSimbol)                \
        void savePrimitive(dType const& iPrimitive);
#elif defined(THEOLIZER_INTERNAL_DEF_LOAD)
    #define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSimbol)                \
        void loadPrimitive(dType& iPrimitive);
#endif

#undef  THEOLIZER_INTERNAL_DEF_SAVE
#undef  THEOLIZER_INTERNAL_DEF_LOAD

// ***************************************************************************
//      簡単化サポート
// ***************************************************************************

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

#ifndef THEOLIZER_INTERNAL_DEF_PRIMITIVE
    #define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)
#endif

//----------------------------------------------------------------------------
//      数値型
//----------------------------------------------------------------------------

#ifndef THEOLIZER_INTERNAL_DEF_ARITHMETIC
    #define THEOLIZER_INTERNAL_DEF_ARITHMETIC(dType, dSymbol)               \
        THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)
#endif

//----------------------------------------------------------------------------
//      整数型(signedとunsignedがプラット・フォームで変わるもの)
//----------------------------------------------------------------------------

#ifndef THEOLIZER_INTERNAL_DEF_INTEGRAL
    #define THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)                 \
        THEOLIZER_INTERNAL_DEF_ARITHMETIC(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_BOOL
    #define THEOLIZER_INTERNAL_DEF_BOOL(dType, dSymbol)                     \
        THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_CHAR
    #define THEOLIZER_INTERNAL_DEF_CHAR(dType, dSymbol)                     \
        THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_WCHAR
    #define THEOLIZER_INTERNAL_DEF_WCHAR(dType, dSymbol)                    \
        THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_CHAR16
    #define THEOLIZER_INTERNAL_DEF_CHAR16(dType, dSymbol)                   \
        THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_CHAR32
    #define THEOLIZER_INTERNAL_DEF_CHAR32(dType, dSymbol)                   \
        THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)
#endif

//----------------------------------------------------------------------------
//      符号有り整数型
//----------------------------------------------------------------------------

#ifndef THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL
    #define THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL(dType, dSymbol)          \
        THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_SIGNED_CHAR
    #define THEOLIZER_INTERNAL_DEF_SIGNED_CHAR(dType, dSymbol)              \
        THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_SIGNED_SHORT
    #define THEOLIZER_INTERNAL_DEF_SIGNED_SHORT(dType, dSymbol)             \
        THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_SIGNED_INT
    #define THEOLIZER_INTERNAL_DEF_SIGNED_INT(dType, dSymbol)               \
        THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_SIGNED_LONG
    #define THEOLIZER_INTERNAL_DEF_SIGNED_LONG(dType, dSymbol)              \
        THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_SIGNED_LONG_LONG
    #define THEOLIZER_INTERNAL_DEF_SIGNED_LONG_LONG(dType, dSymbol)         \
        THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL(dType, dSymbol)
#endif

//----------------------------------------------------------------------------
//      非負整数型
//----------------------------------------------------------------------------

#ifndef THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL
    #define THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL(dType, dSymbol)        \
        THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_UNSIGNED_CHAR
    #define THEOLIZER_INTERNAL_DEF_UNSIGNED_CHAR(dType, dSymbol)            \
        THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_UNSIGNED_SHORT
    #define THEOLIZER_INTERNAL_DEF_UNSIGNED_SHORT(dType, dSymbol)           \
        THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_UNSIGNED_INT
    #define THEOLIZER_INTERNAL_DEF_UNSIGNED_INT(dType, dSymbol)             \
        THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG
    #define THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG(dType, dSymbol)            \
        THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG_LONG
    #define THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG_LONG(dType, dSymbol)       \
        THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL(dType, dSymbol)
#endif

//----------------------------------------------------------------------------
//      浮動小数点型
//----------------------------------------------------------------------------

#ifndef THEOLIZER_INTERNAL_DEF_FLOATING_POINT
    #define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSymbol)           \
        THEOLIZER_INTERNAL_DEF_ARITHMETIC(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_FLOAT
    #define THEOLIZER_INTERNAL_DEF_FLOAT(dType, dSymbol)                    \
        THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_DOUBLE
    #define THEOLIZER_INTERNAL_DEF_DOUBLE(dType, dSymbol)                   \
        THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_LONG_DOUBLE
    #define THEOLIZER_INTERNAL_DEF_LONG_DOUBLE(dType, dSymbol)              \
        THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSymbol)
#endif

//----------------------------------------------------------------------------
//      文字列型
//----------------------------------------------------------------------------

// 文字列型全部
#ifndef THEOLIZER_INTERNAL_DEF_STRING
    #define THEOLIZER_INTERNAL_DEF_STRING(dType, dSymbol)                   \
        THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)
#endif

// Narrow文字列
#ifndef THEOLIZER_INTERNAL_DEF_NARROW_STRING
    #define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType, dSymbol)            \
        THEOLIZER_INTERNAL_DEF_STRING(dType, dSymbol)
#endif

// Wide文字列
#ifndef THEOLIZER_INTERNAL_DEF_WIDE_STRING
    #define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSymbol)              \
        THEOLIZER_INTERNAL_DEF_STRING(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_WSTRING
    #define THEOLIZER_INTERNAL_DEF_WSTRING(dType, dSymbol)                  \
        THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_CHAR16_STRING
    #define THEOLIZER_INTERNAL_DEF_CHAR16_STRING(dType, dSymbol)            \
        THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSymbol)
#endif

#ifndef THEOLIZER_INTERNAL_DEF_CHAR32_STRING
    #define THEOLIZER_INTERNAL_DEF_CHAR32_STRING(dType, dSymbol)            \
        THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSymbol)
#endif

// ***************************************************************************
//      実体化
// ***************************************************************************

THEOLIZER_INTERNAL_DEF_BOOL(bool,                             Bool)
THEOLIZER_INTERNAL_DEF_CHAR(char,                             Char)

THEOLIZER_INTERNAL_DEF_SIGNED_CHAR(signed char,               SignedChar)
THEOLIZER_INTERNAL_DEF_SIGNED_SHORT(short,                    SignedShort)
THEOLIZER_INTERNAL_DEF_SIGNED_INT(int,                        SignedInt)
THEOLIZER_INTERNAL_DEF_SIGNED_LONG(long,                      SignedLong)
THEOLIZER_INTERNAL_DEF_SIGNED_LONG_LONG(long long,            SignedLongLong)

THEOLIZER_INTERNAL_DEF_UNSIGNED_CHAR(unsigned char,           UnsignedChar)
THEOLIZER_INTERNAL_DEF_UNSIGNED_SHORT(unsigned short,         UnsignedShort)
THEOLIZER_INTERNAL_DEF_UNSIGNED_INT(unsigned int,             UnsignedInt)
THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG(unsigned long,           UnsignedLong)
THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG_LONG(unsigned long long, UnsignedLongLong)

// メタ・デシリアライザでC++型名へ変換する際、符号とサイズが合致するものを選択する。
// 下記は頻繁に使われるものではないのて優先順位を落とす。
#ifndef THEOLIZER_INTERNAL_NO_U16_U32
    THEOLIZER_INTERNAL_DEF_CHAR16(char16_t,                   Char16)
    THEOLIZER_INTERNAL_DEF_CHAR32(char32_t,                   Char32)
#endif
THEOLIZER_INTERNAL_DEF_WCHAR(wchar_t,                         WChar)

THEOLIZER_INTERNAL_DEF_FLOAT(float,                           Float)
THEOLIZER_INTERNAL_DEF_DOUBLE(double,                         Double)
THEOLIZER_INTERNAL_DEF_LONG_DOUBLE(long double,               LongDouble)

THEOLIZER_INTERNAL_DEF_NARROW_STRING(std::string,             String)
THEOLIZER_INTERNAL_DEF_WSTRING(std::wstring,                  Wstring)
THEOLIZER_INTERNAL_DEF_CHAR16_STRING(std::u16string,          U16string)
THEOLIZER_INTERNAL_DEF_CHAR32_STRING(std::u32string,          U32string)

// ***************************************************************************
//      マクロ定義の後始末
// ***************************************************************************

#undef  THEOLIZER_INTERNAL_DEF_PRIMITIVE

#undef  THEOLIZER_INTERNAL_DEF_ARITHMETIC

#undef  THEOLIZER_INTERNAL_DEF_INTEGRAL
#undef  THEOLIZER_INTERNAL_DEF_BOOL
#undef  THEOLIZER_INTERNAL_DEF_CHAR
#undef  THEOLIZER_INTERNAL_DEF_CHAR16
#undef  THEOLIZER_INTERNAL_DEF_CHAR32
#undef  THEOLIZER_INTERNAL_DEF_WCHAR

#undef  THEOLIZER_INTERNAL_DEF_SIGNED_INTEGRAL
#undef  THEOLIZER_INTERNAL_DEF_SIGNED_CHAR
#undef  THEOLIZER_INTERNAL_DEF_SIGNED_SHORT
#undef  THEOLIZER_INTERNAL_DEF_SIGNED_INT
#undef  THEOLIZER_INTERNAL_DEF_SIGNED_LONG
#undef  THEOLIZER_INTERNAL_DEF_SIGNED_LONG_LONG

#undef  THEOLIZER_INTERNAL_DEF_UNSIGNED_INTEGRAL
#undef  THEOLIZER_INTERNAL_DEF_UNSIGNED_CHAR
#undef  THEOLIZER_INTERNAL_DEF_UNSIGNED_SHORT
#undef  THEOLIZER_INTERNAL_DEF_UNSIGNED_INT
#undef  THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG
#undef  THEOLIZER_INTERNAL_DEF_UNSIGNED_LONG_LONG

#undef  THEOLIZER_INTERNAL_DEF_FLOATING_POINT
#undef  THEOLIZER_INTERNAL_DEF_FLOAT
#undef  THEOLIZER_INTERNAL_DEF_DOUBLE
#undef  THEOLIZER_INTERNAL_DEF_LONG_DOUBLE

#undef  THEOLIZER_INTERNAL_DEF_STRING
#undef  THEOLIZER_INTERNAL_DEF_NARROW_STRING
#undef  THEOLIZER_INTERNAL_DEF_WIDE_STRING
#undef  THEOLIZER_INTERNAL_DEF_WSTRING
#undef  THEOLIZER_INTERNAL_DEF_CHAR16_STRING
#undef  THEOLIZER_INTERNAL_DEF_CHAR32_STRING

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
