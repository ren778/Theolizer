﻿//############################################################################
//      各serializerのテスト
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

#include "test_serializer_base_auto_gen.h"
#include <theolizer/vector.h>
#include <theolizer/list.h>
#include <theolizer/memory.h>

namespace theolizer{namespace internal{
namespace global_table{
    MyGVNT::MyGVNT()
    {
        add(typeid(theolizer::internal::JsonMidOSerializer),1u,1u,1u,1u);
        add(typeid(theolizer::internal::JsonMidISerializer),1u,1u,1u,1u);
        add(typeid(IntrusiveBase),                      1u,1u,2u,2u);
        add(typeid(IntrusiveBase2),                     1u,1u,1u,1u);
        add(typeid(IntrusiveDerived),                   1u,2u,3u,4u);
        add(typeid(IntrusiveDerived2),                  1u,1u,1u,1u);
        add(typeid(DerivedChange),                      1u,1u,1u,1u);
        add(typeid(change_order::dummy::DerivedChange), 1u,1u,1u,1u);
        add(typeid(DerivedDelete),                      1u,1u,1u,1u);
        add(typeid(EnumTest),                           1u,2u,2u,2u);
        add(typeid(another::EnumTest),                  1u,1u,1u,1u);
        add(typeid(DestinationTest),                    1u,1u,1u,1u);
        add(typeid(Included1),                          1u,1u,1u,1u);
        add(typeid(Included2),                          1u,1u,1u,1u);
        add(typeid(Tracking),                           1u,1u,1u,1u);
        add(typeid(Dynamic),                            1u,1u,1u,1u);
        add(typeid(ArrayClass),                         1u,1u,2u,2u);
        add(typeid(NonIntrusiveBase),                   1u,1u,1u,1u);
        add(typeid(NonIntrusiveDrived),                 1u,1u,1u,1u);
        add(typeid(ArrayClassTemplatePrimary),          1u,1u,1u,1u);
        add(typeid(Pointers),                           1u,1u,1u,1u);
        add(typeid(vectorTheolizer),                    1u,1u,1u,1u);
        add(typeid(listTheolizer),                      1u,1u,1u,1u);
        add(typeid(unique_ptrTheolizer),                1u,1u,1u,1u);
    }
}   // namespace global_table
}}  // namespace theolizer
