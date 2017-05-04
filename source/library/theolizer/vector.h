﻿//############################################################################
//      std::vector<>のシリアライズ
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

#if !defined(THEOLIZER_INTERNAL_VECTOR_H)
#define THEOLIZER_INTERNAL_VECTOR_H

#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      Begin
//############################################################################

#include <vector>
#include "serializer.h"
#include "internal/containers.h"

THEOLIZER_PROVIDED_BY("Theoride Technology");

//############################################################################
//      std::vector<>対応
//############################################################################

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          template<class T, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::vector
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           T, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             vectorTheolizer
#define THEOLIZER_INTERNAL_IS_VECTOR
#include "internal/container_no_key.inc"
#undef  THEOLIZER_INTERNAL_IS_VECTOR

// ***************************************************************************
//      自動生成コード
// ***************************************************************************

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(std::vector<T, Alloc>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME vectorTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::vector",T,Alloc))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

// ***************************************************************************
//      定義したマクロの解放
// ***************************************************************************

#undef  THEOLZIER_INTERNAL_CONTAINER_PARAMETER
#undef  THEOLZIER_INTERNAL_CONTAINER_NAME
#undef  THEOLZIER_INTERNAL_CONTAINER_ARGUMENT
#undef  THEOLZIER_INTERNAL_CONTAINER_UNIQUE

#undef  THEOLIZER_INTERNAL_FULL_NAME
#undef  THEOLIZER_INTERNAL_FULL_NAME_POINTEE

//############################################################################
//      被ポインタ用(theolizer::VectorPointee<>)
//############################################################################

// ***************************************************************************
//      theolizer::VectorPointee<>
//          std::vector<>の単純な派生クラス
//          要素をPointeeとして処理する
// ***************************************************************************

namespace theolizer
{

template<class T, class Alloc=std::allocator<T> >
class THEOLIZER_ANNOTATE(CS) VectorPointee : public std::vector<T, Alloc>
{
public:
    using std::vector<T, Alloc>::vector;
};

}   // namespace theolizer

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          template<class T, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               theolizer::VectorPointee
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           T, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             VectorPointeeTheolizer
#define THEOLIZER_INTERNAL_IS_VECTOR
#define THEOLIZER_INTERNAL_POINTEE
#include "internal/container_no_key.inc"
#undef  THEOLIZER_INTERNAL_POINTEE
#undef  THEOLIZER_INTERNAL_IS_VECTOR

// ***************************************************************************
//      自動生成コード
// ***************************************************************************

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(theolizer::VectorPointee<T, Alloc>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME VectorPointeeTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"theolizer::VectorPointee",T,Alloc))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

// ***************************************************************************
//      定義したマクロの解放
// ***************************************************************************

#undef  THEOLZIER_INTERNAL_CONTAINER_PARAMETER
#undef  THEOLZIER_INTERNAL_CONTAINER_NAME
#undef  THEOLZIER_INTERNAL_CONTAINER_ARGUMENT
#undef  THEOLZIER_INTERNAL_CONTAINER_UNIQUE

#undef  THEOLIZER_INTERNAL_FULL_NAME
#undef  THEOLIZER_INTERNAL_FULL_NAME_POINTEE

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN

#endif  // THEOLIZER_INTERNAL_VECTOR_H