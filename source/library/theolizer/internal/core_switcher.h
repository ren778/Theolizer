﻿//############################################################################
//      Theolizerライブラリのコア部(型による分岐処理部)
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

#if !defined(THEOLIZER_INTERNAL_CORE_SWITCHER_H)
#define THEOLIZER_INTERNAL_CORE_SWITCHER_H

#include <initializer_list>

#include "core_serializer.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4100 4127 4189)
#endif

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      型管理
//############################################################################

// ***************************************************************************
//      Switcher(管理する型毎に特殊化する)
//          シリアライズ／デシリアライズにおける型毎の分岐を実現する
// ***************************************************************************

//----------------------------------------------------------------------------
//      プライマリーSwitcher(エラー)
//----------------------------------------------------------------------------

template<class tBaseSerializer, typename tType, bool tIsRegister,
         TrackingMode tTrackingMode, class tEnable>
struct Switcher
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tType& iInstance)
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
//std::cout << "save(" << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ") illegal!!\n";
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tType& iInstance)
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
//std::cout << "load(" << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ") illegal!!\n";
    }
};

//----------------------------------------------------------------------------
//      ポインタ型(オブジェクト追跡)用Switcher
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_NON_FOUND_DERIVED(dBase)                         \
    THEOLIZER_INTERNAL_DATA_ERROR(                                          \
        u8"Can not find the derived class for %1%."                         \
        " Please specify the derived class by THEOLIZER_REGISTER_CLASS((derived class));",\
        dBase.getCName())

//      ---<<< オブジェクト実体の保存 >>>---

// ClassType以外
template<class tBaseSerializer, typename tType, class tEnalbe=void>
struct SavePointer
{
    static void save(tBaseSerializer& iSerializer, tType*& iPointer)
    {
        Switcher<tBaseSerializer, tType, false, etmDefault>::save(iSerializer, *iPointer);
    }
};

// 侵入型
template<class tBaseSerializer, typename tClassType>
struct SavePointer<tBaseSerializer, tClassType, EnableIf<IsIntrusive<tClassType>::value> >
{
    static void save(tBaseSerializer& iSerializer, tClassType*& iPointer)
    {
        tClassType* aPointer=iPointer;
        auto& aClassTypeInfo=ClassTypeInfo<tClassType>::getInstance();
        bool ret=aClassTypeInfo.saveTypeInstance
        (
            iSerializer,
            iPointer,
            std::type_index(typeid(*aPointer))
        );
        if (!ret)
        {
            THEOLIZER_INTERNAL_NON_FOUND_DERIVED(aClassTypeInfo);
        }
    }
};

// 非侵入型
template<class tBaseSerializer, typename tClassType>
struct SavePointer
<
    tBaseSerializer,
    tClassType,
    EnableIf<IsNonIntrusive<tClassType>::value>
>
{
    static void save(tBaseSerializer& iSerializer, tClassType*& iPointer)
    {
        typedef TheolizerNonIntrusive<tClassType> NonIntrusiveType;
        auto& aClassTypeInfo=ClassTypeInfo<NonIntrusiveType>::getInstance();
        bool ret=aClassTypeInfo.saveTypeInstance
        (
            iSerializer,
            reinterpret_cast<NonIntrusiveType*&>(iPointer),
            std::type_index(typeid(*iPointer))
        );
        if (!ret)
        {
            THEOLIZER_INTERNAL_NON_FOUND_DERIVED(aClassTypeInfo);
        }
    }
};

//      ---<<< オブジェクト実体の回復 >>>---

// ClassType以外
template<class tBaseSerializer, typename tType, class tEnalbe=void>
struct LoadPointer
{
    static void load(tBaseSerializer& iSerializer, tType*& oPointer)
    {
        // 未回復の時、nullptrなら領域獲得
        //  (配列型へのポインタに対応するためreinterpret_castする)
        if (!oPointer) oPointer = reinterpret_cast<tType*>(new tType());

        // オブジェクトを回復する
        Switcher<tBaseSerializer, tType, false, etmDefault
                >::load(iSerializer, *oPointer);
    }
};

// 侵入型
template<class tBaseSerializer, typename tClassType>
struct LoadPointer<tBaseSerializer, tClassType, EnableIf<IsIntrusive<tClassType>::value> >
{
    static void load(tBaseSerializer& iSerializer, tClassType*& oPointer)
    {
        // オブジェクトを回復する
        auto& aClassTypeInfo=ClassTypeInfo<tClassType>::getInstance();
        bool ret;
        // 通常のデータ交換
        if (iSerializer.mCheckMode != CheckMode::InMemory)
        {
            ret=aClassTypeInfo.loadTypeInstance
            (
                iSerializer,
                oPointer,
                iSerializer.getProgramTypeIndex()
            );
        }
        // メモリ内のデータ交換(TypeIndexが変化しない)
        else
        {
            size_t aTypeIndex=0;
            iSerializer.loadControl(aTypeIndex);
            TypeIndexList aTypeIndexList;
            aTypeIndexList.emplace_back(aTypeIndex);
            ret=aClassTypeInfo.loadTypeInstance
            (
                iSerializer,
                oPointer,
                aTypeIndexList
            );
        }
        if (!ret)
        {
            THEOLIZER_INTERNAL_NON_FOUND_DERIVED(aClassTypeInfo);
        }
    }
};

// 非侵入型
template<class tBaseSerializer, typename tClassType>
struct LoadPointer<tBaseSerializer, tClassType, EnableIf<IsNonIntrusive<tClassType>::value> >
{
    static void load(tBaseSerializer& iSerializer, tClassType*& oPointer)
    {
        // オブジェクトを回復する
        typedef TheolizerNonIntrusive<tClassType> NonIntrusiveType;
        auto& aClassTypeInfo=ClassTypeInfo<NonIntrusiveType>::getInstance();
        bool ret;
        // 通常のデータ交換
        if (iSerializer.mCheckMode != CheckMode::InMemory)
        {
            ret=aClassTypeInfo.loadTypeInstance
            (
                iSerializer,
                reinterpret_cast<NonIntrusiveType*&>(oPointer),
                iSerializer.getProgramTypeIndex()
            );
        }
        // メモリ内のデータ交換(TypeIndexが変化しない)
        else
        {
            size_t aTypeIndex=0;
            iSerializer.loadControl(aTypeIndex);
            TypeIndexList aTypeIndexList;
            aTypeIndexList.emplace_back(aTypeIndex);
            ret=aClassTypeInfo.loadTypeInstance
            (
                iSerializer,
                reinterpret_cast<NonIntrusiveType*&>(oPointer),
                aTypeIndexList
            );
        }
        if (!ret)
        {
            THEOLIZER_INTERNAL_NON_FOUND_DERIVED(aClassTypeInfo);
        }
    }
};

//----------------------------------------------------------------------------
//      ポインタ型用Switcher本体
//----------------------------------------------------------------------------

//      ---<<< オーナー・ポインタのdelete >>>---

template
<
    typename tPointerType,
    TrackingMode tTrackingMode,
    THEOLIZER_INTERNAL_OVERLOAD((tTrackingMode != etmOwner))
>
void deletePointer(tPointerType iPointer)
{
}

template
<
    typename tPointerType,
    TrackingMode tTrackingMode,
    THEOLIZER_INTERNAL_OVERLOAD
    (
           (tTrackingMode == etmOwner)
        && (!std::is_array<typename std::remove_pointer<tPointerType>::type>::value)
    )
>
void deletePointer(tPointerType iPointer)
{
    delete iPointer;
}

template
<
    typename tPointerType,
    TrackingMode tTrackingMode,
    THEOLIZER_INTERNAL_OVERLOAD
    (
           (tTrackingMode == etmOwner)
        && (std::is_array<typename std::remove_pointer<tPointerType>::type>::value)
    )
>
void deletePointer(tPointerType iPointer)
{
    delete[] iPointer;
}

//      ---<<< 本体 >>>---

template<class tBaseSerializer,typename tPointerType,bool tIsRegister,TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tPointerType,
    tIsRegister,
    tTrackingMode,
    EnableIf<std::is_pointer<tPointerType>::value>
>
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tPointerType& iPointer)
    {
        // const外し
        typedef typename RemoveCV<tPointerType>::type           PointerType;
        // pointer外し
        typedef typename std::remove_pointer<PointerType>::type PointeeType;

        bool aIsSaved;
//std::cout << "Switcher(Pointer) " << THEOLIZER_INTERNAL_TYPE_NAME(tPointerType)
//          << " " << THEOLIZER_INTERNAL_TYPE_NAME_VAR(iPointer)
//          << " " << typeid(iPointer).name() << "\n";
        SerializeInfo& aSerializeInfo=
            iSerializer.registerObject
            (
                const_cast<PointerType&>(iPointer),
                typeid(PointerType&),
                tTrackingMode,
                &aIsSaved
            );

        // ポイント先の型のTypeIndex取り出し
        std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
        if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
        {
            aTypeIndex = theolizer::internal::getTypeIndex<PointeeType>();
        }

        // 追跡指定がないなら、オブジェクトIDを保存するのみ
        if (tTrackingMode == etmDefault)
        {
            iSerializer.saveObjectId(aSerializeInfo.mObjectId, aTypeIndex);
        }

        // 追跡指定有り
        else
        {
            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreSaveStructure    aAutoRestoreSaveStructure
                (
                    iSerializer,
                    emOrder,
                    (iSerializer.mRefProcessing)?Structure::Reference:Structure::OwnerPointer,
                    aTypeIndex,
                    aSerializeInfo.mObjectId,
                    true
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                SavePointer<tBaseSerializer,
                            PointeeType
                           >::save(iSerializer, const_cast<PointerType&>(iPointer));
                aSerializeInfo.mStatus=etsProcessed;
            }
        }
        iSerializer.mRequireClearTracking=true;
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tPointerType& oPointer)
    {
        // const外し
        typedef typename RemoveCV<tPointerType>::type   PointerType;
        // pointer外し
        typedef typename std::remove_pointer<PointerType>::type PointeeType;

        // ポイント先の型のTypeIndex取り出し
        std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
        if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
        {
            aTypeIndex = theolizer::internal::getTypeIndex<PointeeType>();
        }

        // 追跡指定なし
        if (tTrackingMode == etmDefault)
        {
            // オブジェクトID回復
            size_t aObjectId;
            iSerializer.loadObjectId(aObjectId, aTypeIndex);
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(const_cast<PointerType&>(oPointer)),
                typeid(PointerType&),
                tTrackingMode
            );
        }

        // 追跡指定有り
        else
        {
            size_t aObjectId;

            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreLoadStructure aAutoRestoreLoadStructure
                (
                    iSerializer,
                    emOrder,
                    (iSerializer.mRefProcessing)?Structure::Reference:Structure::OwnerPointer,
                    aTypeIndex,
                    &aObjectId
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            void* aPointer;
            bool aIsLoaded=iSerializer.isLoadedObject(aObjectId, aPointer);
            if (!aIsLoaded)
            {
                if (!iSerializer.readPreElement())
                {
                    THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
                }
                LoadPointer
                <
                    tBaseSerializer,
                    typename std::remove_pointer<PointerType>::type
                >::load(iSerializer, const_cast<PointerType&>(oPointer));
            }
            // オブジェクトのアドレス回復
            tPointerType aPointerBak=oPointer;
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(const_cast<PointerType&>(oPointer)),
                typeid(oPointer),
                tTrackingMode
            );

            // オーナー・ポインタが書き換わっていたら解放する
            if (aPointerBak != oPointer)
            {
                deletePointer<tPointerType, tTrackingMode>(aPointerBak);
            }
        }
        iSerializer.mRequireClearTracking=true;
    }
};

//----------------------------------------------------------------------------
//      トップレベル生配列用Switcher本体
//----------------------------------------------------------------------------

template<class tBaseSerializer, typename tArrayType, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tArrayType,
    tIsRegister,
    tTrackingMode,
    EnableIf<std::is_array<tArrayType>::value>
>
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tArrayType& iArray)
    {
        iSerializer.template saveArray<tBaseSerializer, tArrayType, tTrackingMode>(iArray);
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tArrayType& oArray)
    {
        iSerializer.template loadArray<tBaseSerializer, tArrayType, tTrackingMode>(oArray);
    }
};

//----------------------------------------------------------------------------
//      生配列用TheolizerVersion用Switcher本体
//----------------------------------------------------------------------------

template<class tBaseSerializer, class tVersionType, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tVersionType,
    tIsRegister,
    tTrackingMode,
    typename tVersionType::TheolizerArrayVersion
>
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tVersionType& iArray)
    {
        iSerializer.template saveArray
        <
            tBaseSerializer,
            typename tVersionType::VersionArray,
            tTrackingMode
        >(iArray.get());
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tVersionType& oArray)
    {
        iSerializer.template loadArray
        <
            tBaseSerializer,
            typename tVersionType::VersionArray,
            tTrackingMode
        >(oArray.get());
    }
};

//----------------------------------------------------------------------------
//      生配列用基本部用Switcher本体
//----------------------------------------------------------------------------

template<class tBaseSerializer, class tVersionType, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tVersionType,
    tIsRegister,
    tTrackingMode,
    typename tVersionType::TheolizerArrayUnderlying
>
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tVersionType& iArray)
    {
        Switcher<tBaseSerializer,
                 typename tVersionType::UnderlyingType,
                 false,
                 tTrackingMode
                >::save(iSerializer, iArray.get());
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tVersionType& oArray)
    {
        Switcher<tBaseSerializer,
                 typename tVersionType::UnderlyingType,
                 false,
                 tTrackingMode
                >::load(iSerializer, oArray.get());
    }
};

//----------------------------------------------------------------------------
//      侵入型用Switcher本体
//----------------------------------------------------------------------------

template<class tBaseSerializer, class tTargetClass, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tTargetClass,
    tIsRegister,
    tTrackingMode,
    EnableIf<IsIntrusive<tTargetClass>::value && !IsTheolizerVersion<tTargetClass>::value>
>
{
    typedef tTargetClass IntrusiveType;

    // 保存
    static void save(tBaseSerializer& iSerializer, tTargetClass& iInstance)
    {
        auto& aClassTypeInfo = ClassTypeInfo<IntrusiveType>::getInstance();

#ifndef THEOLIZER_INTERNAL_DISABLE_NULL_REFERENCE
        if (!&iInstance)
        {
            THEOLIZER_INTERNAL_WRONG_USING(
                "This reference(%1%) is nullptr.",
                aClassTypeInfo.getCName());
        }
#endif

        // 参照のポリモーフィズム対応
        //  クラス内の基底クラスのシリアライズ中でない時に、
        //  処理中の型と動的な型が不一致なら、ポリモーフィズム処理を適用する。
        if ((!iSerializer.mBaseProcessing)
         && (typeid(tTargetClass) != typeid(iInstance)))
        {
//std::cout << "Derived!! " << typeid(tTargetClass).name()
//          << ", " << typeid(iInstance).name() << "\n";
            BaseSerializer::AutoRefProcessing aAutoRefProcessing(iSerializer);
            tTargetClass* aInstancePtr = &iInstance;

            // オブジェクト追跡中でないなら、直接保存
            if (tTrackingMode == etmDefault)
            {
                // 開始／終了マーク処理
                BaseSerializer::AutoRestoreSave aAutoRestoreSave(iSerializer, emOrder, true);
                iSerializer.writePreElement();
                SavePointer<tBaseSerializer,
                            tTargetClass
                           >::save(iSerializer, aInstancePtr);
            }
            // オブジェクト追跡中なら、ポインタ保存（オブジェクト追跡する）
            else
            {
                Switcher
                <
                    tBaseSerializer,
                    typename std::add_pointer<tTargetClass>::type,
                    tIsRegister,
                    etmOwner
                >::save(iSerializer, aInstancePtr);
            }
    return;
        }

        // 通常処理
        unsigned aVersionNo=iSerializer.mVersionNoList.at(aClassTypeInfo.mTypeIndex);
        IntrusiveType* aInstancePtr = &iInstance;

        // 追跡モード修正(クラスのオブジェクト追跡中、かつ、基底クラス処理中なら追跡する)
        TrackingMode aTrackingMode=tTrackingMode;
        if ((aTrackingMode == etmDefault)
         && (iSerializer.mClassTracking && iSerializer.mBaseProcessing))
        {
            aTrackingMode=etmPointee;
        }
        if (aTrackingMode == etmDefault)
        {
            IntrusiveType::Theolizer::saveClass(iSerializer, aInstancePtr, aVersionNo);
        }
        else
        {
            bool aIsSaved;
//std::cout << "Switcher(Intrusive) " << THEOLIZER_INTERNAL_TYPE_NAME(IntrusiveType) << "\n";
            SerializeInfo& aSerializeInfo=
                iSerializer.registerObject
                (
                    &iInstance,
                    typeid(&iInstance),
                    aTrackingMode,
                    &aIsSaved
                );

            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tTargetClass>();
            }

            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreSaveStructure    aAutoRestoreSaveStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    aSerializeInfo.mObjectId,
                    true
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                IntrusiveType::Theolizer::saveClass(iSerializer, aInstancePtr, aVersionNo);
            }
        }
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tTargetClass& oInstance)
    {
        auto& aClassTypeInfo = ClassTypeInfo<IntrusiveType>::getInstance();

#ifndef THEOLIZER_INTERNAL_DISABLE_NULL_REFERENCE
        if (!&oInstance)
        {
            THEOLIZER_INTERNAL_WRONG_USING(
                "This reference(%1%) is nullptr.",
                aClassTypeInfo.getCName());
        }
#endif

        // 参照のポリモーフィズム対応
        //  クラス内の基底クラスのシリアライズ中でない時に、
        //  処理中の型と動的な型が不一致なら、ポリモーフィズム処理を適用する。
        if ((!iSerializer.mBaseProcessing)
         && (typeid(tTargetClass) != typeid(oInstance)))
        {
//std::cout << "Derived!! " << typeid(tTargetClass).name()
//          << ", " << typeid(oInstance).name() << "\n";
            BaseSerializer::AutoRefProcessing aAutoRefProcessing(iSerializer);
            tTargetClass* aInstancePtr = &oInstance;

            // オブジェクト追跡中でないなら、直接保存
            if (tTrackingMode == etmDefault)
            {
                // 開始／終了マーク処理
                BaseSerializer::AutoRestoreLoad aAutoRestoreLoad(iSerializer);
                if (!iSerializer.readPreElement())
                {
                    THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
                }
                LoadPointer<tBaseSerializer,
                            tTargetClass
                           >::load(iSerializer, aInstancePtr);
            }
            // オブジェクト追跡中なら、ポインタ保存（オブジェクト追跡する）
            else
            {
                Switcher
                <
                    tBaseSerializer,
                    typename std::add_pointer<tTargetClass>::type,
                    tIsRegister,
                    etmOwner
                >::load(iSerializer, aInstancePtr);
            }
    return;
        }

        // 通常処理
        unsigned aVersionNo=iSerializer.mVersionNoList.at(aClassTypeInfo.mTypeIndex);
        IntrusiveType* aInstancePtr = &oInstance;

        // 追跡モード修正(クラスのオブジェクト追跡中、かつ、基底クラス処理中なら追跡する)
        TrackingMode aTrackingMode=tTrackingMode;
        if ((aTrackingMode == etmDefault)
         && (iSerializer.mClassTracking && iSerializer.mBaseProcessing))
        {
            aTrackingMode=etmPointee;
        }
        if (aTrackingMode == etmDefault)
        {
            IntrusiveType::Theolizer::loadClass(iSerializer, aInstancePtr, aVersionNo);
        }
        else
        {
            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tTargetClass>();
            }

            // 開始／終了マーク処理
            size_t aObjectId=kInvalidSize;
            BaseSerializer::AutoRestoreLoadStructure aAutoRestoreLoadStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    &aObjectId
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            bool aIsLoaded;
            IntrusiveType* aPointer=&oInstance;

            // オブジェクトのアドレス回復
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(aPointer),
                typeid(aPointer),
                aTrackingMode,
                &aIsLoaded
            );
            // 未回復の時のみ回復する
            if (!aIsLoaded)
            {
                if (!iSerializer.readPreElement())
                {
                    THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
                }
                IntrusiveType::Theolizer::loadClass(iSerializer, aInstancePtr, aVersionNo);
            }
        }
    }
};

//----------------------------------------------------------------------------
//      非侵入型用Switcher本体
//          対象クラスがconstの場合、TheolizerNonIntrusive<>が別物になるので、
//          constを外したTheolizerNonIntrusive<>の有無をチェックする
//----------------------------------------------------------------------------

template<class tBaseSerializer, class tTargetClass, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tTargetClass,
    tIsRegister,
    tTrackingMode,
    EnableIf<IsNonIntrusive<tTargetClass>::value && !IsTheolizerVersion<tTargetClass>::value>
>
{
    typedef TheolizerNonIntrusive<tTargetClass> NonIntrusiveType;
    typedef typename NonIntrusiveType::TheolizerTarget TheolizerTarget;

    // 保存
    static void save(tBaseSerializer& iSerializer, tTargetClass& iInstance)
    {
        auto& aClassTypeInfo = ClassTypeInfo<NonIntrusiveType>::getInstance();

#ifndef THEOLIZER_INTERNAL_DISABLE_NULL_REFERENCE
        if (!&iInstance)
        {
            THEOLIZER_INTERNAL_WRONG_USING(
                "This reference(%1%) is nullptr.",
                aClassTypeInfo.getCName());
        }
#endif

        // 参照のポリモーフィズム対応
        //  クラス内の基底クラスのシリアライズ中でない時に、
        //  処理中の型と動的な型が不一致なら、ポリモーフィズム処理を適用する。
        if ((!iSerializer.mBaseProcessing)
         && (typeid(tTargetClass) != typeid(iInstance)))
        {
//std::cout << "Derived!! " << typeid(tTargetClass).name()
//          << ", " << typeid(iInstance).name() << "\n";
            BaseSerializer::AutoRefProcessing aAutoRefProcessing(iSerializer);
            tTargetClass* aInstancePtr = &iInstance;

            // オブジェクト追跡中でないなら、直接保存
            if (tTrackingMode == etmDefault)
            {
                // 開始／終了マーク処理
                BaseSerializer::AutoRestoreSave aAutoRestoreSave(iSerializer, emOrder, true);
                iSerializer.writePreElement();
                SavePointer<tBaseSerializer,
                            tTargetClass
                           >::save(iSerializer, aInstancePtr);
            }
            // オブジェクト追跡中なら、ポインタ保存（オブジェクト追跡する）
            else
            {
                Switcher
                <
                    tBaseSerializer,
                    typename std::add_pointer<tTargetClass>::type,
                    tIsRegister,
                    etmOwner
                >::save(iSerializer, aInstancePtr);
            }
    return;
        }

        // 通常処理
        unsigned aVersionNo=iSerializer.mVersionNoList.at(aClassTypeInfo.mTypeIndex);
        NonIntrusiveType* aInstancePtr = static_cast<NonIntrusiveType*>(&iInstance);

        // 追跡モード修正(クラスのオブジェクト追跡中、かつ、基底クラス処理中なら追跡する)
        TrackingMode aTrackingMode=tTrackingMode;
        if ((aTrackingMode == etmDefault)
         && (iSerializer.mClassTracking && iSerializer.mBaseProcessing))
        {
            aTrackingMode=etmPointee;
        }
        if (aTrackingMode == etmDefault)
        {
            NonIntrusiveType::Theolizer::saveClass(iSerializer, aInstancePtr, aVersionNo);
        }
        else
        {
            bool aIsSaved;
//std::cout << "Switcher(NonIntrusive) "<<THEOLIZER_INTERNAL_TYPE_NAME(NonIntrusiveType) << "\n";
            SerializeInfo& aSerializeInfo=
                iSerializer.registerObject
                (
                    aInstancePtr,
                    typeid(typename std::add_pointer<TheolizerTarget>::type),
                    aTrackingMode,
                    &aIsSaved
                );

            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tTargetClass>();
            }

            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreSaveStructure    aAutoRestoreSaveStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    aSerializeInfo.mObjectId,
                    true
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                NonIntrusiveType::Theolizer::saveClass(iSerializer, aInstancePtr, aVersionNo);
            }
        }
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tTargetClass& oInstance)
    {
        auto& aClassTypeInfo = ClassTypeInfo<NonIntrusiveType>::getInstance();

#ifndef THEOLIZER_INTERNAL_DISABLE_NULL_REFERENCE
        if (!&oInstance)
        {
            THEOLIZER_INTERNAL_WRONG_USING(
                "This reference(%1%) is nullptr.",
                aClassTypeInfo.getCName());
        }
#endif

        // 参照のポリモーフィズム対応
        //  クラス内の基底クラスのシリアライズ中でない時に、
        //  処理中の型と動的な型が不一致なら、ポリモーフィズム処理を適用する。
        if ((!iSerializer.mBaseProcessing)
         && (typeid(tTargetClass) != typeid(oInstance)))
        {
//std::cout << "Derived!! " << typeid(tTargetClass).name()
//          << ", " << typeid(oInstance).name() << "\n";
            BaseSerializer::AutoRefProcessing aAutoRefProcessing(iSerializer);
            tTargetClass* aInstancePtr = &oInstance;

            // オブジェクト追跡中でないなら、直接保存
            if (tTrackingMode == etmDefault)
            {
                // 開始／終了マーク処理
                BaseSerializer::AutoRestoreLoad aAutoRestoreLoad(iSerializer);
                if (!iSerializer.readPreElement())
                {
                    THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
                }
                LoadPointer<tBaseSerializer,
                            tTargetClass
                           >::load(iSerializer, aInstancePtr);
            }
            // オブジェクト追跡中なら、ポインタ保存（オブジェクト追跡する）
            else
            {
                Switcher
                <
                    tBaseSerializer,
                    typename std::add_pointer<tTargetClass>::type,
                    tIsRegister,
                    etmOwner
                >::load(iSerializer, aInstancePtr);
            }
    return;
        }

        // 通常処理
        unsigned aVersionNo=iSerializer.mVersionNoList.at(aClassTypeInfo.mTypeIndex);
        NonIntrusiveType* aInstancePtr = static_cast<NonIntrusiveType*>(&oInstance);

        // 追跡モード修正(クラスのオブジェクト追跡中、かつ、基底クラス処理中なら追跡する)
        TrackingMode aTrackingMode=tTrackingMode;
        if ((aTrackingMode == etmDefault)
         && (iSerializer.mClassTracking && iSerializer.mBaseProcessing))
        {
            aTrackingMode=etmPointee;
        }
        if (aTrackingMode == etmDefault)
        {
            NonIntrusiveType::Theolizer::loadClass(iSerializer, aInstancePtr, aVersionNo);
        }
        else
        {
            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tTargetClass>();
            }

            // 開始／終了マーク処理
            size_t aObjectId=kInvalidSize;
            BaseSerializer::AutoRestoreLoadStructure aAutoRestoreLoadStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    &aObjectId
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            bool aIsLoaded;
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(aInstancePtr),
                typeid(aInstancePtr),
                aTrackingMode,
                &aIsLoaded
            );
            // 未回復の時のみ回復する
            if (!aIsLoaded)
            {
                iSerializer.readPreElement();
                NonIntrusiveType::Theolizer::loadClass(iSerializer, aInstancePtr, aVersionNo);
            }
        }
    }
};

//----------------------------------------------------------------------------
//      侵入型と非侵入型のTheolizerVersion用Switcher本体
//----------------------------------------------------------------------------

template<class tBaseSerializer, class tVersionType, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tVersionType,
    tIsRegister,
    tTrackingMode,
    EnableIf
    <
           IsTheolizerVersion<tVersionType>::value
        && !tVersionType::Theolizer::kIsEnum
        && !tVersionType::Theolizer::kIsArray
    >
>
{
    typedef typename tVersionType::TheolizerClass   TheolizerClass;
    typedef typename tVersionType::TheolizerTarget  TheolizerTarget;

    // 保存
    static void save(tBaseSerializer& iSerializer, tVersionType& iInstance)
    {
        // 追跡モード修正(クラスのオブジェクト追跡中、かつ、基底クラス処理中なら追跡する)
        TrackingMode aTrackingMode=tTrackingMode;
        if ((aTrackingMode == etmDefault)
         && (iSerializer.mClassTracking && iSerializer.mBaseProcessing))
        {
            aTrackingMode=etmPointee;
        }
        if (aTrackingMode == etmDefault)
        {
            iInstance.saveClass(iSerializer, tVersionType::Theolizer::kVersionNo);
        }
        else
        {
            bool aIsSaved;
//std::cout << "Switcher(TheolizerVersion) "<<THEOLIZER_INTERNAL_TYPE_NAME(tVersionType) << "\n";
            SerializeInfo& aSerializeInfo=
                iSerializer.registerObject
                (
                    iInstance.mTheolizerSpecials.mTarget,
                    typeid(typename std::add_pointer<TheolizerTarget>::type),
                    aTrackingMode,
                    &aIsSaved
                );

            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<TheolizerTarget>();
            }

            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreSaveStructure    aAutoRestoreSaveStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    aSerializeInfo.mObjectId,
                    true
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                iInstance.saveClass(iSerializer, tVersionType::Theolizer::kVersionNo);
            }
        }
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tVersionType& oInstance)
    {
        // 追跡モード修正(クラスのオブジェクト追跡中、かつ、基底クラス処理中なら追跡する)
        TrackingMode aTrackingMode=tTrackingMode;
        if ((aTrackingMode == etmDefault)
         && (iSerializer.mClassTracking && iSerializer.mBaseProcessing))
        {
            aTrackingMode=etmPointee;
        }
        if (aTrackingMode == etmDefault)
        {
            oInstance.loadClass(iSerializer, tVersionType::Theolizer::kVersionNo);
        }
        else
        {
            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<TheolizerTarget>();
            }

            // 開始／終了マーク処理
            size_t aObjectId=kInvalidSize;
            BaseSerializer::AutoRestoreLoadStructure aAutoRestoreLoadStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    &aObjectId
                );

            // クラス・インスタンスのオブジェクト追跡中
            BaseSerializer::AutoClassTracking aAutoClassTracking(iSerializer);

            bool aIsLoaded;
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(oInstance.mTheolizerSpecials.mTarget),
                typeid(oInstance.mTheolizerSpecials.mTarget),
                aTrackingMode,
                &aIsLoaded
            );
            // 未回復の時のみ回復する
            if (!aIsLoaded)
            {
                if (!iSerializer.readPreElement()) {
                    THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
                }
                oInstance.loadClass(iSerializer, tVersionType::Theolizer::kVersionNo);
            }
        }
    }
};

//----------------------------------------------------------------------------
//      手動型基底クラス用Switcher
//----------------------------------------------------------------------------

template<class tBaseSerializer, class tBaseType, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tBaseType,
    tIsRegister,
    tTrackingMode,
    EnableIf<IsTheolizerBase<tBaseType>::value>
>
{
    typedef typename tBaseType::Target Target;

    // 保存
    static void save(tBaseSerializer& iSerializer, tBaseType& iInstance)
    {
        Switcher<tBaseSerializer,
                 Target,
                 tTrackingMode
                >::save(iSerializer, static_cast<Target&>(iInstance));
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tBaseType& oInstance)
    {
        Switcher<tBaseSerializer,
                 Target,
                 tTrackingMode
                >::load(iSerializer, static_cast<Target&>(oInstance));
    }
};

//----------------------------------------------------------------------------
//      EnumType用Switcher
//----------------------------------------------------------------------------

//      ---<<< EnumType用Switcher本体 >>>---

template<class tBaseSerializer, typename tEnumType, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tEnumType,
    tIsRegister,
    tTrackingMode,
    EnableIf<std::is_enum<tEnumType>::value>
>
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tEnumType& iInstance)
    {
        unsigned aVersionNo=iSerializer.mVersionNoList.at(
            EnumTypeInfo<tEnumType>::getInstance().mTypeIndex);

        if (tTrackingMode == etmDefault)
        {
            TheolizerNonIntrusive<tEnumType>::Theolizer::
                saveEnum(iSerializer, iInstance, aVersionNo);
        }
        else
        {
            bool aIsSaved;
//std::cout << "Switcher(Enum) " << THEOLIZER_INTERNAL_TYPE_NAME(tEnumType) << "\n";
            SerializeInfo& aSerializeInfo=
                iSerializer.registerObject
                (
                    &iInstance,
                    typeid(&iInstance),
                    tTrackingMode,
                    &aIsSaved
                );

            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tEnumType>();
            }

            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreSaveStructure    aAutoRestoreSaveStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    aSerializeInfo.mObjectId,
                    true
                );

            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                TheolizerNonIntrusive<tEnumType>::Theolizer::
                    saveEnum(iSerializer, iInstance, aVersionNo);
            }
        }
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tEnumType& oInstance)
    {
        unsigned aVersionNo=iSerializer.mVersionNoList.at(
            EnumTypeInfo<tEnumType>::getInstance().mTypeIndex);

        if (tTrackingMode == etmDefault)
        {
            TheolizerNonIntrusive<tEnumType>::Theolizer::
                loadEnum(iSerializer,oInstance,aVersionNo);
        }
        else
        {
            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tEnumType>();
            }

            // 開始／終了マーク処理
            size_t aObjectId=kInvalidSize;
            BaseSerializer::AutoRestoreLoadStructure aAutoRestoreLoadStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    &aObjectId
                );

            bool aIsLoaded;
            tEnumType* aPointer=&oInstance;
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(aPointer),
                typeid(aPointer),
                tTrackingMode,
                &aIsLoaded
            );
            // 未回復の時のみ回復する
            if (!aIsLoaded)
            {
                iSerializer.readPreElement();
                TheolizerNonIntrusive<tEnumType>::Theolizer::
                    loadEnum(iSerializer, oInstance, aVersionNo);
            }
        }
    }
};

//----------------------------------------------------------------------------
//      EnumType用TheolizerVersion用Switcher
//----------------------------------------------------------------------------

//      ---<<< EnumType用TheolizerVersion用Switcher本体 >>>---

template<class tBaseSerializer, class tVersionType, bool tIsRegister, TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tVersionType,
    tIsRegister,
    tTrackingMode,
    typename tVersionType::TheolizerEnumVersion
>
{
    typedef typename tVersionType::TheolizerTarget  TheolizerTarget;

    // 保存
    static void save(tBaseSerializer& iSerializer, tVersionType& iInstance)
    {
        if (tTrackingMode == etmDefault)
        {
            iInstance.saveEnum(iSerializer, tVersionType::Theolizer::kLastVersionNo);
        }
        else
        {
            bool aIsSaved;
//std::cout << "Switcher(EnumVersion) " << THEOLIZER_INTERNAL_TYPE_NAME(tVersionType) << "\n";
            SerializeInfo& aSerializeInfo=
                iSerializer.registerObject
                (
                    iInstance.mTarget,
                    typeid(iInstance.mTarget),
                    tTrackingMode,
                    &aIsSaved
                );

            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<TheolizerTarget>();
            }

            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreSaveStructure    aAutoRestoreSaveStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    aSerializeInfo.mObjectId,
                    true
                );

            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                iInstance.saveEnum(iSerializer, tVersionType::Theolizer::kLastVersionNo);
            }
        }
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tVersionType& oInstance)
    {
        if (tTrackingMode == etmDefault)
        {
            oInstance.loadEnum(iSerializer, tVersionType::Theolizer::kLastVersionNo);
        }
        else
        {
            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<TheolizerTarget>();
            }

            // 開始／終了マーク処理
            size_t aObjectId=kInvalidSize;
            BaseSerializer::AutoRestoreLoadStructure aAutoRestoreLoadStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    &aObjectId
                );

            bool aIsLoaded;
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(oInstance.mTarget),
                typeid(oInstance.mTarget),
                tTrackingMode,
                &aIsLoaded
            );
            // 未回復の時のみ回復する
            if (!aIsLoaded)
            {
                iSerializer.readPreElement();
                oInstance.loadEnum(iSerializer, tVersionType::Theolizer::kLastVersionNo);
            }
        }
    }
};

//----------------------------------------------------------------------------
//      プリミティブ型用Switcher
//----------------------------------------------------------------------------

//      ---<<< Primitive用Switcher本体 >>>---
//      プリミティブへの参照の場合でも、save/loadPrimitiveにマッチする。
//      なので、参照の場合はここにマッチしないように条件を追加している。

template<class tBaseSerializer,typename tPrimitiveType,bool tIsRegister,TrackingMode tTrackingMode>
struct Switcher
<
    tBaseSerializer,
    tPrimitiveType,
    tIsRegister,
    tTrackingMode,
    EnableIf<IsPrimitive<tPrimitiveType>::value>
>
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tPrimitiveType& iInstance)
    {
        if (tTrackingMode == etmDefault)
        {
            iSerializer.savePrimitive(iInstance);
        }
        else
        {
            bool aIsSaved;
//std::cout << "Switcher(Primitive) " << THEOLIZER_INTERNAL_TYPE_NAME(tPrimitiveType) << "\n";
            SerializeInfo& aSerializeInfo=
                iSerializer.registerObject
                (
                    &iInstance,
                    typeid(&iInstance),
                    tTrackingMode,
                    &aIsSaved
                );

            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tPrimitiveType>();
            }

            // 開始／終了マーク処理
            BaseSerializer::AutoRestoreSaveStructure    aAutoRestoreSaveStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    aSerializeInfo.mObjectId,
                    true
                );

            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                iSerializer.savePrimitive(iInstance);
            }
        }
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tPrimitiveType& oInstance)
    {
        if (tTrackingMode == etmDefault)
        {
            iSerializer.loadPrimitive(oInstance);
        }
        else
        {
            // 型のTypeIndex取り出し
            std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
            if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
            {
                aTypeIndex = theolizer::internal::getTypeIndex<tPrimitiveType>();
            }

            // 開始／終了マーク処理
            size_t aObjectId=kInvalidSize;
            BaseSerializer::AutoRestoreLoadStructure aAutoRestoreLoadStructure
                (
                    iSerializer,
                    emOrder,
                    Structure::Pointee,
                    aTypeIndex,
                    &aObjectId
                );

            bool aIsLoaded;
            tPrimitiveType* aPointer=&oInstance;
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(aPointer),
                typeid(aPointer),
                tTrackingMode,
                &aIsLoaded
            );
            // 未回復の時のみ回復する
            if (!aIsLoaded)
            {
                iSerializer.readPreElement();
                iSerializer.loadPrimitive(oInstance);
            }
        }
    }
};

//----------------------------------------------------------------------------
//      TheolizerNonKeepStep<>用Switcher
//----------------------------------------------------------------------------

template
<
    class tBaseSerializer,
    class tTheolizerNonKeepStep,
    bool tIsRegister,
    TrackingMode tTrackingMode
>
struct Switcher
<
    tBaseSerializer,
    tTheolizerNonKeepStep,
    tIsRegister,
    tTrackingMode,
    EnableIf
    <
           IsTheolizerNonKeepStep<tTheolizerNonKeepStep>::value
    >
>
{
    // 保存
    static void save(tBaseSerializer& iSerializer, tTheolizerNonKeepStep& iInstance)
    {
        iInstance.template save<tIsRegister, tTrackingMode>(iSerializer);
    }
    // 回復
    static void load(tBaseSerializer& iSerializer, tTheolizerNonKeepStep& oInstance)
    {
        oInstance.template load<tIsRegister, tTrackingMode>(iSerializer);
    }
};

// ***************************************************************************
//      保存／回復処理入口関数
//          インスタンスはできるだけ早期にconstを外す。
//          内部処理でconstを残すと型のハンドリングがややこしいため。
// ***************************************************************************

//----------------------------------------------------------------------------
//      派生Serializer抽出(ドライバー用)
//----------------------------------------------------------------------------

template<class tMidSerializer>
struct MidSerializer
{
    static MidSerializer& getInstance()
    {
        static MidSerializer    instance;
        return instance;
    }
};

template<class tSerializer>
int registerMidSerializer()
{
    theolizer::internal::MidSerializer<typename tSerializer::MidSerializer>::getInstance();
    return 0;
}

#define THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(dSerializer)             \
    static int k##dSerializer THEOLIZER_INTERNAL_UNUSED=                    \
        theolizer::internal::registerMidSerializer<dSerializer<> >()

//----------------------------------------------------------------------------
//      トップ・レベル／非トップ・レベル分岐と処理
//          既にconstは外れていること
//----------------------------------------------------------------------------

//      ---<<< プライマリ >>>---

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType,
    class tEnable
>
struct BranchedProcess { };

//      ---<<< トップ・レベル >>>---

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType
>
struct BranchedProcess
<
    tTrackingMode,
    tTheolizerVersion,
    tSerializer,
    tType,
    EnableIf<tSerializer::kHasDestination>
>
{
    static void process
    (
        tSerializer&    iSerializer,
        tType&          ioInstance,
        char const*     iName,
        char const*     iFileName,
        size_t          iLineNo
    )
    {
        // upVersionカウンタ・クリア
        theolizer::internal::getUpVersionCount()=0;
//std::cout << "(0.1)UpVersionCount=" << theolizer::internal::getUpVersionCount()
//          << " " << THEOLIZER_INTERNAL_TYPE_NAME(tType) << std::endl;

        // 型情報取得中継クラス登録
        TypeFunctions<tSerializer>  aTypeFunctions;

        // TypeInfoListへ登録
        RegisterType<tSerializer, tType, tTheolizerVersion>::getInstance();

        // エラー情報登録
        ApiBoundary aApiBoundary(&iSerializer.mAdditionalInfo);
        BaseSerializer::AutoRestoreSerializeInfo aAutoRestoreSerializeInfo
            (iSerializer, iName, 0, std::is_pointer<tType>::value, iFileName, iLineNo);

        // 例外の処理
        try
        {
            // エラーが発生していたら戻る
            iSerializer.checkError();

            // 保存／回復処理
            if (iSerializer.mIsSaver)
            {
                BaseSerializer::AutoRestoreSaveProcess
                    aAutoRestoreSaveTop(iSerializer, getTypeIndex<tType>());

                // 保存処理呼び出し
                Switcher<BaseSerializer, tType, true, tTrackingMode>::save(iSerializer,ioInstance);
            }
            else
            {
                BaseSerializer::AutoRestoreLoadProcess
                    aAutoRestoreLoadTop(iSerializer, getTypeIndex<tType>());

                // 回復処理呼び出し
                Switcher<BaseSerializer, tType, true, tTrackingMode>::load(iSerializer,ioInstance);
            }
        }
        catch (ErrorInfo&)
        {
            // 例外要求なら再throw
            if (!iSerializer.getNoThrowException())
            {
throw;
            }
        }
    }
};

//      ---<<< 非トップ・レベル >>>---

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType
>
struct BranchedProcess
<
    tTrackingMode,
    tTheolizerVersion,
    tSerializer,
    tType,
    EnableIf<!tSerializer::kHasDestination>
>
{
    static void process
    (
        tSerializer&    iSerializer,
        tType&          ioInstance,
        char const*     iName,
        char const*     iFileName,
        size_t          iLineNo
    )
    {
        // upVersionCountのクリアと回復制御(processからの戻り時に元に戻す
        AutoRestore<unsigned> aUpVersionCount(getUpVersionCount(), 0);
//std::cout << "(0.2)UpVersionCount=" << theolizer::internal::getUpVersionCount()
//          << " " << THEOLIZER_INTERNAL_TYPE_NAME(tType) << std::endl;

        // TypeInfoListへ登録
        RegisterType<tSerializer, tType, tTheolizerVersion>::getInstance();

        // エラー情報登録
        ApiBoundary aApiBoundary(&iSerializer.mAdditionalInfo);
        BaseSerializer::AutoRestoreSerializeInfo aAutoRestoreSerializeInfo
            (iSerializer, iName, 0, std::is_pointer<tType>::value, iFileName, iLineNo);

        // エラーが発生していたら戻る
        iSerializer.checkError();

        // 保存／回復処理
        if (iSerializer.mIsSaver)
        {
            iSerializer.writePreElement(true);
            BaseSerializer::AutoRestoreSaveProcess
                aAutoRestoreSaveTop(iSerializer, getTypeIndex<tType>());
            Switcher<BaseSerializer, tType, true, tTrackingMode>::save(iSerializer, ioInstance);
        }
        else
        {
            if (iSerializer.readPreElement(true))
            {
                BaseSerializer::AutoRestoreLoadProcess
                    aAutoRestoreLoadTop(iSerializer, getTypeIndex<tType>());
                Switcher<BaseSerializer, tType, true, tTrackingMode>::load(iSerializer,ioInstance);
            }
        }
    }
};

//----------------------------------------------------------------------------
//      保存／回復分岐処理
//----------------------------------------------------------------------------

//      ---<<< const、もしくは、一時オブジェクト(右辺値) >>>---

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType
>
void process
(
    tSerializer&    iSerializer,
    tType const&    iInstance,
    char const*     iName,
    char const*     iFileName,
    size_t          iLineNo
)
{
    THEOLIZER_INTERNAL_ASSERT(iSerializer.mIsSaver, "Can not load to const variable.(%1%)", iName);

    // const外し
    typedef typename RemoveCV<tType>::type  Type;

    // 呼び出し
    //  Cスタイル・キャストを意図的に使用している
    //  例えば、char const* array[10];のようなオブジェクトを保存する時、
    //  constを解除しておかないと、後の処理でconst回避に苦労する。
    //  しかし、const領域へのポインタ配列は、C++スタイル・キャストでは
    //  constを解除できないため。
    BranchedProcess<tTrackingMode, tTheolizerVersion, tSerializer, Type>::
        process(iSerializer, (Type&)iInstance, iName, iFileName, iLineNo);
}

//      ---<<< 通常のオブジェクト >>>---

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType
>
void process
(
    tSerializer&    iSerializer,
    tType&          ioInstance,
    char const*     iName,
    char const*     iFileName,
    size_t          iLineNo
)
{
    BranchedProcess<tTrackingMode, tTheolizerVersion, tSerializer, tType>::
        process(iSerializer, ioInstance, iName, iFileName, iLineNo);
}

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal
}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_CORE_SWITCHER_H
