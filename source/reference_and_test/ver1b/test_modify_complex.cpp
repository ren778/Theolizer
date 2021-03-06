﻿//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の複合テスト
//              オブジェクト追跡
//              バージョン・アップ／ダウン処理（参照テスト含む）
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

#include "disable_test.h"
#ifdef ENABLE_MODIFY_COMPLEX_TEST

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable:4100)
#endif

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <string>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
#include <theolizer/serializer_json.h>
#include <theolizer/serializer_xml.h>
#include <theolizer/memory.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_modify_complex.h"

// 自動生成ソース
#include "test_modify_complex.cpp.theolizer.hpp"

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      型の不一致動作の確認
// ***************************************************************************

template<class tSerializerSave, class tSerializerLoad>
void testModifyComplex(std::string const& iFileName, theolizer::CheckMode iCheckMode)
{
    std::cout << "FileName=" << iFileName << "\n";

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {
        ChangeFromString    aChangeFromString;
        ChangeFromWString   aChangeFromWString;
        ChangeFromU16String aChangeFromU16String;
        ChangeFromU32String aChangeFromU32String;

        std::ifstream   aStream(iFileName);

        // 文字列をエンコードするシリアライザの場合
        if (tSerializerLoad::hasProperty(theolizer::Property::EncodedString))
        {   // 適切に回復可能
            tSerializerLoad aSerializer(aStream);

            THEOLIZER_PROCESS(aSerializer, aChangeFromString);
            THEOLIZER_PROCESS(aSerializer, aChangeFromWString);
            THEOLIZER_PROCESS(aSerializer, aChangeFromU16String);
            THEOLIZER_PROCESS(aSerializer, aChangeFromU32String);

            aChangeFromString.check();
            aChangeFromWString.check();
            aChangeFromU16String.check();
            aChangeFromU32String.check();
        }

        // 文字列をエンコードしない場合、適切に回復できないが、型チェックありなら例外
        else if (iCheckMode != theolizer::CheckMode::NoTypeCheck)
        {
            THEOLIZER_CHECK_EXCEPTION2(
                tSerializerLoad aSerializer(aStream);,                  // dStatements
                theolizer::ErrorInfo& e,                                // dException
                e.getErrorKind() == theolizer::ErrorKind::UnknownData,  // dJudge
                e.getMessage());                                        // dResult
        }

        // そうでないなら文字化けするのでチェックしない
        else
        {
        }
    }
}

//----------------------------------------------------------------------------
//      複数のシリアライザにて検証テスト呼び出し
//----------------------------------------------------------------------------

void tutoriseModifyComplex()
{
    std::cout << "tutoriseModifyComplex() start" << std::endl;

//      ---<<< Json >>>---

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::TypeCheck>,
        theolizer::JsonISerializer<theolizerD::TypeCheck>
    >("json-change-NoTypeCheck.json", theolizer::CheckMode::NoTypeCheck);

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::TypeCheck>,
        theolizer::JsonISerializer<theolizerD::TypeCheck>
    >("json-change-TypeCheck.json", theolizer::CheckMode::TypeCheck);

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::TypeCheck>,
        theolizer::JsonISerializer<theolizerD::TypeCheck>
    >("json-change-TypeCheckByIndex.json", theolizer::CheckMode::TypeCheckByIndex);

//      ---<<< Binary >>>---

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::TypeCheck>,
        theolizer::BinaryISerializer<theolizerD::TypeCheck>
    >("binary-change-NoTypeCheck.bin", theolizer::CheckMode::NoTypeCheck);

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::TypeCheck>,
        theolizer::BinaryISerializer<theolizerD::TypeCheck>
    >("binary-change-TypeCheck.bin", theolizer::CheckMode::TypeCheck);

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::TypeCheck>,
        theolizer::BinaryISerializer<theolizerD::TypeCheck>
    >("binary-change-TypeCheckByIndex.bin", theolizer::CheckMode::TypeCheckByIndex);

    std::cout << "tutoriseModifyComplex() end" << std::endl;
}

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveModifyComplex]
template<class tSerializer>
void saveModifyComplex(tSerializer& iSerializer)
{
    std::cout << "    saveModifyComplex();\n";

//----------------------------------------------------------------------------
//      オブジェクト追跡
//----------------------------------------------------------------------------

    {
        PointeeClass    aPointeeClass;
        PointeeInclude  aPointeeInclude(aPointeeClass, true);
        PointerInclude  aPointerInclude(aPointeeInclude);

        THEOLIZER_PROCESS(iSerializer, aPointeeClass);
        THEOLIZER_PROCESS(iSerializer, aPointeeInclude);
        THEOLIZER_PROCESS(iSerializer, aPointerInclude);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest(ClassKind::Kind1);
        KeepStepTest        aKeepStepTest(aVersionUpDownTest, true);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializer, aKeepStepTest);

        iSerializer.clearTracking();
    }
}
//! [saveModifyComplex]

INSTANTIATION_ALL(saveModifyComplex);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadModifyComplex]
template<class tSerializer>
void loadModifyComplex(tSerializer& iSerializer)
{
//theolizer::DisplayPass aDisplayPass;
    std::cout << "    loadModifyComplex();\n";

//----------------------------------------------------------------------------
//      オブジェクト追跡
//----------------------------------------------------------------------------

    {
//theolizer::DisplayPass aDisplayPass;
        PointeeClass    aPointeeClass;
        PointeeInclude  aPointeeInclude(aPointeeClass);
        PointerInclude  aPointerInclude;

        THEOLIZER_PROCESS(iSerializer, aPointeeClass);
        THEOLIZER_PROCESS(iSerializer, aPointeeInclude);
        THEOLIZER_PROCESS(iSerializer, aPointerInclude);

        aPointeeInclude.check();
        aPointerInclude.check(aPointeeInclude);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest;
        KeepStepTest        aKeepStepTest(aVersionUpDownTest);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializer, aKeepStepTest);
        aVersionUpDownTest.check(ClassKind::Kind1);
        aKeepStepTest.check(aVersionUpDownTest);

        iSerializer.clearTracking();
    }
}
//! [loadModifyComplex]

INSTANTIATION_ALL(loadModifyComplex);

//############################################################################
//      保存先のテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "saveDestinations()" << std::endl;

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest(ClassKind::Kind1);
        KeepStepTest        aKeepStepTest(aVersionUpDownTest, true);
        THEOLIZER_PROCESS_POINTEE(iSerializerA, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerA, aKeepStepTest);
        iSerializerA.clearTracking();

        THEOLIZER_PROCESS_POINTEE(iSerializerB, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerB, aKeepStepTest);
        iSerializerB.clearTracking();
    }
}

INSTANTIATION_DESTINATIONS(saveDestinations);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "loadDestinations()" << std::endl;

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest;
        KeepStepTest        aKeepStepTest(aVersionUpDownTest);
        THEOLIZER_PROCESS_POINTEE(iSerializerA, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerA, aKeepStepTest);
        iSerializerA.clearTracking();

        THEOLIZER_PROCESS_POINTEE(iSerializerB, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerB, aKeepStepTest);
        iSerializerB.clearTracking();

        aVersionUpDownTest.check(ClassKind::Kind1);
        aKeepStepTest.check(aVersionUpDownTest);
    }
}

INSTANTIATION_DESTINATIONS(loadDestinations);

#endif  // ENABLE_MODIFY_COMPLEX_TEST
