﻿//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          classの様々なバリエーション
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
#ifndef DISABLE_CLASS_VARIATION_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <ctime>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_class_variation.h"

// 自動生成ソース
#include "test_class_variation.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      クラス実体定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

void FullAutoTutorial::setNonPublic(int iFullAutoPrivate, int iFullAutoProtected)
{
    mFullAutoPrivate  =iFullAutoPrivate;
    mFullAutoProtected=iFullAutoProtected;
}

void FullAutoTutorial::checkNonPublic(int iFullAutoPrivate, int iFullAutoProtected)
{
    THEOLIZER_EQUAL(mFullAutoPrivate  , iFullAutoPrivate);
    THEOLIZER_EQUAL(mFullAutoProtected, iFullAutoProtected);
}

//----------------------------------------------------------------------------
//      侵入型半自動（名前対応）
//----------------------------------------------------------------------------

void HalfAutoNameTutorial::setNonPublic(int iHalfAutoNamePrivate, int iHalfAutoNameProtected)
{
    mHalfAutoNamePrivate  =iHalfAutoNamePrivate;
    mHalfAutoNameProtected=iHalfAutoNameProtected;
}

void HalfAutoNameTutorial::checkNonPublic(int iHalfAutoNamePrivate, int iHalfAutoNameProtected)
{
    THEOLIZER_EQUAL(mHalfAutoNamePrivate  , iHalfAutoNamePrivate);
    THEOLIZER_EQUAL(mHalfAutoNameProtected, iHalfAutoNameProtected);
}

//----------------------------------------------------------------------------
//      侵入型半自動（順序対応）
//----------------------------------------------------------------------------

void HalfAutoOrderTutorial::setNonPublic(int iHalfAutoOrderPrivate, int iHalfAutoOrderProtected)
{
    mHalfAutoOrderPrivate  =iHalfAutoOrderPrivate;
    mHalfAutoOrderProtected=iHalfAutoOrderProtected;
}

void HalfAutoOrderTutorial::checkNonPublic(int iHalfAutoOrderPrivate, int iHalfAutoOrderProtected)
{
    THEOLIZER_EQUAL(mHalfAutoOrderPrivate  , iHalfAutoOrderPrivate);
    THEOLIZER_EQUAL(mHalfAutoOrderProtected, iHalfAutoOrderProtected);
}

//----------------------------------------------------------------------------
//      派生／包含クラス
//----------------------------------------------------------------------------

void DerivedClass::setNonPublic
(
    int iHalfAutoNamePrivate,
    int iHalfAutoNameProtected,
    int iHalfAutoOrderPrivate,
    int iHalfAutoOrderProtected
)
{
    HalfAutoNameTutorial::setNonPublic(iHalfAutoNamePrivate, iHalfAutoNameProtected);
    mHalfAutoOrderTutorial.setNonPublic(iHalfAutoOrderPrivate, iHalfAutoOrderProtected);
}

void DerivedClass::checkNonPublic
(
    int iHalfAutoNamePrivate,
    int iHalfAutoNameProtected,
    int iHalfAutoOrderPrivate,
    int iHalfAutoOrderProtected
)
{
    HalfAutoNameTutorial::checkNonPublic(iHalfAutoNamePrivate, iHalfAutoNameProtected);
    mHalfAutoOrderTutorial.checkNonPublic(iHalfAutoOrderPrivate, iHalfAutoOrderProtected);
}

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseClassVariation()
{
    std::cout << "tutoriseClassVariation() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {
        std::ofstream   aStream("tutorise_class_variation.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // 非侵入型完全自動
        FullAutoTutorial    aFullAutoTutorial;
        aFullAutoTutorial.setNonPublic(123, 456);
        aFullAutoTutorial.mFullAutoPublic=789;
        THEOLIZER_PROCESS(aSerializer, aFullAutoTutorial);

        // 侵入型半自動（名前対応）
        HalfAutoNameTutorial    aHalfAutoNameTutorial;
        aHalfAutoNameTutorial.setNonPublic(-123, -456);
        aHalfAutoNameTutorial.mHalfAutoNamePublic=-789;
        THEOLIZER_PROCESS(aSerializer, aHalfAutoNameTutorial);

        // 侵入型半自動（順序対応）
        HalfAutoOrderTutorial   aHalfAutoOrderTutorial;
        aHalfAutoOrderTutorial.setNonPublic(-123, -456);
        aHalfAutoOrderTutorial.mHalfAutoOrderPublic=-789;
        THEOLIZER_PROCESS(aSerializer, aHalfAutoOrderTutorial);

        // 非侵入型手動
        ManualTutorial  aManualTutorial;
        aManualTutorial.mManualPublic=100;
        THEOLIZER_PROCESS(aSerializer, aManualTutorial);

        // 派生／包含クラス
        DerivedClass    aDerivedClass;
        aDerivedClass.FullAutoTutorial::setNonPublic(200, 201);
        aDerivedClass.mFullAutoPublic=202;
        aDerivedClass.setNonPublic(203, 204, 205, 206);
        aDerivedClass.mManualTutorial.mManualPublic=207;
        THEOLIZER_PROCESS(aSerializer, aDerivedClass);
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {
        std::ifstream   aStream("tutorise_class_variation.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // 非侵入型完全自動
        FullAutoTutorial    aFullAutoTutorial;
        THEOLIZER_PROCESS(aSerializer, aFullAutoTutorial);
        aFullAutoTutorial.checkNonPublic(0, 456);       // 完全自動のprivateはシリアライズされない
        THEOLIZER_EQUAL(aFullAutoTutorial.mFullAutoPublic, 789);

        // 侵入型半自動（名前対応）
        HalfAutoNameTutorial    aHalfAutoNameTutorial;
        THEOLIZER_PROCESS(aSerializer, aHalfAutoNameTutorial);
        aHalfAutoNameTutorial.checkNonPublic(-123, -456);   // 侵入型のprivateはシリアライズされる
        THEOLIZER_EQUAL(aHalfAutoNameTutorial.mHalfAutoNamePublic, -789);

        // 侵入型半自動（順序対応）
        HalfAutoOrderTutorial   aHalfAutoOrderTutorial;
        THEOLIZER_PROCESS(aSerializer, aHalfAutoOrderTutorial);
        aHalfAutoOrderTutorial.checkNonPublic(-123, -456);  // 侵入型のprivateはシリアライズされる
        THEOLIZER_EQUAL(aHalfAutoOrderTutorial.mHalfAutoOrderPublic, -789);

        // 非侵入型手動
        ManualTutorial  aManualTutorial;
        THEOLIZER_PROCESS(aSerializer, aManualTutorial);
        THEOLIZER_EQUAL(aManualTutorial.mManualPublic, 100);

        // 派生／包含クラス
        DerivedClass    aDerivedClass;
        THEOLIZER_PROCESS(aSerializer, aDerivedClass);
        // 基底クラスFullAutoTutorialのcheckNonPublic()関数呼び出し
        aDerivedClass.FullAutoTutorial::checkNonPublic(0, 201);
        THEOLIZER_EQUAL(aDerivedClass.mFullAutoPublic, 202);
        aDerivedClass.checkNonPublic(0, 0, 0, 0);
        THEOLIZER_EQUAL(aDerivedClass.mManualTutorial.mManualPublic, 207);
    }

    std::cout << "tutoriseClassVariation() end\n" << std::endl;
}

//############################################################################
//      プリミティブ、enum型、クラス、配列
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveClassVariation]
template<class tSerializer>
void saveClassVariation(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      単独テスト
//----------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

#ifndef DISABLE_SINGLE_TEST

    {
        FullAuto    aFullAuto{true};
        std::cout << "        saveClassVariation() : aFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aFullAuto);
    }

//      ---<<< 侵入型半自動－名前対応 >>>---

    {
        HalfAutoName    aHalfAutoName{true};
        std::cout << "        saveClassVariation() : aHalfAutoName" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoName);
    }

//      ---<<< 侵入型半自動－順序対応 >>>---

    {
        HalfAutoOrder   aHalfAutoOrder{true};
        std::cout << "        saveClassVariation() : aHalfAutoOrder" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoOrder);
    }

//      ---<<< 非侵入型手動 >>>---

    {
        Manual    aManual{true};
        std::cout << "        saveClassVariation() : aManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManual);
    }

#endif  // DISABLE_SINGLE_TEST

//----------------------------------------------------------------------------
//      ２重組み合わせテスト
//----------------------------------------------------------------------------

#ifndef DISABLE_COMBINATION_TEST

//      ---<<< 非侵入型完全自動 >>>---

    {
        DerivedFullAuto aDerivedFullAuto{true};
        std::cout << "        saveClassVariation() : aDerivedFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedFullAuto);
    }

//      ---<<< 侵入型半自動 >>>---

    {
        DerivedHalfAuto aDerivedHalfAuto{true};
        std::cout << "        saveClassVariation() : aDerivedHalfAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedHalfAuto);
    }

//      ---<<< 非侵入型手動 >>>---

    {
        DerivedManual   aDerivedManual{true};
        std::cout << "        saveClassVariation() : aDerivedManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedManual);
    }

//----------------------------------------------------------------------------
//      ３重組み合わせテスト
//----------------------------------------------------------------------------

#ifndef DISABLE_TRIPLED_TEST

//      ---<<< 非侵入型完全自動 >>>---

    {
        TripledFullAuto aTripledFullAuto{true};
        std::cout << "        saveClassVariation() : aTripledFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aTripledFullAuto);
    }

//      ---<<< 侵入型半自動 >>>---

    {
        TripledHalfAuto aTripledHalfAuto{true};
        std::cout << "        saveClassVariation() : aTripledHalfAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aTripledHalfAuto);
    }

//      ---<<< 非侵入型手動 >>>---

    {
        TripledManual   aTripledManual{true};
        std::cout << "        saveClassVariation() : aTripledManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aTripledManual);
    }

#endif  // DISABLE_TRIPLED_TEST
#endif  // DISABLE_COMBINATION_TEST
}
//! [saveClassVariation]

INSTANTIATION_ALL(void, saveClassVariation);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadClassVariation]
template<class tSerializer>
void loadClassVariation(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      単独テスト
//----------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

#ifndef DISABLE_SINGLE_TEST

    {
        FullAuto    aFullAuto{};
        std::cout << "        loadClassVariation() : aFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aFullAuto);
        aFullAuto.checkPrivate();
        aFullAuto.checkProtected(true);
        aFullAuto.checkPublic(true);
    }

//      ---<<< 侵入型半自動－名前対応 >>>---

    {
        HalfAutoName    aHalfAutoName{};
        std::cout << "        loadClassVariation() : aHalfAutoName" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoName);
        aHalfAutoName.checkPrivate(true);
        aHalfAutoName.checkProtected(true);
        aHalfAutoName.checkPublic(true);
    }

//      ---<<< 侵入型半自動－順序対応 >>>---

    {
        HalfAutoOrder   aHalfAutoOrder{true};
        std::cout << "        loadClassVariation() : aHalfAutoOrder" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoOrder);
        aHalfAutoOrder.checkPrivate(true);
        aHalfAutoOrder.checkProtected(true);
        aHalfAutoOrder.checkPublic(true);
    }

//      ---<<< 非侵入型手動 >>>---

    {
        Manual      aManual{};
        std::cout << "        loadClassVariation() : aManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManual);
        aManual.checkPublic(true);
    }

#endif  // DISABLE_SINGLE_TEST

//----------------------------------------------------------------------------
//      ２重組み合わせテスト
//----------------------------------------------------------------------------

#ifndef DISABLE_COMBINATION_TEST

//      ---<<< 非侵入型完全自動 >>>---

    {
        DerivedFullAuto aDerivedFullAuto{};
        std::cout << "        loadClassVariation() : aDerivedFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedFullAuto);
        aDerivedFullAuto.check();
    }

//      ---<<< 侵入型半自動 >>>---

    {
        DerivedHalfAuto aDerivedHalfAuto{};
        std::cout << "        loadClassVariation() : aDerivedHalfAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedHalfAuto);
        aDerivedHalfAuto.check();
    }

//      ---<<< 非侵入型手動 >>>---

    {
        DerivedManual   aDerivedManual{};
        std::cout << "        loadClassVariation() : aDerivedManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedManual);
        aDerivedManual.check();
    }

//----------------------------------------------------------------------------
//      ３重組み合わせテスト
//----------------------------------------------------------------------------

#ifndef DISABLE_TRIPLED_TEST

//      ---<<< 非侵入型完全自動 >>>---

    {
        TripledFullAuto aTripledFullAuto{};
        std::cout << "        loadClassVariation() : aTripledFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aTripledFullAuto);
        aTripledFullAuto.check();
    }

//      ---<<< 侵入型半自動 >>>---

    {
        TripledHalfAuto aTripledHalfAuto{};
        std::cout << "        loadClassVariation() : aTripledHalfAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aTripledHalfAuto);
        aTripledHalfAuto.check();
    }

//      ---<<< 非侵入型手動 >>>---

    {
        TripledManual   aTripledManual{};
        std::cout << "        loadClassVariation() : aTripledManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aTripledManual);
        aTripledManual.check();
    }

#endif  // DISABLE_TRIPLED_TEST
#endif  // DISABLE_COMBINATION_TEST
}
//! [loadClassVariation]

INSTANTIATION_ALL(void, loadClassVariation);

#endif  // DISABLE_CLASS_VARIATION_TEST
