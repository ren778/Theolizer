﻿//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          STLサポート
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
#ifdef ENABLE_SUPPORT_STL_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <ctime>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
#include <theolizer/serializer_json.h>
#include <theolizer/serializer_xml.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_support_stl.h"

// 自動生成ソース
#include "test_support_stl.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseSupportStl()
{
    std::cout << "tutoriseSupportStl() start" << std::endl;

//----------------------------------------------------------------------------
//      スマート・ポインタ
//----------------------------------------------------------------------------

//![SmartPtr]

//      ---<<< 保存 >>>---

    {
        // 保存データ生成
        std::unique_ptr<int>    aUnique{new int{100}};
        std::shared_ptr<int>    aShared0{new int{200}};
        std::shared_ptr<int>    aShared1{aShared0};
        std::shared_ptr<int>    aSharedForWeak{new int{300}};
        std::weak_ptr<int>      aWeak{aSharedForWeak};

        // 保存
        std::ofstream   aStream("tutorise_support_stl.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS(aSerializer, aUnique);
        THEOLIZER_PROCESS(aSerializer, aShared0);
        THEOLIZER_PROCESS(aSerializer, aShared1);
        THEOLIZER_PROCESS(aSerializer, aSharedForWeak);
        THEOLIZER_PROCESS(aSerializer, aWeak);

        aSerializer.clearTracking();
    }

//      ---<<< 回復 >>>---

    {
        // 回復先データ生成
        std::unique_ptr<int>    aUnique{};
        std::shared_ptr<int>    aShared0{};
        std::shared_ptr<int>    aShared1{};
        std::shared_ptr<int>    aSharedForWeak{};
        std::weak_ptr<int>      aWeak{};

        // 回復
        std::ifstream   aStream("tutorise_support_stl.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS(aSerializer, aUnique);
        THEOLIZER_PROCESS(aSerializer, aShared0);
        THEOLIZER_PROCESS(aSerializer, aShared1);
        THEOLIZER_PROCESS(aSerializer, aSharedForWeak);
        THEOLIZER_PROCESS(aSerializer, aWeak);

        // チェック
        THEOLIZER_EQUAL(*(aUnique.get()), 100);
        THEOLIZER_EQUAL(*(aShared0.get()), 200);
        THEOLIZER_EQUAL_PTR(aShared1.get(), aShared0.get());
        THEOLIZER_EQUAL(*(aSharedForWeak.get()), 300);
        THEOLIZER_EQUAL_PTR(aWeak.lock().get(), aSharedForWeak.get());

        aSerializer.clearTracking();
    }

//![SmartPtr]

//----------------------------------------------------------------------------
//      標準コンテナの通常使用
//----------------------------------------------------------------------------

//![ContaierNomal]

//      ---<<< 保存 >>>---

    {
        // 保存データ生成
        std::list<int>  aList;
        aList.emplace_back(100);
        aList.emplace_back(200);
        aList.emplace_back(300);

        // 保存
        std::ofstream   aStream("tutorise_support_stl_0.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS(aSerializer, aList);
    }

//      ---<<< 回復 >>>---

    {
        // 回復先データ生成
        std::list<int>  aList;

        // 回復
        std::ifstream   aStream("tutorise_support_stl_0.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS(aSerializer, aList);

        // チェック
        auto itr=aList.begin();
        THEOLIZER_EQUAL(*itr, 100);     ++itr;
        THEOLIZER_EQUAL(*itr, 200);     ++itr;
        THEOLIZER_EQUAL(*itr, 300);     ++itr;
    }

//![ContaierNomal]

//----------------------------------------------------------------------------
//      標準コンテナの被ポインタ使用
//----------------------------------------------------------------------------

//![ContaierPointee]

//      ---<<< 保存 >>>---

    {
        // 保存データ生成
        theolizer::VectorPointee<int>   aVectorPointee;
        aVectorPointee.emplace_back(400);
        aVectorPointee.emplace_back(500);
        aVectorPointee.emplace_back(600);

        auto itr=aVectorPointee.begin();
        int*    aPtr0=&*itr++;
        int*    aPtr1=&*itr++;
        int*    aPtr2=&*itr++;

        // 保存
        std::ofstream   aStream("tutorise_support_stl_1.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS(aSerializer, aVectorPointee);
        THEOLIZER_PROCESS(aSerializer, aPtr0);
        THEOLIZER_PROCESS(aSerializer, aPtr1);
        THEOLIZER_PROCESS(aSerializer, aPtr2);

        aSerializer.clearTracking();
    }

//      ---<<< 回復 >>>---

    {
        // 回復先データ生成
        theolizer::VectorPointee<int>   aVectorPointee;
        int*    aPtr0=nullptr;
        int*    aPtr1=nullptr;
        int*    aPtr2=nullptr;

        // 回復
        std::ifstream   aStream("tutorise_support_stl_1.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS(aSerializer, aVectorPointee);
        THEOLIZER_PROCESS(aSerializer, aPtr0);
        THEOLIZER_PROCESS(aSerializer, aPtr1);
        THEOLIZER_PROCESS(aSerializer, aPtr2);

        // チェック
        auto itr=aVectorPointee.begin();
        THEOLIZER_EQUAL_PTR(&*itr, aPtr0);
        THEOLIZER_EQUAL(*itr, 400);     ++itr;
        THEOLIZER_EQUAL_PTR(&*itr, aPtr1);
        THEOLIZER_EQUAL(*itr, 500);     ++itr;
        THEOLIZER_EQUAL_PTR(&*itr, aPtr2);
        THEOLIZER_EQUAL(*itr, 600);     ++itr;

        aSerializer.clearTracking();
    }

//![ContaierPointee]

//----------------------------------------------------------------------------
//      標準コンテナの保存先指定による分割保存と回復
//----------------------------------------------------------------------------

//![ContaierDestinations]

//      ---<<< 保存 >>>---

    {
        // 保存データ生成
        std::map<std::string, StlTutorial>  aMap;
        aMap.emplace("first",  StlTutorial(100, 200));
        aMap.emplace("second", StlTutorial(101, 201));
        aMap.emplace("third",  StlTutorial(102, 202));

        // 保存
        std::ofstream   aStreamA("tutorise_support_stl_a.json");
        theolizer::JsonOSerializer<theolizerD::DestA> aSerializerA(aStreamA);
        std::ofstream   aStreamB("tutorise_support_stl_b.json");
        theolizer::JsonOSerializer<theolizerD::DestB> aSerializerB(aStreamB);

        THEOLIZER_PROCESS(aSerializerA, aMap);
        THEOLIZER_PROCESS(aSerializerB, aMap);

        aSerializerA.clearTracking();
        aSerializerB.clearTracking();
    }

//      ---<<< 回復 >>>---

    {
        // 回復先データ生成
        std::map<std::string, StlTutorial>  aMap;

        // 回復準備
        std::ifstream   aStreamA("tutorise_support_stl_a.json");
        theolizer::JsonISerializer<> aSerializerA(aStreamA);
        std::ifstream   aStreamB("tutorise_support_stl_b.json");
        theolizer::JsonISerializer<> aSerializerB(aStreamB);

        // 回復(DestA)
        THEOLIZER_PROCESS(aSerializerA, aMap);

        // チェックA(DestBは回復されていない)
        auto itr=aMap.find("first");
        THEOLIZER_CHECK(itr != aMap.end(), "first");
        THEOLIZER_EQUAL(itr->second.mDataA, 100);
        THEOLIZER_EQUAL(itr->second.mDataB, 0);

        itr=aMap.find("second");
        THEOLIZER_CHECK(itr != aMap.end(), "second");
        THEOLIZER_EQUAL(itr->second.mDataA, 101);
        THEOLIZER_EQUAL(itr->second.mDataB, 0);

        itr=aMap.find("third");
        THEOLIZER_CHECK(itr != aMap.end(), "third");
        THEOLIZER_EQUAL(itr->second.mDataA, 102);
        THEOLIZER_EQUAL(itr->second.mDataB, 0);

        // 回復(DestB)
        THEOLIZER_PROCESS(aSerializerB, aMap);

        // チェックB
        itr=aMap.find("first");
        THEOLIZER_CHECK(itr != aMap.end(), "first");
        THEOLIZER_EQUAL(itr->second.mDataA, 100);
        THEOLIZER_EQUAL(itr->second.mDataB, 200);

        itr=aMap.find("second");
        THEOLIZER_CHECK(itr != aMap.end(), "second");
        THEOLIZER_EQUAL(itr->second.mDataA, 101);
        THEOLIZER_EQUAL(itr->second.mDataB, 201);

        itr=aMap.find("third");
        THEOLIZER_CHECK(itr != aMap.end(), "third");
        THEOLIZER_EQUAL(itr->second.mDataA, 102);
        THEOLIZER_EQUAL(itr->second.mDataB, 202);

        aSerializerA.clearTracking();
        aSerializerB.clearTracking();
    }

//![ContaierDestinations]

    std::cout << "tutoriseSupportStl() end\n" << std::endl;
}

//############################################################################
//      網羅テスト
//############################################################################

//----------------------------------------------------------------------------
//      キー無しコンテナ設定用
//----------------------------------------------------------------------------

// プライマリ
template<class tContainer, typename tType, class tEnable=void>
struct Emplace
{
    void operator()(tContainer& iContainer, tType const& iFirst, std::size_t iCount)
    {
        for (std::size_t i=0; i < iCount; ++i)
        {
            iContainer.emplace(iContainer.end(), iFirst+static_cast<int>(i));
        }
    }
};

// forward_list用(先頭への追加が自然なので、逆順で追加する)
template<class tContainer, typename tType>
struct Emplace
<
    tContainer,
    tType,
    typename std::enable_if<std::is_convertible<tContainer,std::forward_list<tType>>::value>::type
>
{
    void operator()(tContainer& iContainer, tType const& iFirst, std::size_t iCount)
    {
        for (std::size_t i=iCount; 0 < i; --i)
        {
            iContainer.emplace_front(iFirst+static_cast<int>(i-1));
        }
    }
};

// ***************************************************************************
//      サブ処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      固定長コンテナのテスト
//----------------------------------------------------------------------------

//      ---<<< 保存処理 >>>---

template<class tSerializer, class tContainer, typename tType>
void saveContainerFixedSub
(
    tSerializer& iSerializer,
    tContainer& iContainer,
    tType const& iFirst,
    bool iIsPointee
)
{
    // 保存データ生成
    for (std::size_t i=0; i < iContainer.size(); ++i)
    {
        iContainer[i]=iFirst+static_cast<int>(i);
    }
    std::vector<tType*> aPtr;
    if (iIsPointee)
    {
        aPtr.resize(iContainer.size());
        for (std::size_t i=0; i < iContainer.size(); ++i)
        {
            aPtr[i]=&iContainer[i];
        }
    }

    // 保存
    if (iIsPointee)
    {
        for (std::size_t i=0; i < iContainer.size(); ++i)
        {
            THEOLIZER_PROCESS(iSerializer, aPtr[i]);
        }
    }
    THEOLIZER_PROCESS(iSerializer, iContainer);
}

template
<
    class tSerializer,
    template<typename tType, std::size_t tSize> class tContainer,
    typename tType
>
void saveContainerFixed
(
    tSerializer& iSerializer,
    tType const& iFirst,
    bool iIsPointee
)
{
    tContainer<tType, 3>  aArray0;
    saveContainerFixedSub(iSerializer, aArray0, tType(iFirst), iIsPointee);

    tContainer<tType, 4>  aArray1;
    saveContainerFixedSub(iSerializer, aArray1, tType(iFirst+100), iIsPointee);

    iSerializer.clearTracking();
}

//      ---<<< 回復処理 >>>---

template<class tSerializer, class tContainer, typename tType>
void loadContainerFixedSub
(
    tSerializer& iSerializer,
    tContainer& iContainer,
    tType const& iFirst,
    bool iIsPointee
)
{
    // ポインタ回復
    std::vector<tType*> aPtr;
    if (iIsPointee)
    {
        aPtr.resize(iContainer.size());
        for (std::size_t i=0; i < iContainer.size(); ++i)
        {
            aPtr[i]=nullptr;
        }

        for (std::size_t i=0; i < iContainer.size(); ++i)
        {
            THEOLIZER_PROCESS(iSerializer, aPtr[i]);
        }
    }
    // コンテナ回復
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    for (std::size_t i=0; i < iContainer.size(); ++i)
    {
        if (iIsPointee)
        {
            THEOLIZER_EQUAL_PTR(aPtr[i], &iContainer[i]);
        }
        THEOLIZER_EQUAL(iContainer[i], iFirst+static_cast<int>(i));
    }
}

template
<
    class tSerializer,
    template<typename tType, std::size_t tSize> class tContainer,
    typename tType
>
void loadContainerFixed
(
    tSerializer& iSerializer,
    tType const& iFirst,
    bool iIsPointee
)
{
    tContainer<tType, 3>  aArray0;
    loadContainerFixedSub(iSerializer, aArray0, tType(iFirst), iIsPointee);

    tContainer<tType, 4>  aArray1;
    loadContainerFixedSub(iSerializer, aArray1, tType(iFirst+100), iIsPointee);

    iSerializer.clearTracking();
}

//----------------------------------------------------------------------------
//      キー無しコンテナのテスト
//----------------------------------------------------------------------------

//      ---<<< 保存処理 >>>---

template<class tSerializer, class tContainer, typename tType>
void saveContainerSub
(
    tSerializer& iSerializer,
    tContainer& iContainer,
    tType const& iFirst,
    bool iIsPointee
)
{
    // 保存データ生成
    tType   *aPtr0;
    tType   *aPtr1;
    tType   *aPtr2;
    Emplace<tContainer, tType> emplace;
    emplace(iContainer, iFirst, 3);
    if (iIsPointee)
    {
        auto itr=iContainer.begin();
        aPtr0=&*(itr++);
        aPtr1=&*(itr++);
        aPtr2=&*(itr++);
    }

    // 保存
    if (iIsPointee)
    {
        THEOLIZER_PROCESS(iSerializer, aPtr0);
        THEOLIZER_PROCESS(iSerializer, aPtr1);
        THEOLIZER_PROCESS(iSerializer, aPtr2);
    }
    THEOLIZER_PROCESS(iSerializer, iContainer);
}

template<class tSerializer, class tContainer, typename tType>
void saveContainer(tSerializer& iSerializer, tType const& iFirst, bool iIsPointee)
{
    tContainer  aVector0;
    saveContainerSub(iSerializer, aVector0, tType(iFirst), iIsPointee);

    tContainer  aVector1;
    saveContainerSub(iSerializer, aVector1, tType(iFirst+100), iIsPointee);

    tContainer  aVector2;
    saveContainerSub(iSerializer, aVector2, tType(iFirst+200), iIsPointee);

    iSerializer.clearTracking();
}

//      ---<<< 回復処理 >>>---

// 事前準備要素数=0
template<class tSerializer, class tContainer, typename tType>
void loadContainer0Sub
(
    tSerializer& iSerializer,
    tContainer& iContainer,
    tType const& iFirst,
    bool iIsPointee
)
{
    // 回復先生成
    tType   *aPtr0=nullptr;
    tType   *aPtr1=nullptr;
    tType   *aPtr2=nullptr;
    iContainer.clear();

    // 回復
    if (iIsPointee)
    {
        THEOLIZER_PROCESS(iSerializer, aPtr0);
        THEOLIZER_PROCESS(iSerializer, aPtr1);
        THEOLIZER_PROCESS(iSerializer, aPtr2);
    }
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    auto itr=iContainer.begin();
    if (iIsPointee)
    {
        THEOLIZER_EQUAL_PTR(aPtr0, &*itr);  ++itr;
        THEOLIZER_EQUAL_PTR(aPtr1, &*itr);  ++itr;
        THEOLIZER_EQUAL_PTR(aPtr2, &*itr);  ++itr;
        itr=iContainer.begin();
    }
    THEOLIZER_EQUAL(*itr, iFirst+0);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+1);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+2);    ++itr;
}

// 事前準備要素数=2
template<class tSerializer, class tContainer, typename tType>
void loadContainer2Sub
(
    tSerializer& iSerializer,
    tContainer& iContainer,
    tType const& iFirst,
    bool iIsPointee
)
{
    // 回復先生成
    tType   *aPtr0=nullptr;
    tType   *aPtr1=nullptr;
    tType   *aPtr2=nullptr;
    iContainer.resize(2);

    // 回復
    if (iIsPointee)
    {
        THEOLIZER_PROCESS(iSerializer, aPtr0);
        THEOLIZER_PROCESS(iSerializer, aPtr1);
        THEOLIZER_PROCESS(iSerializer, aPtr2);
    }
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    auto itr=iContainer.begin();
    if (iIsPointee)
    {
        THEOLIZER_EQUAL_PTR(aPtr0, &*itr);  ++itr;
        THEOLIZER_EQUAL_PTR(aPtr1, &*itr);  ++itr;
        THEOLIZER_EQUAL_PTR(aPtr2, &*itr);  ++itr;
        itr=iContainer.begin();
    }
    THEOLIZER_EQUAL(*itr, iFirst+0);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+1);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+2);    ++itr;
}

// 事前準備要素数=4
template<class tSerializer, class tContainer, typename tType>
void loadContainer4Sub
(
    tSerializer& iSerializer,
    tContainer& iContainer,
    tType const& iFirst,
    bool iIsPointee
)
{
    // 回復先生成
    tType   *aPtr0=nullptr;
    tType   *aPtr1=nullptr;
    tType   *aPtr2=nullptr;
    iContainer.resize(4, iFirst+3);

    // 回復
    if (iIsPointee)
    {
        THEOLIZER_PROCESS(iSerializer, aPtr0);
        THEOLIZER_PROCESS(iSerializer, aPtr1);
        THEOLIZER_PROCESS(iSerializer, aPtr2);
    }
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    auto itr=iContainer.begin();
    if (iIsPointee)
    {
        THEOLIZER_EQUAL_PTR(aPtr0, &*itr);  ++itr;
        THEOLIZER_EQUAL_PTR(aPtr1, &*itr);  ++itr;
        THEOLIZER_EQUAL_PTR(aPtr2, &*itr);  ++itr;
        itr=iContainer.begin();
    }
    THEOLIZER_EQUAL(*itr, iFirst+0);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+1);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+2);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+3);    ++itr;
}

template<class tSerializer, class tContainer, typename tType>
void loadContainer(tSerializer& iSerializer, tType const& iFirst, bool iIsPointee)
{
    tContainer  aVector0;
    loadContainer0Sub(iSerializer, aVector0, tType(iFirst), iIsPointee);

    tContainer  aVector1;
    loadContainer2Sub(iSerializer, aVector1, tType(iFirst+100), iIsPointee);

    tContainer  aVector2;
    loadContainer4Sub(iSerializer, aVector2, tType(iFirst+200), iIsPointee);

    iSerializer.clearTracking();
}

//      ---<<< std::setシリーズのテスト >>>---

template<class tSerializer, class tContainer, typename tType>
void saveContainerSet(tSerializer& iSerializer, tType const& iFirst)
{
    for (int i=0; i < 2; ++i)
    {
        // 保存データ生成
        tContainer  aContainer;
        aContainer.insert(iFirst+i*100+0);
        aContainer.insert(iFirst+i*100+1);
        aContainer.insert(iFirst+i*100+2);

        // 保存
        THEOLIZER_PROCESS(iSerializer, aContainer);
    }
}

template<class tSerializer, class tContainer, typename tType>
void loadContainerSet(tSerializer& iSerializer, tType const& iFirst, bool iIsMulti)
{
    {
        // 回復領域生成
        tContainer  aContainer;

        // 回復
        THEOLIZER_PROCESS(iSerializer, aContainer);

        // チェック
        THEOLIZER_EQUAL(aContainer.size(), 3);
        THEOLIZER_CHECK(aContainer.find(iFirst+0) != aContainer.end(), iFirst+0);
        THEOLIZER_CHECK(aContainer.find(iFirst+1) != aContainer.end(), iFirst+1);
        THEOLIZER_CHECK(aContainer.find(iFirst+2) != aContainer.end(), iFirst+2);
    }
    {
        // 回復領域生成
        tContainer  aContainer;
        aContainer.insert(iFirst+100+0);
        aContainer.insert(iFirst+100+1);
        aContainer.insert(iFirst+100+2);
        aContainer.insert(iFirst+100+3);

        // 回復
        THEOLIZER_PROCESS(iSerializer, aContainer);

        // チェック
        THEOLIZER_CHECK(aContainer.find(iFirst+100+0) != aContainer.end(), iFirst+100+0);
        THEOLIZER_CHECK(aContainer.find(iFirst+100+1) != aContainer.end(), iFirst+100+1);
        THEOLIZER_CHECK(aContainer.find(iFirst+100+2) != aContainer.end(), iFirst+100+2);
        if (!iIsMulti)
        {
            THEOLIZER_EQUAL(aContainer.size(), 4);
            THEOLIZER_CHECK(aContainer.find(iFirst+100+3) != aContainer.end(), iFirst+100+3);
        }
        else
        {
            THEOLIZER_EQUAL(aContainer.size(), 3);
        }
    }
}

//      ---<<< std::mapシリーズのテスト >>>---

// emplaceの戻り値の分岐(std::mapとstd::unordered_mapでemplaceの戻り値が異なる)
template<class tIterator>
tIterator getIterator(tIterator iIterator)
{
    return iIterator;
}

template<class tIterator>
tIterator getIterator(std::pair<tIterator, bool>& iPair)
{
    return iPair.first;
}

// 本体
template<class tSerializer, class tContainer, typename tType>
void saveContainerMap(tSerializer& iSerializer, tType const& iFirst, bool iIsPointee)
{
    for (int i=0; i < 2; ++i)
    {
        // 保存データ生成
        tType   *aPtr0;
        tType   *aPtr1;
        tType   *aPtr2;
        tContainer  aContainer;
        auto temp0=aContainer.emplace(iFirst+i*100+0, iFirst+i*100+10);
        if (iIsPointee)
        {
            aPtr0=&(getIterator(temp0)->second);
        }
        auto temp1=aContainer.emplace(iFirst+i*100+1, iFirst+i*100+11);
        if (iIsPointee)
        {
            aPtr1=&(getIterator(temp1)->second);
        }
        auto temp2=aContainer.emplace(iFirst+i*100+2, iFirst+i*100+12);
        if (iIsPointee)
        {
            aPtr2=&(getIterator(temp2)->second);
        }

        // 保存
        if (iIsPointee)
        {
            THEOLIZER_PROCESS(iSerializer, aPtr0);
            THEOLIZER_PROCESS(iSerializer, aPtr1);
            THEOLIZER_PROCESS(iSerializer, aPtr2);
        }
        THEOLIZER_PROCESS(iSerializer, aContainer);

        iSerializer.clearTracking();
    }
}

template<class tSerializer, class tContainer, typename tType>
void loadContainerMap(tSerializer& iSerializer,tType const& iFirst,bool iIsMulti,bool iIsPointee)
{
    {
        // 回復領域生成
        tContainer  aContainer;
        tType   *aPtr0=nullptr;
        tType   *aPtr1=nullptr;
        tType   *aPtr2=nullptr;

        // 回復
        if (iIsPointee)
        {
            THEOLIZER_PROCESS(iSerializer, aPtr0);
            THEOLIZER_PROCESS(iSerializer, aPtr1);
            THEOLIZER_PROCESS(iSerializer, aPtr2);
        }
        THEOLIZER_PROCESS(iSerializer, aContainer);

        // チェック
        THEOLIZER_EQUAL(aContainer.size(), 3);

        auto itr=aContainer.find(iFirst+0);
        THEOLIZER_CHECK(itr != aContainer.end(), iFirst+0);
        THEOLIZER_EQUAL(itr->second, iFirst+10);
        if (iIsPointee && (itr != aContainer.end()))
        {
            THEOLIZER_EQUAL_PTR(aPtr0, &(itr->second));
        }

        itr=aContainer.find(iFirst+1);
        THEOLIZER_CHECK(itr != aContainer.end(), iFirst+1);
        THEOLIZER_EQUAL(itr->second, iFirst+11);
        if (iIsPointee && (itr != aContainer.end()))
        {
            THEOLIZER_EQUAL_PTR(aPtr1, &(itr->second));
        }

        itr=aContainer.find(iFirst+2);
        THEOLIZER_CHECK(itr != aContainer.end(), iFirst+2);
        THEOLIZER_EQUAL(itr->second, iFirst+12);
        if (iIsPointee && (itr != aContainer.end()))
        {
            THEOLIZER_EQUAL_PTR(aPtr2, &(itr->second));
        }

        iSerializer.clearTracking();
    }
    {
        // 回復領域生成
        tContainer  aContainer;
        aContainer.insert(typename tContainer::value_type(iFirst+100+0, iFirst+100+10));
        aContainer.insert(typename tContainer::value_type(iFirst+100+1, iFirst+100+11));
        aContainer.insert(typename tContainer::value_type(iFirst+100+2, iFirst+100+12));
        aContainer.insert(typename tContainer::value_type(iFirst+100+3, iFirst+100+13));
        tType   *aPtr0=nullptr;
        tType   *aPtr1=nullptr;
        tType   *aPtr2=nullptr;

        // 回復
        if (iIsPointee)
        {
            THEOLIZER_PROCESS(iSerializer, aPtr0);
            THEOLIZER_PROCESS(iSerializer, aPtr1);
            THEOLIZER_PROCESS(iSerializer, aPtr2);
        }
        THEOLIZER_PROCESS(iSerializer, aContainer);

        // チェック
        auto itr=aContainer.find(iFirst+100+0);
        THEOLIZER_CHECK(itr != aContainer.end(), iFirst+100+0);
        THEOLIZER_EQUAL(itr->second, iFirst+100+10);
        if (iIsPointee && (itr != aContainer.end()))
        {
            THEOLIZER_EQUAL_PTR(aPtr0, &(itr->second));
        }

        itr=aContainer.find(iFirst+100+1);
        THEOLIZER_CHECK(itr != aContainer.end(), iFirst+100+1);
        THEOLIZER_EQUAL(itr->second, iFirst+100+11);
        if (iIsPointee && (itr != aContainer.end()))
        {
            THEOLIZER_EQUAL_PTR(aPtr1, &(itr->second));
        }

        itr=aContainer.find(iFirst+100+2);
        THEOLIZER_CHECK(itr != aContainer.end(), iFirst+100+2);
        THEOLIZER_EQUAL(itr->second, iFirst+100+12);
        if (iIsPointee && (itr != aContainer.end()))
        {
            THEOLIZER_EQUAL_PTR(aPtr2, &(itr->second));
        }

        if (!iIsMulti)
        {
            THEOLIZER_EQUAL(aContainer.size(), 4);
            itr=aContainer.find(iFirst+100+3);
            THEOLIZER_CHECK(itr != aContainer.end(), iFirst+100+3);
            THEOLIZER_EQUAL(itr->second, iFirst+100+13);
        }
        else
        {
            THEOLIZER_EQUAL(aContainer.size(), 3);
        }

        iSerializer.clearTracking();
    }
}

// ***************************************************************************
//      保存先指定の合成テスト用
// ***************************************************************************

//----------------------------------------------------------------------------
//      固定長コンテナ
//----------------------------------------------------------------------------

template<class tContainer, class tSerializerA, class tSerializerB>
void saveFixed
(
    tSerializerA&   iSerializerA,
    tSerializerB&   iSerializerB,
    int             iFirst
)
{
    tContainer  aContainer;
    for (std::size_t i=0; i < aContainer.size(); ++i)
    {
        aContainer[i]=TestStlDestinations(iFirst+static_cast<int>(i));
    }
    THEOLIZER_PROCESS(iSerializerA, aContainer);
    THEOLIZER_PROCESS(iSerializerB, aContainer);

    iSerializerA.clearTracking();
    iSerializerB.clearTracking();
}

template<class tContainer, class tSerializerA, class tSerializerB>
void loadFixed
(
    tSerializerA&   iSerializerA,
    tSerializerB&   iSerializerB,
    int             iFirst
)
{
    tContainer  aContainer;
    THEOLIZER_PROCESS(iSerializerA, aContainer);
    for (std::size_t i=0; i < aContainer.size(); ++i)
    {
        THEOLIZER_EQUAL(aContainer[i], TestStlDestinations(iFirst+static_cast<int>(i), 0));
    }
    THEOLIZER_PROCESS(iSerializerB, aContainer);
    for (std::size_t i=0; i < aContainer.size(); ++i)
    {
        THEOLIZER_EQUAL(aContainer[i], TestStlDestinations(iFirst+static_cast<int>(i)));
    }

    iSerializerA.clearTracking();
    iSerializerB.clearTracking();
}

//----------------------------------------------------------------------------
//      キー無しコンテナ
//----------------------------------------------------------------------------

template<class tContainer, class tSerializerA, class tSerializerB>
void saveNoKey
(
    tSerializerA&   iSerializerA,
    tSerializerB&   iSerializerB,
    int             iFirst
)
{
    tContainer  aContainer;
    Emplace<tContainer, TestStlDestinations> emplace;
    emplace(aContainer, TestStlDestinations(iFirst), 3);
    THEOLIZER_PROCESS(iSerializerA, aContainer);
    THEOLIZER_PROCESS(iSerializerB, aContainer);

    iSerializerA.clearTracking();
    iSerializerB.clearTracking();
}

template<class tContainer, class tSerializerA, class tSerializerB>
void loadNoKey
(
    tSerializerA&   iSerializerA,
    tSerializerB&   iSerializerB,
    int             iFirst
)
{
    tContainer  aContainer;
    THEOLIZER_PROCESS(iSerializerA, aContainer);
    int i=0;
    for (auto&& itr : aContainer)
    {
        THEOLIZER_EQUAL(itr, TestStlDestinations(iFirst+i, 0));
        ++i;
    }
    THEOLIZER_PROCESS(iSerializerB, aContainer);
    i=0;
    for (auto&& itr : aContainer)
    {
        THEOLIZER_EQUAL(itr, TestStlDestinations(iFirst+i));
        ++i;
    }

    iSerializerA.clearTracking();
    iSerializerB.clearTracking();
}

//----------------------------------------------------------------------------
//      キー有りコンテナ
//----------------------------------------------------------------------------

template<class tContainer, class tSerializerA, class tSerializerB>
void saveKey
(
    tSerializerA&   iSerializerA,
    tSerializerB&   iSerializerB,
    int             iFirst
)
{
    tContainer  aContainer;
    aContainer.emplace("start",  TestStlDestinations(iFirst+0));
    aContainer.emplace("center", TestStlDestinations(iFirst+1));
    aContainer.emplace("end",    TestStlDestinations(iFirst+2));
    THEOLIZER_PROCESS(iSerializerA, aContainer);
    THEOLIZER_PROCESS(iSerializerB, aContainer);

    iSerializerA.clearTracking();
    iSerializerB.clearTracking();
}

template<class tContainer, class tSerializerA, class tSerializerB>
void loadKey
(
    tSerializerA&   iSerializerA,
    tSerializerB&   iSerializerB,
    int             iFirst
)
{
    tContainer  aContainer;

    THEOLIZER_PROCESS(iSerializerA, aContainer);
    auto itr=aContainer.find("start");
    THEOLIZER_CHECK(itr != aContainer.end(), "start");
    THEOLIZER_EQUAL(itr->second, TestStlDestinations(iFirst+0, 0));
    itr=aContainer.find("center");
    THEOLIZER_CHECK(itr != aContainer.end(), "center");
    THEOLIZER_EQUAL(itr->second, TestStlDestinations(iFirst+1, 0));
    itr=aContainer.find("end");
    THEOLIZER_CHECK(itr != aContainer.end(), "end");
    THEOLIZER_EQUAL(itr->second, TestStlDestinations(iFirst+2, 0));

    THEOLIZER_PROCESS(iSerializerB, aContainer);
    itr=aContainer.find("start");
    THEOLIZER_CHECK(itr != aContainer.end(), "start");
    THEOLIZER_EQUAL(itr->second, TestStlDestinations(iFirst+0));
    itr=aContainer.find("center");
    THEOLIZER_CHECK(itr != aContainer.end(), "center");
    THEOLIZER_EQUAL(itr->second, TestStlDestinations(iFirst+1));
    itr=aContainer.find("end");
    THEOLIZER_CHECK(itr != aContainer.end(), "end");
    THEOLIZER_EQUAL(itr->second, TestStlDestinations(iFirst+2));

    iSerializerA.clearTracking();
    iSerializerB.clearTracking();
}

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveSupportStl(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      スマート・ポインタ対応
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Manual(Top Level)" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};

        std::shared_ptr<SmartBase>  aShared0{new SmartDerived{100}};
        std::shared_ptr<SmartBase>  aForWeak0{new SmartDerived{110}};

        std::shared_ptr<SmartBase>  aShared1{new SmartDerived{200}};
        std::shared_ptr<SmartBase>  aForWeak1{new SmartDerived{210}};

        std::shared_ptr<SmartBase>  aShared2{new SmartDerived{300}};
        std::shared_ptr<SmartBase>  aForWeak2{new SmartDerived{310}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{120, aShared0, aForWeak0};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{220, aShared1, aForWeak1};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{320, aShared2, aForWeak2};

        // 保存
        processSmart(iSerializer, aSmartTestAutoNull);

        THEOLIZER_PROCESS(iSerializer, aShared0);
        THEOLIZER_PROCESS(iSerializer, aForWeak0);
        processSmart(iSerializer, aSmartTestAuto0);

        THEOLIZER_PROCESS(iSerializer, aShared1);
        THEOLIZER_PROCESS(iSerializer, aForWeak1);
        processSmart(iSerializer, aSmartTestAuto1);

        THEOLIZER_PROCESS(iSerializer, aShared2);
        THEOLIZER_PROCESS(iSerializer, aForWeak2);
        processSmart(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();
    }

//      ---<<< 自動メンバ・リスト生成による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Auto" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{200}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{210}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{220, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{230, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{240, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAutoNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();
    }

//      ---<<< 手動(非トップ・レベル)による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Manul(Non-Top)" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{300}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{310}};

        // nullptrチェック用
        SmartTestManual             aSmartTestManualNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestManual             aSmartTestManual0{320, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual1{330, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual2{340, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManualNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual2);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      コンテナ対応
//----------------------------------------------------------------------------

//      ---<<< std::array >>>---

    {
        std::cout << "        saveContainerFixed() : array<int, *>" << std::endl;
        saveContainerFixed<tSerializer, std::array, int>(iSerializer, 1000, false);

        std::cout << "        saveContainerFixed() : array<TestStl, *>" << std::endl;
        saveContainerFixed<tSerializer, std::array, TestStl>(iSerializer, 2000, false);

        std::cout << "        saveContainerFixed() : ArrayPointee<int, *>" << std::endl;
        saveContainerFixed<tSerializer, theolizer::ArrayPointee, int>(iSerializer, 3000, true);

        std::cout << "        saveContainerFixed() : ArrayPointee<TestStl, *>" << std::endl;
        saveContainerFixed<tSerializer, theolizer::ArrayPointee, TestStl>(iSerializer, 4000, true);
    }

//      ---<<< std::vector >>>---

    {
        std::cout << "        saveContainer() : std::vector<int>" << std::endl;
        saveContainer<tSerializer, std::vector<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        saveContainer() : std::vector<TestStl>" << std::endl;
        saveContainer<tSerializer, std::vector<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        saveContainer() : theolizer::VectorPointee<int>" << std::endl;
        saveContainer<tSerializer, theolizer::VectorPointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        saveContainer() : theolizer::VectorPointee<TestStl>" << std::endl;
        saveContainer<tSerializer, theolizer::VectorPointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::vector<bool> >>>---

    {
        std::cout << "        save : std::vector<bool>" << std::endl;
        std::vector<bool>   aVectorBool;
        aVectorBool.resize(3, true);
        THEOLIZER_PROCESS(iSerializer, aVectorBool);
        THEOLIZER_PROCESS(iSerializer, aVectorBool);
        THEOLIZER_PROCESS(iSerializer, aVectorBool);
    }

//      ---<<< std::deque >>>---

    {
        std::cout << "        saveContainer() : std::deque<int>" << std::endl;
        saveContainer<tSerializer, std::deque<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        saveContainer() : std::deque<TestStl>" << std::endl;
        saveContainer<tSerializer, std::deque<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        saveContainer() : theolizer::DequePointee<int>" << std::endl;
        saveContainer<tSerializer, theolizer::DequePointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        saveContainer() : theolizer::DequePointee<TestStl>" << std::endl;
        saveContainer<tSerializer, theolizer::DequePointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::list >>>---

    {
        std::cout << "        saveContainer() : std::list<int>" << std::endl;
        saveContainer<tSerializer, std::list<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        saveContainer() : std::list<TestStl>" << std::endl;
        saveContainer<tSerializer, std::list<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        saveContainer() : theolizer::ListPointee<int>" << std::endl;
        saveContainer<tSerializer, theolizer::ListPointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        saveContainer() : theolizer::ListPointee<TestStl>" << std::endl;
        saveContainer<tSerializer, theolizer::ListPointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::forward_list >>>---

    {
        std::cout << "        saveContainer() : std::forward_list<int>" << std::endl;
        saveContainer<tSerializer, std::forward_list<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        saveContainer() : std::forward_list<TestStl>" << std::endl;
        saveContainer<tSerializer, std::forward_list<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        saveContainer() : theolizer::ForwardListPointee<int>" << std::endl;
        saveContainer<tSerializer, theolizer::ForwardListPointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        saveContainer() : theolizer::ForwardListPointee<TestStl>"<<std::endl;
        saveContainer<tSerializer, theolizer::ForwardListPointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::set >>>---

    {
        std::cout << "        saveContainerSet() : std::set<int>" << std::endl;
        saveContainerSet<tSerializer, std::set<int>, int>
        (
            iSerializer, 100
        );

        std::cout << "        saveContainerSet() : std::set<TestStl>" << std::endl;
        saveContainerSet<tSerializer, std::set<TestStl>, TestStl>
        (
            iSerializer, 500
        );
    }

//      ---<<< std::multiset >>>---

    {
        std::cout << "        saveContainerSet() : std::multiset<int>" << std::endl;
        saveContainerSet<tSerializer, std::multiset<int>, int>
        (
            iSerializer, 100
        );

        std::cout << "        saveContainerSet() : std::multiset<TestStl>" << std::endl;
        saveContainerSet<tSerializer, std::multiset<TestStl>, TestStl>
        (
            iSerializer, 500
        );
    }

//      ---<<< std::map >>>---

    {
        std::cout << "        saveContainerMap() : std::map<int, int>" << std::endl;
        saveContainerMap<tSerializer, std::map<int, int>, int>
        (
            iSerializer, 100, false
        );

        std::cout << "        saveContainerMap() : std::map<TestStl, TestStl>" << std::endl;
        saveContainerMap<tSerializer, std::map<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false
        );

        // 被ポインタのテスト
        std::cout << "        saveContainerMap() : theolizer::MapPointee<int, int>" << std::endl;
        saveContainerMap<tSerializer, theolizer::MapPointee<int, int>, int>
        (
            iSerializer, 100, true
        );

        std::cout << "        saveContainerMap() : theolizer::MapPointee<TestStl, TestStl>"
            << std::endl;
        saveContainerMap<tSerializer, theolizer::MapPointee<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, true
        );
    }

//      ---<<< std::multimap >>>---

    {
        std::cout << "        saveContainerMap() : std::multimap<int, int>" << std::endl;
        saveContainerMap<tSerializer, std::multimap<int, int>, int>
        (
            iSerializer, 100, false
        );

        std::cout << "        saveContainerMap() : std::multimap<TestStl, TestStl>" << std::endl;
        saveContainerMap<tSerializer, std::multimap<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false
        );
    }

//      ---<<< std::unordered_set >>>---

    {
        std::cout << "        saveContainerSet() : std::unordered_set<int>" << std::endl;
        saveContainerSet<tSerializer, std::unordered_set<int>, int>
        (
            iSerializer, 100
        );

        std::cout << "        saveContainerSet() : std::unordered_set<TestStl>" << std::endl;
        saveContainerSet<tSerializer, std::unordered_set<TestStl>, TestStl>
        (
            iSerializer, 500
        );
    }

//      ---<<< std::unordered_multiset >>>---

    {
        std::cout << "        saveContainerSet() : std::unordered_multiset<int>" << std::endl;
        saveContainerSet<tSerializer, std::unordered_multiset<int>, int>
        (
            iSerializer, 100
        );

        std::cout << "        saveContainerSet() : std::unordered_multiset<TestStl>" << std::endl;
        saveContainerSet<tSerializer, std::unordered_multiset<TestStl>, TestStl>
        (
            iSerializer, 500
        );
    }

//      ---<<< std::unordered_map >>>---

    {
        std::cout << "        saveContainerMap() : std::unordered_map<int, int>" << std::endl;
        saveContainerMap<tSerializer, std::unordered_map<int, int>, int>
        (
            iSerializer, 100, false
        );

        std::cout << "        saveContainerMap() : std::unordered_map<TestStl, TestStl>"
            << std::endl;
        saveContainerMap<tSerializer, std::unordered_map<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false
        );

        // 被ポインタのテスト
        std::cout << "        saveContainerMap() : theolizer::UnorderedMapPointee<int, int>"
            << std::endl;
        saveContainerMap<tSerializer, theolizer::UnorderedMapPointee<int, int>, int>
        (
            iSerializer, 100, true
        );

        std::cout << "        saveContainerMap() : theolizer::UnorderedMapPointee<TestStl,TestStl>"
            << std::endl;
        saveContainerMap<tSerializer, theolizer::UnorderedMapPointee<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, true
        );
    }

//      ---<<< std::unordered_multimap >>>---

    {
        std::cout << "        saveContainerMap() : std::unordered_multimap<int, int>" << std::endl;
        saveContainerMap<tSerializer, std::unordered_multimap<int, int>, int>
        (
            iSerializer, 100, false
        );

        std::cout << "        saveContainerMap() : std::unordered_multimap<TestStl, TestStl>"
            << std::endl;
        saveContainerMap<tSerializer, std::unordered_multimap<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false
        );
    }
}

INSTANTIATION_ALL(saveSupportStl);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadSupportStl(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      スマート・ポインタ対応
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Manual(Top Level)" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};

        std::shared_ptr<SmartBase>  aShared0{};
        std::shared_ptr<SmartBase>  aForWeak0{};

        std::shared_ptr<SmartBase>  aShared1{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak1{new SmartDerived{}};

        std::shared_ptr<SmartBase>  aShared2{new SmartBase{}};
        std::shared_ptr<SmartBase>  aForWeak2{new SmartBase{}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{};

        // 回復
        processSmart(iSerializer, aSmartTestAutoNull);

        THEOLIZER_PROCESS(iSerializer, aShared0);
        THEOLIZER_PROCESS(iSerializer, aForWeak0);
        processSmart(iSerializer, aSmartTestAuto0);

        THEOLIZER_PROCESS(iSerializer, aShared1);
        THEOLIZER_PROCESS(iSerializer, aForWeak1);
        processSmart(iSerializer, aSmartTestAuto1);

        THEOLIZER_PROCESS(iSerializer, aShared2);
        THEOLIZER_PROCESS(iSerializer, aForWeak2);
        processSmart(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();

        // チェック
        THEOLIZER_EQUAL(static_cast<bool>(aSharedNull), false);
        THEOLIZER_EQUAL(static_cast<bool>(aForWeakNull), false);
        aShared0.get()->check(100);
        aForWeak0.get()->check(110);
        aSmartTestAuto0.check(120, aShared0, aForWeak0);

        aShared1.get()->check(200);
        aForWeak1.get()->check(210);
        aSmartTestAuto1.check(220, aShared1, aForWeak1);

        aShared2.get()->check(300);
        aForWeak2.get()->check(310);
        aSmartTestAuto2.check(320, aShared2, aForWeak2);
    }

//      ---<<< 自動メンバ・リスト生成による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Auto" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAutoNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(200);
        aForWeak.get()->check(210);
        THEOLIZER_EQUAL(static_cast<bool>(aSharedNull), false);
        THEOLIZER_EQUAL(static_cast<bool>(aForWeakNull), false);
        aSmartTestAuto0.check(220, aShared, aForWeak);
        aSmartTestAuto1.check(230, aShared, aForWeak);
        aSmartTestAuto2.check(240, aShared, aForWeak);
    }

//      ---<<< 手動(非トップ・レベル)による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Manul(Non-Top)" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestManual             aSmartTestManualNull{};
        // nullptrへの回復チェック用
        SmartTestManual             aSmartTestManual0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManualNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(300);
        aForWeak.get()->check(310);
        THEOLIZER_EQUAL(static_cast<bool>(aSharedNull), false);
        THEOLIZER_EQUAL(static_cast<bool>(aForWeakNull), false);
        aSmartTestManual0.check(320, aShared, aForWeak);
        aSmartTestManual1.check(330, aShared, aForWeak);
        aSmartTestManual2.check(340, aShared, aForWeak);
    }

//----------------------------------------------------------------------------
//      コンテナ対応
//----------------------------------------------------------------------------

//      ---<<< std::array >>>---

    {
        std::cout << "        loadContainerFixed() : array<int, *>" << std::endl;
        loadContainerFixed<tSerializer, std::array, int>(iSerializer, 1000, false);

        std::cout << "        loadContainerFixed() : array<TestStl, *>" << std::endl;
        loadContainerFixed<tSerializer, std::array, TestStl>(iSerializer, 2000, false);

        std::cout << "        loadContainerFixed() : ArrayPointee<int, *>" << std::endl;
        loadContainerFixed<tSerializer, theolizer::ArrayPointee, int>(iSerializer, 3000, true);

        std::cout << "        loadContainerFixed() : ArrayPointee<TestStl, *>" << std::endl;
        loadContainerFixed<tSerializer, theolizer::ArrayPointee, TestStl>(iSerializer, 4000, true);
    }

//      ---<<< std::vector >>>---

    {
        std::cout << "        loadContainer() : std::vector<int>" << std::endl;
        loadContainer<tSerializer, std::vector<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        loadContainer() : std::vector<TestStl>" << std::endl;
        loadContainer<tSerializer, std::vector<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        loadContainer() : theolizer::VectorPointee<int>" << std::endl;
        loadContainer<tSerializer, theolizer::VectorPointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        loadContainer() : theolizer::VectorPointee<TestStl>" << std::endl;
        loadContainer<tSerializer, theolizer::VectorPointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::vector<bool> >>>---

    {
        std::cout << "        load : std::vector<bool>" << std::endl;
        std::vector<bool>   aVectorBool0;
        THEOLIZER_PROCESS(iSerializer, aVectorBool0);
        std::vector<bool>   aVectorBool1;
        aVectorBool1.resize(2, false);
        THEOLIZER_PROCESS(iSerializer, aVectorBool1);
        std::vector<bool>   aVectorBool2;
        aVectorBool2.resize(4, false);
        THEOLIZER_PROCESS(iSerializer, aVectorBool2);

        iSerializer.clearTracking();

        for (std::size_t i=0; i < 3; ++i)
        {
            THEOLIZER_EQUAL(aVectorBool0[i], true);
            THEOLIZER_EQUAL(aVectorBool1[i], true);
            THEOLIZER_EQUAL(aVectorBool2[i], true);
        }
        THEOLIZER_EQUAL(aVectorBool2[3], false);
    }

//      ---<<< std::deque >>>---

    {
        std::cout << "        loadContainer() : std::deque<int>" << std::endl;
        loadContainer<tSerializer, std::deque<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        loadContainer() : std::deque<TestStl>" << std::endl;
        loadContainer<tSerializer, std::deque<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        loadContainer() : theolizer::DequePointee<int>" << std::endl;
        loadContainer<tSerializer, theolizer::DequePointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        loadContainer() : theolizer::DequePointee<TestStl>" << std::endl;
        loadContainer<tSerializer, theolizer::DequePointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::list >>>---

    {
        std::cout << "        loadContainer() : std::list<int>" << std::endl;
        loadContainer<tSerializer, std::list<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        loadContainer() : std::list<TestStl>" << std::endl;
        loadContainer<tSerializer, std::list<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        loadContainer() : theolizer::ListPointee<int>" << std::endl;
        loadContainer<tSerializer, theolizer::ListPointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        loadContainer() : theolizer::ListPointee<TestStl>" << std::endl;
        loadContainer<tSerializer, theolizer::ListPointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::forward_list >>>---

    {
        std::cout << "        loadContainer() : std::forward_list<int>" << std::endl;
        loadContainer<tSerializer, std::forward_list<int>, int>
            (iSerializer, int(1000), false);

        std::cout << "        loadContainer() : std::forward_list<TestStl>" << std::endl;
        loadContainer<tSerializer, std::forward_list<TestStl>, TestStl>
            (iSerializer, TestStl(2000), false);

        std::cout << "        loadContainer() : theolizer::ForwardListPointee<int>" << std::endl;
        loadContainer<tSerializer, theolizer::ForwardListPointee<int>, int>
            (iSerializer, int(3000), true);

        std::cout << "        loadContainer() : theolizer::ForwardListPointee<TestStl>"<<std::endl;
        loadContainer<tSerializer, theolizer::ForwardListPointee<TestStl>, TestStl>
            (iSerializer, TestStl(4000), true);
    }

//      ---<<< std::set >>>---

    {
        std::cout << "        loadContainerSet() : std::set<int>" << std::endl;
        loadContainerSet<tSerializer, std::set<int>, int>
        (
            iSerializer, 100, false
        );

        std::cout << "        loadContainerSet() : std::set<TestStl>" << std::endl;
        loadContainerSet<tSerializer, std::set<TestStl>, TestStl>
        (
            iSerializer, 500, false
        );
    }

//      ---<<< std::multiset >>>---

    {
        std::cout << "        loadContainerSet() : std::multiset<int>" << std::endl;
        loadContainerSet<tSerializer, std::multiset<int>, int>
        (
            iSerializer, 100, true
        );

        std::cout << "        loadContainerSet() : std::multiset<TestStl>" << std::endl;
        loadContainerSet<tSerializer, std::multiset<TestStl>, TestStl>
        (
            iSerializer, 500, true
        );
    }

//      ---<<< std::map >>>---

    {
        std::cout << "        loadContainerMap() : std::map<int, int>" << std::endl;
        loadContainerMap<tSerializer, std::map<int, int>, int>
        (
            iSerializer, 100, false, false
        );

        std::cout << "        loadContainerMap() : std::map<TestStl, TestStl>" << std::endl;
        loadContainerMap<tSerializer, std::map<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false, false
        );

        // 被ポインタのテスト
        std::cout << "        loadContainerMap() : theolizer::MapPointee<int, int>" << std::endl;
        loadContainerMap<tSerializer, theolizer::MapPointee<int, int>, int>
        (
            iSerializer, 100, false, true
        );

        std::cout << "        loadContainerMap() : theolizer::MapPointee<TestStl, TestStl>"
            << std::endl;
        loadContainerMap<tSerializer, theolizer::MapPointee<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false, true
        );
    }

//      ---<<< std::multimap >>>---

    {
        std::cout << "        loadContainerMap() : std::multimap<int, int>" << std::endl;
        loadContainerMap<tSerializer, std::multimap<int, int>, int>
        (
            iSerializer, 100, true, false
        );

        std::cout << "        loadContainerMap() : std::multimap<TestStl, TestStl>" << std::endl;
        loadContainerMap<tSerializer, std::multimap<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, true, false
        );
    }

//      ---<<< std::unordered_set >>>---

    {
        std::cout << "        loadContainerSet() : std::unordered_set<int>" << std::endl;
        loadContainerSet<tSerializer, std::unordered_set<int>, int>
        (
            iSerializer, 100, false
        );

        std::cout << "        loadContainerSet() : std::unordered_set<TestStl>" << std::endl;
        loadContainerSet<tSerializer, std::unordered_set<TestStl>, TestStl>
        (
            iSerializer, 500, false
        );
    }

//      ---<<< std::unordered_multiset >>>---

    {
        std::cout << "        loadContainerSet() : std::unordered_multiset<int>" << std::endl;
        loadContainerSet<tSerializer, std::unordered_multiset<int>, int>
        (
            iSerializer, 100, true
        );

        std::cout << "        loadContainerSet() : std::unordered_multiset<TestStl>" << std::endl;
        loadContainerSet<tSerializer, std::unordered_multiset<TestStl>, TestStl>
        (
            iSerializer, 500, true
        );
    }

//      ---<<< std::unordered_map >>>---

    {
        std::cout << "        loadContainerMap() : std::unordered_map<int, int>" << std::endl;
        loadContainerMap<tSerializer, std::unordered_map<int, int>, int>
        (
            iSerializer, 100, false, false
        );

        std::cout << "        loadContainerMap() : std::unordered_map<TestStl, TestStl>"
            << std::endl;
        loadContainerMap<tSerializer, std::unordered_map<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false, false
        );

        // 被ポインタのテスト
        std::cout << "        loadContainerMap() : theolizer::UnorderedMapPointee<int, int>"
            << std::endl;
        loadContainerMap<tSerializer, theolizer::UnorderedMapPointee<int, int>, int>
        (
            iSerializer, 100, false, true
        );

        std::cout << "        loadContainerMap() : theolizer::UnorderedMapPointee<TestStl,TestStl>"
            << std::endl;
        loadContainerMap<tSerializer, theolizer::UnorderedMapPointee<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, false, true
        );
    }

//      ---<<< std::unordered_multimap >>>---

    {
        std::cout << "        loadContainerMap() : std::unordered_multimap<int, int>" << std::endl;
        loadContainerMap<tSerializer, std::unordered_multimap<int, int>, int>
        (
            iSerializer, 100, true, false
        );

        std::cout << "        loadContainerMap() : std::unordered_multimap<TestStl, TestStl>"
            << std::endl;
        loadContainerMap<tSerializer, std::unordered_multimap<TestStl, TestStl>, TestStl>
        (
            iSerializer, 500, true, false
        );
    }
}

INSTANTIATION_ALL(loadSupportStl);

// ***************************************************************************
//      保存（保存先指定の合成テスト）
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveSupportStlDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB&
)
{
//----------------------------------------------------------------------------
//      スマート・ポインタ
//----------------------------------------------------------------------------

    {
        std::cout << "        saveSupportStlDestinations() : std::unique_ptr" << std::endl;
        std::unique_ptr<TestStlDestinations>    aUnique{new TestStlDestinations{100}};
        THEOLIZER_PROCESS(iSerializerA, aUnique);
        THEOLIZER_PROCESS(iSerializerB, aUnique);

        std::cout << "        saveSupportStlDestinations() : std::shared_ptr" << std::endl;
        std::shared_ptr<TestStlDestinations>    aShared{new TestStlDestinations{200}};
        std::shared_ptr<TestStlDestinations>    aShared2{aShared};
        THEOLIZER_PROCESS(iSerializerA, aShared);
        THEOLIZER_PROCESS(iSerializerA, aShared2);
        THEOLIZER_PROCESS(iSerializerB, aShared);
        THEOLIZER_PROCESS(iSerializerB, aShared2);

        iSerializerA.clearTracking();
        iSerializerB.clearTracking();
    }

//----------------------------------------------------------------------------
//      コンテナ
//----------------------------------------------------------------------------

    {
        std::cout << "        saveSupportStlDestinations() : Container Fixed" << std::endl;
        saveFixed<std::array<TestStlDestinations, 3> >      (iSerializerA, iSerializerB, 100);
        saveFixed<theolizer::ArrayPointee<TestStlDestinations, 3> >
                                                            (iSerializerA, iSerializerB, 110);

        std::cout << "        saveSupportStlDestinations() : Container NoKey" << std::endl;
        saveNoKey<std::vector<TestStlDestinations> >        (iSerializerA, iSerializerB, 200);
        saveNoKey<theolizer::VectorPointee<TestStlDestinations> >(iSerializerA, iSerializerB, 210);

        saveNoKey<std::deque<TestStlDestinations> >         (iSerializerA, iSerializerB, 300);
        saveNoKey<theolizer::DequePointee<TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 310);
        saveNoKey<std::forward_list<TestStlDestinations> >  (iSerializerA, iSerializerB, 400);
        saveNoKey<theolizer::ForwardListPointee<TestStlDestinations> >
                                                            (iSerializerA,iSerializerB,410);
        saveNoKey<std::list<TestStlDestinations> >          (iSerializerA, iSerializerB, 500);
        saveNoKey<theolizer::ListPointee<TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 510);

        std::cout << "        saveSupportStlDestinations() : Container Key" << std::endl;
        saveKey<std::map<std::string, TestStlDestinations> >(iSerializerA, iSerializerB, 600);
        saveKey<theolizer::MapPointee<std::string, TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 610);

        saveKey<std::unordered_map<std::string, TestStlDestinations> >
                                                            (iSerializerA,iSerializerB,700);
        saveKey<theolizer::UnorderedMapPointee<std::string, TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 710);
    }
}

INSTANTIATION_DESTINATIONS(saveSupportStlDestinations);

// ***************************************************************************
//      回復（保存先指定の合成テスト）
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadSupportStlDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB&
)
{
//----------------------------------------------------------------------------
//      スマート・ポインタ
//----------------------------------------------------------------------------

    {
        std::cout << "        loadSupportStlDestinations() : std::unique_ptr" << std::endl;
        std::unique_ptr<TestStlDestinations>    aUnique{};

        THEOLIZER_PROCESS(iSerializerA, aUnique);
        THEOLIZER_EQUAL(*(aUnique.get()), TestStlDestinations(100, 0));
        THEOLIZER_PROCESS(iSerializerB, aUnique);
        THEOLIZER_EQUAL(*(aUnique.get()), TestStlDestinations(100));

        std::cout << "        loadSupportStlDestinations() : std::shared_ptr" << std::endl;
        std::shared_ptr<TestStlDestinations>    aShared{};
        std::shared_ptr<TestStlDestinations>    aShared2{};
        THEOLIZER_PROCESS(iSerializerA, aShared);
        THEOLIZER_EQUAL(*(aShared.get()), TestStlDestinations(200, 0));
        THEOLIZER_PROCESS(iSerializerB, aShared);
        THEOLIZER_EQUAL(*(aShared.get()), TestStlDestinations(200));

        THEOLIZER_PROCESS(iSerializerA, aShared2);
        THEOLIZER_EQUAL_PTR(aShared.get(), aShared2.get());
        THEOLIZER_PROCESS(iSerializerB, aShared2);
        THEOLIZER_EQUAL_PTR(aShared.get(), aShared2.get());

        iSerializerA.clearTracking();
        iSerializerB.clearTracking();
    }

//----------------------------------------------------------------------------
//      コンテナ
//----------------------------------------------------------------------------

    {
        std::cout << "        loadSupportStlDestinations() : Container Fixed" << std::endl;
        loadFixed<std::array<TestStlDestinations, 3> >      (iSerializerA, iSerializerB, 100);
        loadFixed<theolizer::ArrayPointee<TestStlDestinations, 3> >
                                                            (iSerializerA, iSerializerB, 110);

        std::cout << "        loadSupportStlDestinations() : Container NoKey" << std::endl;
        loadNoKey<std::vector<TestStlDestinations> >        (iSerializerA, iSerializerB, 200);
        loadNoKey<theolizer::VectorPointee<TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 210);
        loadNoKey<std::deque<TestStlDestinations> >         (iSerializerA, iSerializerB, 300);
        loadNoKey<theolizer::DequePointee<TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 310);
        loadNoKey<std::forward_list<TestStlDestinations> >  (iSerializerA, iSerializerB, 400);
        loadNoKey<theolizer::ForwardListPointee<TestStlDestinations> >
                                                            (iSerializerA,iSerializerB,410);
        loadNoKey<std::list<TestStlDestinations> >          (iSerializerA, iSerializerB, 500);
        loadNoKey<theolizer::ListPointee<TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 510);

        std::cout << "        loadSupportStlDestinations() : Container Key" << std::endl;
        loadKey<std::map<std::string, TestStlDestinations> >(iSerializerA, iSerializerB, 600);
        loadKey<theolizer::MapPointee<std::string, TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 610);

        loadKey<std::unordered_map<std::string, TestStlDestinations> >
                                                            (iSerializerA,iSerializerB,700);
        loadKey<theolizer::UnorderedMapPointee<std::string, TestStlDestinations> >
                                                            (iSerializerA, iSerializerB, 710);
    }
}

INSTANTIATION_DESTINATIONS(loadSupportStlDestinations);

#endif  // ENABLE_SUPPORT_STL_TEST
