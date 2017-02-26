﻿//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          enum型変更の使い方
/*
    Copyright (c) 2016 Yohinori Tahara(Theoride Technology) - http://theolizer.com/

    商用ライセンス
        有効なTheolizer商用ライセンスを保持している人は、
        田原良則(Theoride Technology)と締結している商用ライセンス契約の
        使用条件に従って、このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        GPLv3のコピーをLICENSE.TXTファイルにおいてます。
*/
//############################################################################

#include "disable_test.h"
#ifndef DISABLE_MODIFY_ENUM_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <string>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_modify_enum.h"

// 自動生成ソース
#include "test_modify_enum.cpp.theolizer.hpp"

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveModifyEnum]
template<class tSerializer>
void saveModifyEnum(tSerializer& iSerializer)
{
    std::cout << "    saveModifyEnum();\n";

//      ---<<< enum型(完全自動型) >>>---

    {
        EnumFullAuto    aEnumFullAuto0(eefaValue1);
        EnumFullAuto    aEnumFullAuto1(eefaName11);
        EnumFullAuto    aEnumFullAuto2(eefaName20);
        EnumFullAuto    aEnumFullAuto3(eefaName21);
        EnumFullAuto    aEnumFullAuto4(eefaName22);
        EnumFullAuto    aEnumFullAuto5(eefaName31);
        EnumFullAuto    aEnumFullAuto6(eefaName32);
        EnumFullAuto    aEnumFullAuto7(eefaName33);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto0);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto1);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto2);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto3);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto4);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto5);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto6);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto7);

        ScopedEnumFullAuto  aScopedEnumFullAuto0(ScopedEnumFullAuto::Value1);
        ScopedEnumFullAuto  aScopedEnumFullAuto1(ScopedEnumFullAuto::Name11);
        ScopedEnumFullAuto  aScopedEnumFullAuto2(ScopedEnumFullAuto::Name20);
        ScopedEnumFullAuto  aScopedEnumFullAuto3(ScopedEnumFullAuto::Name21);
        ScopedEnumFullAuto  aScopedEnumFullAuto4(ScopedEnumFullAuto::Name22);
        ScopedEnumFullAuto  aScopedEnumFullAuto5(ScopedEnumFullAuto::Name31);
        ScopedEnumFullAuto  aScopedEnumFullAuto6(ScopedEnumFullAuto::Name32);
        ScopedEnumFullAuto  aScopedEnumFullAuto7(ScopedEnumFullAuto::Name33);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto0);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto1);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto2);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto3);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto4);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto5);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto6);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto7);
    }

//      ---<<< enum型(半自動型－シンボル名対応) >>>---

    {
        EnumSymName aEnumSymName0(eesnValue1);
        EnumSymName aEnumSymName1(eesnName11);
        EnumSymName aEnumSymName2(eesnName20);
        EnumSymName aEnumSymName3(eesnName21);
        EnumSymName aEnumSymName4(eesnName22);
        EnumSymName aEnumSymName5(eesnName31);
        EnumSymName aEnumSymName6(eesnName32);
        EnumSymName aEnumSymName7(eesnName33);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName0);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName1);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName2);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName3);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName4);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName5);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName6);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName7);

        ScopedEnumSymName   aScopedEnumSymName0(ScopedEnumSymName::Value1);
        ScopedEnumSymName   aScopedEnumSymName1(ScopedEnumSymName::Name11);
        ScopedEnumSymName   aScopedEnumSymName2(ScopedEnumSymName::Name20);
        ScopedEnumSymName   aScopedEnumSymName3(ScopedEnumSymName::Name21);
        ScopedEnumSymName   aScopedEnumSymName4(ScopedEnumSymName::Name22);
        ScopedEnumSymName   aScopedEnumSymName5(ScopedEnumSymName::Name31);
        ScopedEnumSymName   aScopedEnumSymName6(ScopedEnumSymName::Name32);
        ScopedEnumSymName   aScopedEnumSymName7(ScopedEnumSymName::Name33);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName0);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName1);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName2);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName3);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName4);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName5);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName6);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName7);
    }

//      ---<<< enum型(半自動型－シンボル値対応) >>>---

    {
        EnumSymVal  aEnumSymVal0(eesvName1);
        EnumSymVal  aEnumSymVal1(eesvValue11);
        EnumSymVal  aEnumSymVal2(eesvValue20);
        EnumSymVal  aEnumSymVal3(eesvValue21);
        EnumSymVal  aEnumSymVal4(eesvValue22);
        EnumSymVal  aEnumSymVal5(eesvValue31);
        EnumSymVal  aEnumSymVal6(eesvValue32);
        EnumSymVal  aEnumSymVal7(eesvValue33);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal0);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal1);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal2);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal3);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal4);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal5);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal6);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal7);

        ScopedEnumSymVal    aScopedEnumSymVal0(ScopedEnumSymVal::Name1);
        ScopedEnumSymVal    aScopedEnumSymVal1(ScopedEnumSymVal::Value11);
        ScopedEnumSymVal    aScopedEnumSymVal2(ScopedEnumSymVal::Value20);
        ScopedEnumSymVal    aScopedEnumSymVal3(ScopedEnumSymVal::Value21);
        ScopedEnumSymVal    aScopedEnumSymVal4(ScopedEnumSymVal::Value22);
        ScopedEnumSymVal    aScopedEnumSymVal5(ScopedEnumSymVal::Value31);
        ScopedEnumSymVal    aScopedEnumSymVal6(ScopedEnumSymVal::Value32);
        ScopedEnumSymVal    aScopedEnumSymVal7(ScopedEnumSymVal::Value33);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal0);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal1);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal2);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal3);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal4);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal5);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal6);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal7);
    }
}
//! [saveModifyEnum]

INSTANTIATION_ALL(saveModifyEnum);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadModifyEnum]
template<class tSerializer>
void loadModifyEnum(tSerializer& iSerializer)
{
theolizer::DisplayPass aDisplayPass;
    std::cout << "    loadModifyEnum();\n";

//      ---<<< enum型(完全自動型) >>>---

    {
        EnumFullAuto    aEnumFullAuto0;
        EnumFullAuto    aEnumFullAuto1;
        EnumFullAuto    aEnumFullAuto2;
        EnumFullAuto    aEnumFullAuto3;
        EnumFullAuto    aEnumFullAuto4;
        EnumFullAuto    aEnumFullAuto5;
        EnumFullAuto    aEnumFullAuto6;
        EnumFullAuto    aEnumFullAuto7;
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto0);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto1);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto2);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto3);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto4);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto5);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto6);
        THEOLIZER_PROCESS(iSerializer, aEnumFullAuto7);

        // 自分自身での保存なら値の統合無し
        if (gMyIndex == gProgramIndex)
        {
            THEOLIZER_EQUAL(aEnumFullAuto0, eefaValue1);
            THEOLIZER_EQUAL(aEnumFullAuto1, eefaName11);
            THEOLIZER_EQUAL(aEnumFullAuto2, eefaName20);
            THEOLIZER_EQUAL(aEnumFullAuto3, eefaName21);
            THEOLIZER_EQUAL(aEnumFullAuto4, eefaName22);
            THEOLIZER_EQUAL(aEnumFullAuto5, eefaName31);
            THEOLIZER_EQUAL(aEnumFullAuto6, eefaName32);
            THEOLIZER_EQUAL(aEnumFullAuto7, eefaName33);
        }
        // ver1bがあるので自分より新しいバージョンはサポートできない
        else if (gMyIndex < gProgramIndex)
        {
            std::cout << "Not support newer version.\n";
            exit(1);
        }

        ScopedEnumFullAuto  aScopedEnumFullAuto0;
        ScopedEnumFullAuto  aScopedEnumFullAuto1;
        ScopedEnumFullAuto  aScopedEnumFullAuto2;
        ScopedEnumFullAuto  aScopedEnumFullAuto3;
        ScopedEnumFullAuto  aScopedEnumFullAuto4;
        ScopedEnumFullAuto  aScopedEnumFullAuto5;
        ScopedEnumFullAuto  aScopedEnumFullAuto6;
        ScopedEnumFullAuto  aScopedEnumFullAuto7;
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto0);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto1);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto2);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto3);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto4);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto5);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto6);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumFullAuto7);

        // 自分自身での保存なら値の統合無し
        if (gMyIndex == gProgramIndex)
        {
            THEOLIZER_EQUAL(aScopedEnumFullAuto0, ScopedEnumFullAuto::Value1);
            THEOLIZER_EQUAL(aScopedEnumFullAuto1, ScopedEnumFullAuto::Name11);
            THEOLIZER_EQUAL(aScopedEnumFullAuto2, ScopedEnumFullAuto::Name20);
            THEOLIZER_EQUAL(aScopedEnumFullAuto3, ScopedEnumFullAuto::Name21);
            THEOLIZER_EQUAL(aScopedEnumFullAuto4, ScopedEnumFullAuto::Name22);
            THEOLIZER_EQUAL(aScopedEnumFullAuto5, ScopedEnumFullAuto::Name31);
            THEOLIZER_EQUAL(aScopedEnumFullAuto6, ScopedEnumFullAuto::Name32);
            THEOLIZER_EQUAL(aScopedEnumFullAuto7, ScopedEnumFullAuto::Name33);
        }
        // ver1bがあるので自分より新しいバージョンはサポートできない
        else if (gMyIndex < gProgramIndex)
        {
            std::cout << "Not support newer version.\n";
            exit(1);
        }
    }

//      ---<<< enum型(半自動型－シンボル名対応) >>>---

    {
        EnumSymName aEnumSymName0;
        EnumSymName aEnumSymName1;
        EnumSymName aEnumSymName2;
        EnumSymName aEnumSymName3;
        EnumSymName aEnumSymName4;
        EnumSymName aEnumSymName5;
        EnumSymName aEnumSymName6;
        EnumSymName aEnumSymName7;
        THEOLIZER_PROCESS(iSerializer, aEnumSymName0);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName1);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName2);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName3);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName4);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName5);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName6);
        THEOLIZER_PROCESS(iSerializer, aEnumSymName7);

        // 自分自身での保存なら値の統合無し
        if (gMyIndex == gProgramIndex)
        {
            THEOLIZER_EQUAL(aEnumSymName0, eesnValue1);
            THEOLIZER_EQUAL(aEnumSymName1, eesnName11);
            THEOLIZER_EQUAL(aEnumSymName2, eesnName20);
            THEOLIZER_EQUAL(aEnumSymName3, eesnName21);
            THEOLIZER_EQUAL(aEnumSymName4, eesnName22);
            THEOLIZER_EQUAL(aEnumSymName5, eesnName31);
            THEOLIZER_EQUAL(aEnumSymName6, eesnName32);
            THEOLIZER_EQUAL(aEnumSymName7, eesnName33);
        }
        // ver1bがあるので自分より新しいバージョンはサポートできない
        else if (gMyIndex < gProgramIndex)
        {
            std::cout << "Not support newer version.\n";
            exit(1);
        }

        ScopedEnumSymName   aScopedEnumSymName0;
        ScopedEnumSymName   aScopedEnumSymName1;
        ScopedEnumSymName   aScopedEnumSymName2;
        ScopedEnumSymName   aScopedEnumSymName3;
        ScopedEnumSymName   aScopedEnumSymName4;
        ScopedEnumSymName   aScopedEnumSymName5;
        ScopedEnumSymName   aScopedEnumSymName6;
        ScopedEnumSymName   aScopedEnumSymName7;
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName0);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName1);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName2);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName3);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName4);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName5);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName6);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName7);

        // 自分自身での保存なら値の統合無し
        if (gMyIndex == gProgramIndex)
        {
            THEOLIZER_EQUAL(aScopedEnumSymName0, ScopedEnumSymName::Value1);
            THEOLIZER_EQUAL(aScopedEnumSymName1, ScopedEnumSymName::Name11);
            THEOLIZER_EQUAL(aScopedEnumSymName2, ScopedEnumSymName::Name20);
            THEOLIZER_EQUAL(aScopedEnumSymName3, ScopedEnumSymName::Name21);
            THEOLIZER_EQUAL(aScopedEnumSymName4, ScopedEnumSymName::Name22);
            THEOLIZER_EQUAL(aScopedEnumSymName5, ScopedEnumSymName::Name31);
            THEOLIZER_EQUAL(aScopedEnumSymName6, ScopedEnumSymName::Name32);
            THEOLIZER_EQUAL(aScopedEnumSymName7, ScopedEnumSymName::Name33);
        }
        // ver1bがあるので自分より新しいバージョンはサポートできない
        else if (gMyIndex < gProgramIndex)
        {
            std::cout << "Not support newer version.\n";
            exit(1);
        }
    }

//      ---<<< enum型(半自動型－シンボル値対応) >>>---

    {
        EnumSymVal  aEnumSymVal0;
        EnumSymVal  aEnumSymVal1;
        EnumSymVal  aEnumSymVal2;
        EnumSymVal  aEnumSymVal3;
        EnumSymVal  aEnumSymVal4;
        EnumSymVal  aEnumSymVal5;
        EnumSymVal  aEnumSymVal6;
        EnumSymVal  aEnumSymVal7;
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal0);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal1);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal2);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal3);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal4);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal5);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal6);
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal7);

        // 自分自身での保存なら値の統合無し
        if (gMyIndex == gProgramIndex)
        {
            THEOLIZER_EQUAL(aEnumSymVal0, eesvName1);
            THEOLIZER_EQUAL(aEnumSymVal1, eesvValue11);
            THEOLIZER_EQUAL(aEnumSymVal2, eesvValue20);
            THEOLIZER_EQUAL(aEnumSymVal3, eesvValue21);
            THEOLIZER_EQUAL(aEnumSymVal4, eesvValue22);
            THEOLIZER_EQUAL(aEnumSymVal5, eesvValue31);
            THEOLIZER_EQUAL(aEnumSymVal6, eesvValue32);
            THEOLIZER_EQUAL(aEnumSymVal7, eesvValue33);
        }
        // ver1bがあるので自分より新しいバージョンはサポートできない
        else if (gMyIndex < gProgramIndex)
        {
            std::cout << "Not support newer version.\n";
            exit(1);
        }

        ScopedEnumSymVal    aScopedEnumSymVal0;
        ScopedEnumSymVal    aScopedEnumSymVal1;
        ScopedEnumSymVal    aScopedEnumSymVal2;
        ScopedEnumSymVal    aScopedEnumSymVal3;
        ScopedEnumSymVal    aScopedEnumSymVal4;
        ScopedEnumSymVal    aScopedEnumSymVal5;
        ScopedEnumSymVal    aScopedEnumSymVal6;
        ScopedEnumSymVal    aScopedEnumSymVal7;
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal0);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal1);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal2);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal3);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal4);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal5);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal6);
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymVal7);

        // 自分自身での保存なら値の統合無し
        if (gMyIndex == gProgramIndex)
        {
            THEOLIZER_EQUAL(aScopedEnumSymVal0, ScopedEnumSymVal::Name1);
            THEOLIZER_EQUAL(aScopedEnumSymVal1, ScopedEnumSymVal::Value11);
            THEOLIZER_EQUAL(aScopedEnumSymVal2, ScopedEnumSymVal::Value20);
            THEOLIZER_EQUAL(aScopedEnumSymVal3, ScopedEnumSymVal::Value21);
            THEOLIZER_EQUAL(aScopedEnumSymVal4, ScopedEnumSymVal::Value22);
            THEOLIZER_EQUAL(aScopedEnumSymVal5, ScopedEnumSymVal::Value31);
            THEOLIZER_EQUAL(aScopedEnumSymVal6, ScopedEnumSymVal::Value32);
            THEOLIZER_EQUAL(aScopedEnumSymVal7, ScopedEnumSymVal::Value33);
        }
        // ver1bがあるので自分より新しいバージョンはサポートできない
        else if (gMyIndex < gProgramIndex)
        {
            std::cout << "Not support newer version.\n";
            exit(1);
        }
    }
}
//! [loadModifyEnum]

INSTANTIATION_ALL(loadModifyEnum);

#endif  // DISABLE_MODIFY_ENUM_TEST